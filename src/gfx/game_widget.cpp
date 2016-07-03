#include "gfx/game_widget.h"
#include "gfx/mesh_maker.h"

#include "game/obj_bearded_man.h"
#include "game/world.h"

#include "util/error.h"

#include <cmath>
#include "region_iterator.hpp"

namespace bm {

// Given integer cell position make world pos
static inline Vec3f pos_for_cell(const Vec3i &i) {
    return Vec3f((float)i.getX() - GameWidget::CELL_SIZE * 0.5f,
                 (float)i.getY() - GameWidget::WALL_HEIGHT * 1.3f,
                 (float)i.getZ() - GameWidget::CELL_SIZE * 0.5f);
}

void GameWidget::initialize() {
    terrain_shader_ = bm::load_shader("colored_blocks");
    rgb_vox_shader_ = bm::load_shader("rgb_blocks");

    //
    // World setup and update terrain
    //
    //vol_ = std::make_unique<WorldPager>();
    //vol_slice_ = std::make_unique<SlabVolume>(vol_.get(), 64 * 1048576, 64);
    pv::Region world_reg(Vec3i(0, 0, 0),
                         Vec3i(WORLDSZ_X, WORLDSZ_Y, WORLDSZ_Z));
    volume_ = std::make_unique<RawVolume>(world_reg);
    populate::populate_voxels(world_reg, *volume_);

    //
    // Load models
    //
    load_rgb(ModelId::Cursor, "cursor");
    cursor_pos_ = Vec3i(VIEWSZ_X / 2, 4, VIEWSZ_Z / 2);

    world_ = std::make_unique<World>(*volume_);

    load_rgb(ModelId::MarkPick, "mark_pick", false);
    load_rgb(ModelId::MarkRoute, "mark_route", false);
    load_rgb(ModelId::MarkArea, "mark_area", false);
    load_rgb(ModelId::MarkRamp, "mark_ramp", false);

    load_rgb(ModelId::BeardedMan, "bearded_man");

    // Inanimate
//    auto ramp = load_rgb(ModelId::Ramp, "ramp");
    //ramp->mesh_->translation_ = QVector3D(-.5f, 0, -.5f);

    load_rgb(ModelId::Wood, "wood");
    load_rgb(ModelId::Boulder, "boulder");

    // Other
    auto xyz = load_rgb(ModelId::Xyz, "xyz");
    xyz->mesh_->scale_ *= 2.0f;
    xyz->mesh_->translation_ = QVector3D(-1.0f, -1.0f, -1.0f); // pivot

    create_ramps();

    camera_follow_cursor();
    update_terrain();

    // Keyboard input mode
    change_fsm(KeyFSM::Default);

    emit SIG_battle_log("Game started");
    emit SIG_battle_log("Hello");
    emit SIG_battle_log("Test");
}

template <class VxType>
class MyMarchingCubesController {
        //: public pv::DefaultMarchingCubesController<VoxelType>
public:
    using DensityType = uint8_t;
    using MaterialType = BlockType;

    MyMarchingCubesController() {
        thresh_ = DensityType(0);
    }

