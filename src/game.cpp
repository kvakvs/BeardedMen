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
    terrain_mesh_ = addMesh(decodedMesh);
    terrain_mesh_.rotation_y_ = -90.f;

    //
    // Load models
    //
    model1_      = std::make_unique<QBFile>("assets/model/dorf.qb");
    auto m1_tmp  = model1_->get_mesh_for_volume(0);
    // Using -.7f offset because 1 pixel is added all around the QB model and
    // model's 0,0,0 is 0.5f away from block origin. Scale down by original 1/8
    model1_mesh_ = addMesh(m1_tmp,
                           Vec3f(-.7f, 0.5f, +.7f),
                           model1_->get_downscale(0));

    cursor_  = std::make_unique<QBFile>("assets/model/cursor.qb");
    auto cur_tmp  = cursor_->get_mesh_for_volume(0);
    cursor_mesh_ = addMesh(cur_tmp,
                           Vec3f(-.7f, 0.5f, -.7f),
                           cursor_->get_downscale(0));
    follow_cursor();
//    qtimer_.start();
}

void GameWidget::renderOneFrame() {
    render_model(terrain_mesh_, terrain_shader_);
    render_model(model1_mesh_, rgb_vox_shader_);

//    cursor_mesh_.rotation_y_ =
//            std::sin(qtimer_.elapsed() % 314159 / 1000.f) * 15.f;
    cursor_mesh_.translation_ = QVector3D((float)cursor_pos_.getX() - .6f,
                                         (float)cursor_pos_.getY() + .5f,
                                         (float)cursor_pos_.getZ() + .8f),
    render_model(cursor_mesh_, rgb_vox_shader_);
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
    default:
        event->ignore();
        break;
    }
}

void GameWidget::render_model(OpenGLMeshData &mesh,
                              BaseWidget::ShaderPtr shad) {
    shad->bind();

    // These two matrices are constant for all meshes.
    shad->setUniformValue("viewMatrix", viewMatrix());
    shad->setUniformValue("projectionMatrix", projectionMatrix());

    // Set up the model matrrix based on provided translation and scale.
    QMatrix4x4 model_mx;
    model_mx.translate(mesh.translation_);
    model_mx.scale(mesh.scale_);
    // rotate all models (but not terrain, there is -90 to compensate for this)
    model_mx.rotate(mesh.rotation_y_ + 90.f, 0.f, 1.f, 0.f);
    shad->setUniformValue("modelMatrix", model_mx);

    // Bind the vertex array for the current mesh
    this->glBindVertexArray(mesh.vert_array_);
    // Draw the mesh
    this->glDrawElements(GL_TRIANGLES, mesh.indx_count_, mesh.indx_type_, 0);
    // Unbind the vertex array.
    this->glBindVertexArray(0);

    // We're done with the shader for this frame.
    shad->release();
}

} // ns nrdf
