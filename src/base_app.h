#pragma once

#include "PolyVox/Mesh.h"

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <cmath>

#include "gl_version.h"
#include "vector.h"
#include "model.h"

namespace pv = PolyVox;
namespace bm {

class BaseWidget : public GLVersion_Widget {
    Q_OBJECT
   public:
    BaseWidget(QWidget* parent) : MyGLWidget(parent) {}
    virtual ~BaseWidget();

    // Convert a PolyVox mesh to OpenGL index/vertex buffers. Inlined because
    // it's templatised.
    template <typename MeshType>
    OpenglMesh create_opengl_mesh_from_raw(
            const MeshType& surfaceMesh,
            const Vec3f& trans = Vec3f(0,0,0),
            const Vec3f &scale = Vec3f(1.0f, 1.0f, 1.0f))
    {
        // This struct holds the OpenGL properties (buffer handles, etc) which
        // will be used to render our mesh. We copy the data from the PolyVox
        // mesh into this structure.
        OpenglMesh result;

        // Create the VAO for the mesh
        glGenVertexArrays(1, &(result.vert_array_));
        glBindVertexArray(result.vert_array_);

        // The GL_ARRAY_BUFFER will contain the list of vertex positions
        glGenBuffers(1, &(result.vert_buf_));
        glBindBuffer(GL_ARRAY_BUFFER, result.vert_buf_);
        glBufferData(GL_ARRAY_BUFFER,
                     surfaceMesh.getNoOfVertices()
                        * sizeof(typename MeshType::VertexType),
                     surfaceMesh.getRawVertexData(), GL_STATIC_DRAW);

        // and GL_ELEMENT_ARRAY_BUFFER will contain the indices
        glGenBuffers(1, &(result.indx_buf_));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.indx_buf_);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            surfaceMesh.getNoOfIndices() * sizeof(typename MeshType::IndexType),
            surfaceMesh.getRawIndexData(), GL_STATIC_DRAW);

        // Every surface extractor outputs valid positions for the vertices, so
        // tell OpenGL how these are laid out
        glEnableVertexAttribArray(0);  // Attrib '0' is the vertex positions
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE, sizeof(typename MeshType::VertexType),
            (GLvoid*)(offsetof(
                typename MeshType::VertexType,
                position)));
        // take the first 3 floats from every
        //      sizeof(decltype(vecVertices)::value_type)

        // Some surface extractors also generate normals, so tell OpenGL how
        // these are laid out. If a surface extractor does not generate normals
        // then nonsense values are written into the buffer here and sghould be
        // ignored by the shader. This is mostly just to simplify this example
        // code - in a real application you will know whether your chosen surface
        // extractor generates normals and can skip uploading them if not.
        glEnableVertexAttribArray(1);  // Attrib '1' is the vertex normals.
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE, sizeof(typename MeshType::VertexType),
            (GLvoid*)(offsetof(typename MeshType::VertexType, normal)));

        // Finally a surface extractor will probably output additional data.
        // This is highly application dependant. For this example code we're
        // just uploading it as a set of bytes which we can read individually,
        // but real code will want to do something specialised here.
        glEnableVertexAttribArray(
            2);  // We're talking about shader attribute '2'
        GLint size = (std::min)(sizeof(typename MeshType::VertexType::DataType),
                                size_t(4));
        // Can't upload more that 4 components (vec4 is GLSL's biggest type)
        glVertexAttribIPointer(
            2, size, GL_UNSIGNED_BYTE, sizeof(typename MeshType::VertexType),
            (GLvoid*)(offsetof(typename MeshType::VertexType, data)));

        // We're done uploading and can now unbind.
        glBindVertexArray(0);

        // A few additional properties can be copied across for use during
        // rendering.
        result.indx_count_ = surfaceMesh.getNoOfIndices();
        result.translation_ = QVector3D(trans.getX(), trans.getY(),
                                         trans.getZ());
        result.scale_ = QVector3D(scale.getX(), scale.getY(),
                                   scale.getZ());

        // Set 16 or 32-bit index buffer size.
        result.indx_type_ = sizeof(typename MeshType::IndexType) == 2
                                 ? GL_UNSIGNED_SHORT
                                 : GL_UNSIGNED_INT;

        return result;
    }

    ShaderPtr load_shader(const char *name);

protected:
    const float PI = M_PI;

    virtual void initializeExample() = 0;
    void initialize() override;

   private:
    // Index/vertex buffer data
    //std::vector<OpenGLMeshData> mesh_data_;
    //QSharedPointer<QGLShaderProgram> shader_;
};

} // namespace bm