    DensityType convertToDensity(VxType voxel) {
        return voxel.getDensity();
    }
    MaterialType convertToMaterial(VxType voxel) {
        return voxel.getMaterial();
    }
    VxType blendMaterials(VxType a, VxType b, float /*weight*/) {
        auto d = std::max(a.getDensity(), b.getDensity());
        if (convertToDensity(a) > convertToDensity(b)) {
            return VoxelType(convertToMaterial(a), d);
        } else {
            return VoxelType(convertToMaterial(b), d);
        }
    }
    DensityType getThreshold(void) { return thresh_; }
    void setThreshold(DensityType t) { thresh_ = t; }

private:
    DensityType thresh_;
};

// For cubic extractor
class TerrainIsQuadNeeded {
   public:
    bool operator()(VoxelType back, VoxelType front,
                    VoxelType& materialToUse) {
        if (back.getDensity() == VoxelType::getMaxDensity()
            && front.getDensity() == VoxelType::getMinDensity())
        {
            materialToUse = static_cast<VoxelType>(back);
            return true;
        } else {
            return false;
        }
    }
};

// Extract the surface
void GameWidget::update_terrain() {
    update_terrain_model();
    update_ramps();
}

void GameWidget::update_terrain_model()
{
    auto org_y = cursor_pos_.getY();
    pv::Region reg2(Vec3i(0, org_y, 0),
                    Vec3i(VIEWSZ_X, org_y + VIEWSZ_Y - 1, VIEWSZ_Z));

    auto raw_mesh = pv::extractCubicMesh(
                volume_.get(), reg2, TerrainIsQuadNeeded(), true);
//    auto raw_mesh = pv::extractMarchingCubesMesh(volume_.get(), reg2,
//                                    MyMarchingCubesController<VoxelType>());

    std::cout << "terrain mesh #vertices: " << raw_mesh.getNoOfVertices()
              << std::endl;

    auto decoded_mesh = pv::decodeMesh(raw_mesh);

    // Pass the surface to the OpenGL window
    //
    terrain_.release();
    terrain_ = std::make_unique<Model>(
                mesh::create_opengl_mesh_from_raw(this, decoded_mesh),
                terrain_shader_
                );
    terrain_->mesh_->scale_ = QVector3D(1,1,1);
    // move terrain slab together with cursor
    terrain_->mesh_->translation_ = QVector3D(
                0.f, cursor_pos_.getY() - WALL_HEIGHT * .5f, 0.f);

    this->update();
}

void GameWidget::update_ramps()
{
    ramps_.clear();

    pv::Region reg = get_visible_region();

    util::each_in_region(reg, [this](int x, int y, int z) {
        VoxelType vox = world_->get_voxel(x, y, z);
        if (vox.is_ramp()) {
            VoxelReader  voxr(world_->get_volume(), Vec3i(x, y, z));
            // add_ramp will possibly add a ramp, but will ignore degenerate
            // cases when ramp can't exist
            // TODO: draw a warning symbol or destroy ramp?
            visual::Ramp::add_ramp(Vec3f(x, y - WALL_HEIGHT, z), voxr, ramps_);
        }
    });
}

// Returns pointer for temporary use and modification, do not store permanently
GameWidget::GameWidget(QWidget *parent)
    : GLVersion_Widget(parent) /*, lua_(true)*/ {
    //lua_.Load("scripts/game.lua");
}

Model *GameWidget::load_rgb(ModelId reg, const char *name, bool re_scale)
{
    std::string path("assets/model/");
    path += name;
    path += ".qb";
    return load_model(reg, path.c_str(), rgb_vox_shader_, re_scale);
}

Model *GameWidget::load_model(ModelId register_as,
                              const char *file,
                              ShaderPtr shad,
                              bool re_scale) {
    auto qb_model = std::make_unique<QBFile>(file);
    auto raw_mesh = qb_model->get_mesh_for_volume(0);
    qb_model->free_voxels_for_volume(0);

    auto opengl_mesh = mesh::create_opengl_mesh_from_raw(
                this,
                raw_mesh,
                Vec3f(0.f, 0.f, 0.f),
                // Rescale to fit in 1x1x1 cube or use typical 1/8=0.125
                // shrink for 8x8x8 model
                re_scale ? qb_model->get_downscale(0)
                         : Vec3f(.125f, .125f, .125f)
                );

    models_[register_as] = Model(opengl_mesh, shad);
    return &(models_[register_as]);
}

void GameWidget::create_ramps() {
    for (int r = 0; r < bm::NUM_RAMPS; ++r) {
        ModelId r_id = (ModelId)((int)ModelId::Ramp_B + r);

        auto m = mesh::create_ramp(this, r_id);
        BM_ASSERT(m);

        models_[r_id] = Model(m, terrain_shader_);
    }
}

const Model *GameWidget::find_model(ModelId id) const
{
    auto iter = models_.find(id);
    if (iter == models_.end()) { return nullptr; }
    return &iter->second;
}

void GameWidget::render_frame() {
    render_terrain_model();
    render_ramps();

    render_animate_objects();
    render_inanimate_objects();

    auto cursor = find_model(ModelId::Cursor);
    BM_ASSERT(cursor);
    render_model(*cursor, pos_for_cell(cursor_pos_), 0.f);

    render_orders();
    //render_debug_routes();
    render_marked_area();

    render_overlay_xyz();
}

void GameWidget::render_ramps() {

//    GLsizei count = 1;
    for (auto& r: ramps_) {
        auto modl = find_model(r.model_id);
        BM_ASSERT(modl);
        render_model(*modl, r.pos, r.rotation);
    }
//    glDrawElementsInstanced(GL_TRIANGLES,
//                            m->mesh_->indx_count_,
//                            m->mesh_->indx_type_,
//                            nullptr,
//                            count);

/*    auto m_ramp = models_.find(ModelId::Ramp);
    Vec3i p0, p1;
    get_visible_region(p0, p1);
    auto reg = util::make_region(p0, p1);

    for (int x = reg.getLowerX(); x <= reg.getUpperX(); x++) {
        for (int z = reg.getLowerZ(); z <= reg.getUpperZ(); z++) {
            for (int y = reg.getLowerY(); y <= reg.getUpperY(); y++) {
                Vec3i pos(x, y, z);
                if (world_->get_voxel(pos).is_ramp()) {
                    render_model(m_ramp->second,
                                 pos_for_cell(pos),
                                 0.f);
                }
            }
        }
    }
    */
}

pv::Region GameWidget::get_visible_region() {
    auto a = cursor_pos_ + Vec3i(-VIEWSZ_X/2, -1, -VIEWSZ_Z/2);
    auto b = cursor_pos_ + Vec3i(VIEWSZ_X/2, VIEWSZ_Y, VIEWSZ_Z/2);
    return pv::Region(a, b);
}

//void GameWidget::get_visible_region(Array3i& a, Array3i& b) {
//    Vec3i aa, bb;
//    get_visible_region(aa, bb);
//    a = util::make_array(aa);
//    b = util::make_array(bb);
//}

void GameWidget::render_animate_objects() {
    auto reg = get_visible_region();

    world_->for_each_animate_r(reg,
        [this](AnimateObject* ao, const Vec3i& pos) {
            auto ent = ao->as_entity();
            if (ent) {
                auto model_id = ent->get_model_id();
                if (model_id != ModelId::NIL) {
                    auto m = this->find_model(model_id);
                    Q_ASSERT(m);
                    this->render_model(*m, pos_for_cell(pos), 0.0f);
                } // have model
            } // ent
        });
}

void GameWidget::render_inanimate_objects() {
    auto reg = get_visible_region();

    world_->for_each_inanimate_r(reg,
        [this](InanimateObject& ia, const Vec3i& pos) {
            auto m = this->find_model(ia.model_);
            Q_ASSERT(m);
            this->render_model(*m, pos_for_cell(pos), 0.0f);
        });
}

void GameWidget::render_orders() {
    render_orders(world_->get_orders());
    render_orders(world_->get_orders_low());
    render_orders(world_->get_orders_verylow());
}

void GameWidget::render_orders(const ai::OrderMap& order_map)
{
    Vec3f offset(0, -0.125f, 0);

    for (auto o_iter: order_map) {
        auto& o = *o_iter.second;
        auto pos = o.ctx_.pos_;
        auto model = find_model(o.visual_.model_id_);
        render_model(*model, pos_for_cell(pos) + offset, 0.f);
    }
}

void GameWidget::render_marked_area() {
    if (not mark_begin_.has_value()) {
        return;
    }

    auto m_mark = models_.find(ModelId::MarkArea);
    auto reg = util::make_region(cursor_pos_, mark_begin_.get_value());

    // Completely ignore Y, assume it is current for cursor depth
    for (int x = reg.getLowerX(); x <= reg.getUpperX(); x++) {
        for (int z = reg.getLowerZ(); z <= reg.getUpperZ(); z++) {
            render_model(m_mark->second,
                         pos_for_cell(Vec3i(x, cursor_pos_.getY() - 1, z)),
                         0.f);
        }
    }
}

void GameWidget::render_debug_routes()
{
    auto m_flag = models_.find(ModelId::MarkRoute);
    Vec3i oneup(0,-1,0);

    for (auto o_iter: world_->get_animate_objects()) {
        const AnimateObject *o = o_iter.second;
        auto ent = o->as_entity();
        auto& route = ent->get_route();
        for (auto step: route) {
            render_model(m_flag->second, pos_for_cell(step + oneup), 0.f);
        }
    }
}

void GameWidget::render_model(const Model& m, const Vec3f &pos, float rot_y)
{
    if (not m.mesh_->is_valid()) {
        return;
    }

    m.shad_->bind();

    // These two matrices are constant for all meshes.
    m.shad_->setUniformValue("viewMatrix", this->get_view_matrix());
    m.shad_->setUniformValue("projectionMatrix", this->get_projection_matrix());

    // Set up the model matrrix based on provided translation and scale.
    //
    QMatrix4x4 model_mx;
    model_mx.translate(m.mesh_->translation_
                       + QVector3D(pos.getX(), pos.getY(), pos.getZ()));
    model_mx.rotate(rot_y + m.mesh_->rotation_y_, 0.f, 1.f, 0.f);
    model_mx.scale(m.mesh_->scale_);
    model_mx.translate(-m.mesh_->translation_);

    m.shad_->setUniformValue("modelMatrix", model_mx);

    // Bind the vertex array for the current mesh
    glBindVertexArray(m.mesh_->vert_array_);
    // Draw the mesh
    glDrawElements(GL_TRIANGLES,
                   m.mesh_->indx_count_,
                   m.mesh_->indx_type_,
                   nullptr);
    // Unbind the vertex array.
    glBindVertexArray(0);

    // We're done with the shader for this frame.
    m.shad_->release();
}

void GameWidget::render_terrain_model()
{
    const Model& m = *terrain_;
    if (not m.mesh_->is_valid()) {
        return;
    }

    m.shad_->bind();

    // These two matrices are constant for all meshes.
    m.shad_->setUniformValue("viewMatrix", this->get_view_matrix());
    m.shad_->setUniformValue("projectionMatrix", this->get_projection_matrix());

    // Use focusDepth as max brightness layer and make other layers darker
    // Assume that mesh always originates at depth 0
    m.shad_->setUniformValue("focusDepth", 0.f);

    // Set up the model matrrix based on provided translation and scale.
    //
    QMatrix4x4 model_mx;
    //model_mx.translate(QVector3D(0.f, cursor_pos_.getY() - 0.5f, 0.f));
    model_mx.translate(m.mesh_->translation_);
    model_mx.scale(m.mesh_->scale_);

    m.shad_->setUniformValue("modelMatrix", model_mx);

    // Bind the vertex array for the current mesh
    glBindVertexArray(m.mesh_->vert_array_);
    // Draw the mesh
    glDrawElements(GL_TRIANGLES,
                   m.mesh_->indx_count_,
                   m.mesh_->indx_type_, 0);
    // Unbind the vertex array.
    glBindVertexArray(0);

    // We're done with the shader for this frame.
    m.shad_->release();
}

QVector3D GameWidget::get_camera_focus(QVector3D) {
    auto cur = pos_for_cell(cursor_pos_);
    return QVector3D(cur.getX(), cur.getY(), cur.getZ());
}

QVector3D GameWidget::get_camera_up(QVector3D, QVector3D) {
    return QVector3D(0.0f, -1.0f, 0.0f);
}

QVector3D GameWidget::get_cam_forward() const {
    auto cur = pos_for_cell(cursor_pos_);
    auto forw = QVector3D(cur.getX() - cam_pos_.x(),
                          cur.getY() - cam_pos_.y(),
                          cur.getZ() - cam_pos_.z());
    forw.normalize();
    return forw;
}

void GameWidget::render_overlay_xyz() {
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0,
               this->geometry().width() * 0.25f,
               this->geometry().height() * 0.25f);

