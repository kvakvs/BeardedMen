#include "game.h"

#include <cmath>

namespace bm {

void GameWidget::initialize_game() {
    terrain_shader_ = load_shader("colored_blocks");
    rgb_vox_shader_ = load_shader("rgb_blocks");

    //
    // World setup and update terrain
    //
    vol_ = std::make_unique<WorldPager>();

    //
    // Load models
    //
    dorf_ = load_model("dorf", "assets/model/dorf.qb", rgb_vox_shader_);
    cursor_ = load_model("cursor", "assets/model/cursor.qb", rgb_vox_shader_);
    cursor_pos_ = Vec3i(VIEWSZ_X / 2, 1, VIEWSZ_Z / 2);

    follow_cursor();
    update_terrain_model();
}

void GameWidget::update_terrain_model() {
    SlabVolume vol_slice(vol_.get(), 64 * 1024 * 1024, 64);

    // Prefetch slab sized viewszx x viewszz and of height viewszy+2
    // Zero top cells, so that mesh looks good
    Vec3i half_view(VIEWSZ_X / 2, 0, VIEWSZ_Z / 2);
    pv::Region reg(cursor_pos_ - half_view,
                   Vec3i(VIEWSZ_X, VIEWSZ_Y+1, VIEWSZ_Z));
    vol_slice.prefetch(reg);
    //    vol_slice.flushAll();

    // Zero upper layer
    VoxelType empty(0, VoxelType::getMinDensity());
    for (int x = reg.getWidthInCells(); x >= 0; --x) {
        for (int z = reg.getDepthInCells(); z >= 0; --z) {
            vol_slice.setVoxel(x, 0, z, empty);
        }
    }
    vol_slice.setVoxel(0, 0, 0, VoxelType(1, VoxelType::getMaxDensity()));
    vol_slice.setVoxel(1, 0, 0, VoxelType(2, VoxelType::getMaxDensity()));
    vol_slice.setVoxel(2, 0, 0, VoxelType(2, VoxelType::getMaxDensity()));
    vol_slice.setVoxel(0, 0, 1, VoxelType(3, VoxelType::getMaxDensity()));
    vol_slice.setVoxel(0, 0, 2, VoxelType(3, VoxelType::getMaxDensity()));

    //
    // Extract the surface
    //
    pv::Region reg2(Vec3i(0, 0, 0),
                    Vec3i(VIEWSZ_X, VIEWSZ_Y, VIEWSZ_Z));

    auto mesh = pv::extractCubicMesh(&vol_slice, reg2);
    //auto mesh = pv::extractMarchingCubesMesh(&vol_slice, reg2);
    std::cout << "#vertices: " << mesh.getNoOfVertices() << std::endl;

    auto decodedMesh = pv::decodeMesh(mesh);

    // Pass the surface to the OpenGL window
    //
    terrain_ = Model(create_opengl_mesh_from_raw(decodedMesh), terrain_shader_);
    terrain_.mesh_->scale_.setY(-1.0f);

    // Keyboard input mode
    change_keyboard_fsm(KeyFSM::ExploreMap);
}

Model GameWidget::load_model(const char *register_as,
                             const char *file,
                             ShaderPtr shad) {
    auto qb_model = std::make_unique<QBFile>(file);
    auto raw_mesh = qb_model->get_mesh_for_volume(0);

    auto opengl_mesh = create_opengl_mesh_from_raw(
                raw_mesh,
                Vec3f(0.f, 0.f, 0.f),
                qb_model->get_downscale(0)
                );

    raw_meshes_[register_as] = opengl_mesh;
    return Model(opengl_mesh, shad);
}

void GameWidget::render_frame() {
    terrain_.render(this, Vec3f(0.f, 0.f, 0.f), 0.f);
    dorf_.render(this, Vec3f(-0.5f, 0.5f, -0.5f), 0.f);

    cursor_.render(this,
                   Vec3f((float)cursor_pos_.getX() - 0.5f,
                         -(float)cursor_pos_.getY() + 0.5f,
                         (float)cursor_pos_.getZ() - 0.5f),
                   0.f);
}

void GameWidget::follow_cursor()
{
    setCameraTransform(QVector3D(cursor_pos_.getX(), // x
                                 15.0f, // height above field
                                 cursor_pos_.getZ() + 7), // z+some cells down
                       -PI/3, //pitch (minus - look down)
                       PI); // yaw
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
        if (cursor_pos_.getX() < world_sz_x - 1) {
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
        if (cursor_pos_.getZ() < world_sz_z - 1) {
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
    case Qt::Key_O: {
        change_keyboard_fsm(KeyFSM::Orders);
        break;
    }
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_A:
    case Qt::Key_D:
    case Qt::Key_Escape:
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
