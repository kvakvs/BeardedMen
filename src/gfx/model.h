#pragma once

#include <QGLShaderProgram>

#include <string>
#include <map>

#include "util/vec.h"
#include "gfx/gl_version.h"
#include "model_id.h"

namespace bm {

// This structure holds all the data required to render one of our meshes
// through OpenGL. Managed using shared ptr, on destroy frees buffers.
class OpenglMesh {
public:
    using Ptr = std::shared_ptr<OpenglMesh>;

    OpenglMesh(GLVersion_Widget *gl): gl_(gl) {}
    OpenglMesh(const OpenglMesh &) = delete;
    OpenglMesh(OpenglMesh &&) = default;

    ~OpenglMesh() {
        destroy();
    }

    bool is_valid() const { return vert_array_ != 0; }

public:
    GLuint indx_count_ = 0;
    GLenum indx_type_ = 0;
    GLuint indx_buf_ = 0;
    GLuint vert_buf_ = 0;
    GLuint vert_array_ = 0;

    float rotation_y_ = 0.f;
    //QVector3D origin_;
    QVector3D translation_;
    QVector3D scale_;

private:
    // Mesh is an opengl resource, so it needs to have access to gl context
    // which is privately managed by this widget
    GLVersion_Widget *gl_ = nullptr;

    void destroy();
};

using ShaderPtr = QSharedPointer<QGLShaderProgram>;

//class Model;
// Implement this in Game or wherever ModelMap is located
//class IModelStorage {
//public:
//    virtual Model *find_model(ModelId) = 0;
//};

class Model {
public:
    OpenglMesh::Ptr mesh_;
    ShaderPtr shad_;

    Model() {}
    Model(OpenglMesh::Ptr m, ShaderPtr shad): mesh_(m), shad_(shad) {}
    ~Model() {}
};

//using MeshMap = std::map<ModelId, OpenglMesh::Ptr>;
using ModelMap = std::map<ModelId, Model>;

} // namespace bm