    QVector3D cam_f = this->get_cam_forward() * 5.0f;

    view_matrix_.setToIdentity();
    view_matrix_.lookAt(
        -cam_f,  // Camera is at
        QVector3D(0.f, 0.f, 0.f),  // Look at
        QVector3D(0.f, 1.f, 0.f)   // Camera up
        );

    auto xyz = find_model(ModelId::Xyz);
    Q_ASSERT(xyz);
    render_model(*xyz, Vec3f(0.f, 0.f, 0.f), 0.f);

    glViewport(0, 0,
               this->geometry().width(),
               this->geometry().height());
    glEnable(GL_DEPTH_TEST);
}

void GameWidget::camera_follow_cursor()
{
    QVector3D cam_pos((cursor_pos_.getX() - 3) * CELL_SIZE,
                      (cursor_pos_.getY() - 15.0f) * WALL_HEIGHT, // up
                      (cursor_pos_.getZ() + 6) * CELL_SIZE);

    set_camera_transform(cam_pos,
                         -7.0 * M_PI / 18.0, //pitch (minus - look down)
                         M_PI); // yaw
    on_cursor_changed();
}

void GameWidget::on_cursor_changed()
{
    emit SIG_cursor_changed(QPoint(cursor_pos_.getX(),
                                   cursor_pos_.getZ()),
                            cursor_pos_.getY());
}

