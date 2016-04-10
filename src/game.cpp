#include "game.h"

#include <cmath>
#include <QElapsedTimer>

namespace nrdf {

void GameWidget::initializeExample() {
    terrain_shader_ = load_shader("colored_blocks");
    rgb_vox_shader_   = load_shader("rgb_blocks");

    vol_ = std::make_unique<WorldPager>();
    SlabVolume vol_slice(vol_.get(), 64 * 1024 * 1024, 64);
//    SlabVolume vol_slice(pv::Region(Vec3i(0, 0, 0),
//                                    Vec3i(view_sz_x, view_sz_y, view_sz_z)));

    // Just some tests of memory usage, etc.
    std::cout << "Memory usage: "
              << (vol_slice.calculateSizeInBytes() / 1024.0 / 1024.0) << "MB"
              << std::endl;

    pv::Region reg(Vec3i(0, 0, 0),
                   Vec3i(world_sz_x, world_sz_y, world_sz_z));
    std::cout << "Prefetching region: " << reg.getLowerCorner() << " -> "
              << reg.getUpperCorner() << std::endl;
    vol_slice.prefetch(reg);

    std::cout << "Flushing entire volume" << std::endl;
    vol_slice.flushAll();

    //
    // Extract the surface
    //
    pv::Region reg2(Vec3i(0, 0, 0),
                    Vec3i(view_sz_x, view_sz_y, view_sz_z));

    auto mesh = pv::extractCubicMesh(&vol_slice, reg2,
                                     IsQuadNeeded<VoxelType>());
    //         auto mesh = pv::extractCubicMesh(&volData, reg2);
    //        auto mesh = pv::extractMarchingCubesMesh(&volData, reg2);
    std::cout << "#vertices: " << mesh.getNoOfVertices() << std::endl;

    auto decodedMesh = pv::decodeMesh(mesh);

    // Pass the surface to the OpenGL window
    //
    terrain_ = Model(create_opengl_mesh_from_raw(decodedMesh), terrain_shader_);
    //terrain_.mesh_.rotation_y_ = -90.f;

    //
    // Load models
    //
    dorf_ = load_model("dorf", "assets/model/dorf.qb", rgb_vox_shader_);
    cursor_ = load_model("cursor", "assets/model/cursor.qb", rgb_vox_shader_);

    follow_cursor();
//    qtimer_.start();
}

Model GameWidget::load_model(const char *register_as,
                             const char *file,
                             ShaderPtr shad) {
    auto qb_model = std::make_unique<QBFile>(file);
    auto raw_mesh = qb_model->get_mesh_for_volume(0);

    // Using -.7f offset because 1 pixel is added all around the QB model and
    // model's 0,0,0 is 0.5f away from block origin. Scale down by original 1/8
    auto opengl_mesh = create_opengl_mesh_from_raw(
                raw_mesh,
                Vec3f(0.f, 0.f, 0.f), //Vec3f(-.7f, 0.5f, +.7f),
                qb_model->get_downscale(0)
                );

    raw_meshes_[register_as] = opengl_mesh;
    return Model(opengl_mesh, shad);
}

void GameWidget::renderOneFrame() {
    terrain_.render(this, Vec3f(0.f, 0.f, 0.f), 0.f);
    dorf_.render(this, Vec3f(0.f, 0.f, 0.f), 0.f);

    cursor_.render(this,
                   Vec3f((float)cursor_pos_.getX() - .6f,
                         (float)cursor_pos_.getY() + .5f,
                         (float)cursor_pos_.getZ() + .8f),
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

void GameWidget::keyPressEvent(QKeyEvent *event) {
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

} // ns nrdf
