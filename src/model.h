#pragma once

#include <QGLShaderProgram>

#include <string>
#include <map>

#include "vector.h"
#include "gl_version.h"

namespace bm {

// This structure holds all the data required to render one of our meshes
// through OpenGL
struct OpenglMesh {
    GLuint indx_count_;
    GLenum indx_type_;
    GLuint indx_buf_;
    GLuint vert_buf_;
    GLuint vert_array_;
    float rotation_y_ = 0.f;
    //QVector3D origin_;
    QVector3D translation_;
    QVector3D scale_;
};

using MeshMap = std::map<std::string, OpenglMesh>;
using ShaderPtr = QSharedPointer<QGLShaderProgram>;

class Model {
public:
    OpenglMesh mesh_;
    ShaderPtr shad_;

    Model() {}
    Model(const OpenglMesh &m, ShaderPtr shad): mesh_(m), shad_(shad) {}
    Model(MeshMap &storage, const char *name, ShaderPtr shad)
        : shad_(shad)
    {
        mesh_ = storage[name];
    }

    void render(GLVersion_Widget *gl,
                const Vec3f &pos, float rot_y);
};

} // namespace bm