void GameWidget::change_keyboard_fsm(bm::KeyFSM fsm_state)
{
    switch (fsm_state) {
    case KeyFSM::Default:
        keyboard_handler_ = &GameWidget::fsm_keypress_exploremap;
        break;
//    case KeyFSM::Orders:
//        keyboard_handler_ = &GameWidget::fsm_keypress_orders;
//        break;
    case KeyFSM::Designations:
        keyboard_handler_ = &GameWidget::fsm_keypress_designations;
        break;
    }
    emit SIG_keyboard_fsm_changed(fsm_state);
}

bool GameWidget::keypress_navigate_cursor(QKeyEvent* event) {
    switch ( event->key() ) {
    case Qt::Key_Period: {
            world_->think();
            if (world_->force_update_terrain_mesh_) {
                update_terrain();
                world_->force_update_terrain_mesh_ = false;
            }
            this->update();
            event->accept();
        } break;
    case Qt::Key_Comma: {
            auto v = world_->get_voxel(cursor_pos_);
            v.set_ramp(not v.is_ramp());
            world_->set_voxel(cursor_pos_, v);
            update_terrain();
            this->update();
            event->accept();
        } break;
    case Qt::Key_Left:
        if (cursor_pos_.getX() < WORLDSZ_X - 1) {
            cursor_pos_ += Vec3i(-1, 0, 0);
            camera_follow_cursor();
        }
        break;
    case Qt::Key_Right:
        if (cursor_pos_.getX() > 0) {
            cursor_pos_ += Vec3i(1, 0, 0);
            camera_follow_cursor();
        }
        break;
    case Qt::Key_Down:
        if (cursor_pos_.getZ() < WORLDSZ_Z - 1) {
            cursor_pos_ += Vec3i(0, 0, 1);
            camera_follow_cursor();
        }
        break;
    case Qt::Key_Up:
        if (cursor_pos_.getZ() > 0) {
            cursor_pos_ += Vec3i(0, 0, -1);
            camera_follow_cursor();
        }
        break;
    case Qt::Key_Minus:
        if (cursor_pos_.getY() > 0) {
            cursor_pos_ += Vec3i(0, -1, 0);
            // this is to emit ui update, camera doesn't really move
            camera_follow_cursor();
            update_terrain();
        }
        break;
    case Qt::Key_Plus:
        if (cursor_pos_.getY() < WORLDSZ_Y - 1) {
            cursor_pos_ += Vec3i(0, 1, 0);
            // this is to emit ui update, camera doesn't really move
            camera_follow_cursor();
            update_terrain();
        }
        break;
    default:
        return false;
    }
    event->accept();
    return true;
}

