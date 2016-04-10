#include "model.h"

namespace bm {

void Model::render(GLVersion_Widget *gl,
                   const Vec3f &pos, float rot_y)
{
    shad_->bind();

    // These two matrices are constant for all meshes.
    shad_->setUniformValue("viewMatrix", gl->viewMatrix());
    shad_->setUniformValue("projectionMatrix", gl->projectionMatrix());

    // Set up the model matrrix based on provided translation and scale.
    //
    QMatrix4x4 model_mx;
    model_mx.translate(mesh_.translation_
                       + QVector3D(pos.getX(), pos.getY(), pos.getZ()));
    model_mx.scale(mesh_.scale_);
    model_mx.rotate(rot_y + mesh_.rotation_y_, 0.f, 1.f, 0.f);

    shad_->setUniformValue("modelMatrix", model_mx);

    // Bind the vertex array for the current mesh
    gl->glBindVertexArray(mesh_.vert_array_);
    // Draw the mesh
    gl->glDrawElements(GL_TRIANGLES, mesh_.indx_count_, mesh_.indx_type_, 0);
    // Unbind the vertex array.
    gl->glBindVertexArray(0);

    // We're done with the shader for this frame.
    shad_->release();
}

} // namespace bm
