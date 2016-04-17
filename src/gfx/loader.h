#pragma once

#include "PolyVox/Mesh.h"

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <cmath>

#include "gfx/gl_version.h"
#include "util/vec.h"
#include "gfx/model.h"

namespace pv = PolyVox;
namespace bm {

class Loader {
public:
    // Convert a PolyVox mesh to OpenGL index/vertex buffers.
    template <typename MeshType>
    static OpenglMesh::Ptr create_opengl_mesh_from_raw(
            GLVersion_Widget* gl,
            const MeshType& mesh,
            const Vec3f& trans = Vec3f(0,0,0),
            const Vec3f& scale = Vec3f(1.0f, 1.0f, 1.0f))
    {
        const auto sizeof_vertex = sizeof(typename MeshType::VertexType);
        void *raw_v_data = const_cast<void*>(
                            static_cast<const void*>(mesh.getRawVertexData()));
        void *raw_i_data = const_cast<void*>(
                            static_cast<const void*>(mesh.getRawIndexData()));
        return create_mesh_from_raw_(
            gl,
            mesh.getNoOfVertices(), raw_v_data,
            sizeof_vertex,
            mesh.getNoOfIndices(), raw_i_data,
            sizeof(typename MeshType::IndexType),
            sizeof(typename MeshType::VertexType::DataType),
            (GLvoid*)(offsetof(typename MeshType::VertexType, position)),
            (GLvoid*)(offsetof(typename MeshType::VertexType, normal)),
            (GLvoid*)(offsetof(typename MeshType::VertexType, data)),
            trans, scale);
    }

    static ShaderPtr load_shader(const char *name);

private:
    // Index/vertex buffer data
    //std::vector<OpenGLMeshData> mesh_data_;
    //QSharedPointer<QGLShaderProgram> shader_;
    static OpenglMesh::Ptr create_mesh_from_raw_(
        GLVersion_Widget* gl,
        GLsizeiptr n_verts, void *raw_vertex_data,
        size_t sizeof_vertex,
        GLsizeiptr n_indices, void *raw_index_data,
        size_t sizeof_index,
        size_t sizeof_vertex_data,
        void* offsetof_vertex_pos,
        void* offsetof_vertex_normal,
        void* offsetof_vertex_data,
        const Vec3f& trans, const Vec3f& scale);
};


} // namespace bm