// Normal keyboard mode where you can navigate camera and map, possibly switch
// to different modes.
void GameWidget::fsm_keypress_exploremap(QKeyEvent *event) {
    if (keypress_navigate_cursor(event)) {
        return;
    }
    switch ( event->key() ) {
//    case Qt::Key_O: {
//        change_keyboard_fsm(KeyFSM::Orders);
//        break;
//    }
    case Qt::Key_D: {
        change_keyboard_fsm(KeyFSM::Designations);
        event->accept();
        break;
    }
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_A:
//    case Qt::Key_D:
//    case Qt::Key_Escape:
        return GLVersion_Widget::keyPressEvent(event);
    default:
        event->ignore();
        break;
    }
}

void GameWidget::fsm_keypress_orders(QKeyEvent *event) {
    switch ( event->key() ) {
    case Qt::Key_Escape:
        change_keyboard_fsm(KeyFSM::Default);
        event->accept();
        break;
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_A:
    case Qt::Key_D:
        return GLVersion_Widget::keyPressEvent(event);
    default:
        event->ignore();
        break;
    }
}

void GameWidget::fsm_keypress_designations(QKeyEvent *event)
{
    if (keypress_navigate_cursor(event)) {
        return;
    }
    switch ( event->key() ) {
    case Qt::Key_Escape:
        change_keyboard_fsm(KeyFSM::Default);
        event->accept();
        break;
    case Qt::Key_M: {
        if (mark_begin_.has_value()) {
            qDebug() << "mark drop";
            mark_begin_ = {};
        } else {
            qDebug() << "mark begin" << cursor_pos_;
            mark_begin_ = cursor_pos_;
        }
        event->accept();
        break;
    }
    case Qt::Key_D: {
        // {D}esignations -> {D} mine
        // Records player's wish to have some blocks digged
        world_->add_goal(mark_begin_, cursor_pos_, [this](const Vec3i& pos) {
            this->world_->add_goal_dig(pos);
        });
        mark_begin_ = {};
        // Order accepted, return to default
        change_keyboard_fsm(KeyFSM::Default);
        event->accept();
    } break;
    case Qt::Key_R: {
        // {D}esignations -> {R}amp
        // Records player's wish to have ramp created
        world_->add_goal(mark_begin_, cursor_pos_, [this](const Vec3i& pos) {
            this->world_->add_goal_ramp(pos);
        });
        mark_begin_ = {};
        // Order accepted, return to default
        change_keyboard_fsm(KeyFSM::Default);
        event->accept();
    } break;
//    case Qt::Key_W:case Qt::Key_S:case Qt::Key_A:case Qt::Key_D:
//        return GLVersion_Widget::keyPressEvent(event);
    default:
        event->ignore();
        break;
    }
}

} // namespace bm
