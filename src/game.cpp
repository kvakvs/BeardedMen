#include "game.h"

#include <cmath>

namespace bm {

void GameWidget::initialize_game() {
    terrain_shader_ = load_shader("colored_blocks");
    rgb_vox_shader_ = load_shader("rgb_blocks");

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
    cursor_ = load_model("cursor", "assets/model/cursor.qb", rgb_vox_shader_);
    cursor_red_ = load_model("cursor_red", "assets/model/cursor_red.qb",
                             rgb_vox_shader_);
    cursor_pos_ = Vec3i(VIEWSZ_X / 2, 8, VIEWSZ_Z / 2);

    // Spawn one bearded man
    world_ = std::make_unique<World>(*volume_);
    auto bm_mod = load_model("dorf", "assets/model/dorf.qb", rgb_vox_shader_);
    auto bman = new BeardedMan(bm_mod, cursor_pos_);
    world_->add(bman);

    wood_ = load_model("wood", "assets/model/wood.qb", rgb_vox_shader_);
//    grass_[0] = load_model("grass1", "assets/model/grass1.qb", rgb_vox_shader_);
//    grass_[1] = load_model("grass2", "assets/model/grass2.qb", rgb_vox_shader_);
//    grass_[2] = load_model("grass3", "assets/model/grass3.qb", rgb_vox_shader_);
    xyz_ = load_model("xyz", "assets/model/xyz.qb", rgb_vox_shader_);
    xyz_.mesh_->scale_ *= 2.0f;
    xyz_.mesh_->translation_ = QVector3D(-1.0f, -1.0f, -1.0f); // pivot

    follow_cursor();
    update_terrain_model();

    // Keyboard input mode
    change_keyboard_fsm(KeyFSM::ExploreMap);
}

// Extract the surface
void GameWidget::update_terrain_model() {
    auto org_y = cursor_pos_.getY();
    pv::Region reg2(Vec3i(0, org_y, 0),
                    Vec3i(VIEWSZ_X, org_y + VIEWSZ_Y - 1, VIEWSZ_Z));

    auto raw_mesh = pv::extractCubicMesh(
                volume_.get(), reg2, TerrainIsQuadNeeded(), true);
    //auto mesh = pv::extractMarchingCubesMesh(&vol_slice, reg2);
    std::cout << "terrain mesh #vertices: " << raw_mesh.getNoOfVertices()
              << std::endl;

    auto decoded_mesh = pv::decodeMesh(raw_mesh);

    // Pass the surface to the OpenGL window
    //
    terrain_.release();
    terrain_ = std::make_unique<Model>(
                create_opengl_mesh_from_raw(decoded_mesh), terrain_shader_);
    terrain_->mesh_->scale_.setY(-WALL_HEIGHT);
    // move terrain slab together with cursor
    terrain_->mesh_->translation_.setY(-cursor_pos_.getY());

    this->update();
} // upd terrain

Model GameWidget::load_model(const char *register_as,
                             const char *file,
                             ShaderPtr shad) {
    auto qb_model = std::make_unique<QBFile>(file);
    auto raw_mesh = qb_model->get_mesh_for_volume(0);
    qb_model->free_voxels_for_volume(0);

    auto opengl_mesh = create_opengl_mesh_from_raw(
                raw_mesh,
                Vec3f(0.f, 0.f, 0.f),
                qb_model->get_downscale(0)
                );

    raw_meshes_[register_as] = opengl_mesh;
    return Model(opengl_mesh, shad);
}

void GameWidget::render_frame() {
    terrain_->render(this, Vec3f(0.f, 0.f, 0.f), 0.f);
    //dorf_.render(this, pos_for_cell(dorf_pos_), 0.f);

    world_->each_ent([this](EntityId id, IEntity *e) {
        auto r = dynamic_cast<IRenderable*>(e);
        if (r) {
            auto model = r->get_model();
            if (model) {
                model->render(this, pos_for_cell(e->get_pos()), 0.0f);
            }
        }
    });

    //wood_.render(this, pos_for_cell(dorf_pos_+Vec3i(0,0,2)), 0.f);

    cursor_.render(this, pos_for_cell(cursor_pos_), 0.f);

    render_overlay_xyz();
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

    xyz_.render(this, Vec3f(0.f, 0.f, 0.f), -cam_yaw_);
    glEnable(GL_DEPTH_TEST);
}

void GameWidget::follow_cursor()
{
    QVector3D cam_pos(cursor_pos_.getX() * CELL_SIZE,
                      (15.0f - cursor_pos_.getY()) * WALL_HEIGHT, // up
                      (cursor_pos_.getZ() + 7) * CELL_SIZE);
    setCameraTransform(cam_pos,
                       -7.0*PI/18.0, //pitch (minus - look down)
                       PI); // yaw
    on_cursor_changed();
}

void GameWidget::on_cursor_changed()
{
    emit SIG_cursor_changed(QPoint(cursor_pos_.getX(),
                                   cursor_pos_.getZ()),
                            cursor_pos_.getY());
}

void GameWidget::change_keyboard_fsm(GameWidget::KeyFSM id)
{
    switch (id) {
    case KeyFSM::ExploreMap:
        keyboard_handler_ = &GameWidget::fsm_keypress_exploremap;
        break;
    case KeyFSM::Orders:
        keyboard_handler_ = &GameWidget::fsm_keypress_orders;
        break;
    }
}

// Normal keyboard mode where you can navigate camera and map, possibly switch
// to different modes.
void GameWidget::fsm_keypress_exploremap(QKeyEvent *event) {
    switch ( event->key() ) {
    case Qt::Key_Right:
        if (cursor_pos_.getX() < WORLDSZ_X - 1) {
            cursor_pos_ += Vec3i(1, 0, 0);
            follow_cursor();
        }
        break;
    case Qt::Key_Left:
        if (cursor_pos_.getX() > 0) {
            cursor_pos_ += Vec3i(-1, 0, 0);
            follow_cursor();
        }
        break;
    case Qt::Key_Down:
        if (cursor_pos_.getZ() < WORLDSZ_Z - 1) {
            cursor_pos_ += Vec3i(0, 0, 1);
            follow_cursor();
        }
        break;
    case Qt::Key_Up:
        if (cursor_pos_.getZ() > 0) {
            cursor_pos_ += Vec3i(0, 0, -1);
            follow_cursor();
        }
        break;
    case Qt::Key_Minus:
        if (cursor_pos_.getY() > 0) {
            cursor_pos_ += Vec3i(0, -1, 0);
            // this is to emit ui update, camera doesn't really move
            follow_cursor();
//            on_cursor_changed();
            update_terrain_model();
        }
        break;
    case Qt::Key_Plus:
        if (cursor_pos_.getY() < WORLDSZ_Y - 1) {
            cursor_pos_ += Vec3i(0, 1, 0);
            // this is to emit ui update, camera doesn't really move
            follow_cursor();
//            on_cursor_changed();
            update_terrain_model();
        }
        break;
//    case Qt::Key_O: {
//        change_keyboard_fsm(KeyFSM::Orders);
//        break;
//    }
    case Qt::Key_Period: {
        world_->think();
        this->update();
    } break;

    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_A:
    case Qt::Key_D:
//    case Qt::Key_Escape:
        return BaseWidget::keyPressEvent(event);
    default:
        event->ignore();
        break;
    }
}
void GameWidget::fsm_keypress_orders(QKeyEvent *event) {
    switch ( event->key() ) {
    case Qt::Key_Escape:
        change_keyboard_fsm(KeyFSM::ExploreMap);
        break;
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_A:
    case Qt::Key_D:
        return BaseWidget::keyPressEvent(event);
    default:
        event->ignore();
        break;
    }
}

} // namespace bm
