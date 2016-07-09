#pragma once

#include "PolyVox/Mesh.h"

//#include <QOpenGLBuffer>
//#include <QOpenGLVertexArrayObject>
#include <OgreRenderOperation.h>

#include <cmath>

//#include "gfx/gl_version.h"
#include "util/vec.h"
#include "util/ogre_types.h"
//#include "gfx/model.h"

namespace pv = PolyVox;
namespace bm {

//ShaderPtr load_shader(const char *name);

namespace mesh {

template <typename MeshType>
uniq::ManualObject create_mesh_from_pv(Ogre::SceneManager *scenem,
                                       const char *name,
                                       const MeshType &pvmesh)
{
    uniq::ManualObject m(scenem->createManualObject(name));
    m->estimateVertexCount(pvmesh.getNoOfVertices());
    m->estimateIndexCount(pvmesh.getNoOfIndices());
    m->begin("colored_cubes", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    return m;
}


#if 0
namespace impl {
    bm::ManualObject create_manual_object_impl(
        GLsizeiptr n_verts, void *raw_vertex_data,
        size_t sizeof_vertex,
        GLsizeiptr n_indices, void *raw_index_data,
        size_t sizeof_index,
        size_t sizeof_vertex_data,
        void* offsetof_vertex_pos,
        void* offsetof_vertex_normal,
        void* offsetof_vertex_data,
        const Vec3f& trans, const Vec3f& scale);
} // ns bm::mesh::impl

// Convert a PolyVox mesh to OpenGL index/vertex buffers.
template <typename MeshType>
uniq::ManualObject create_manual_object_from_raw(
        GLVersion_Widget *gl,
        const MeshType &mesh,
        const Vec3f &trans = Vec3f(0, 0, 0),
        const Vec3f &scale = Vec3f(1.0f, 1.0f, 1.0f))
{
    const auto sizeof_vertex = sizeof(typename MeshType::VertexType);
    void *raw_v_data = const_cast<void*>(
                        static_cast<const void*>(mesh.getRawVertexData()));
    void *raw_i_data = const_cast<void*>(
                        static_cast<const void*>(mesh.getRawIndexData()));
    return impl::create_manual_object_impl(
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

OpenglMesh::Ptr create_ramp(GLVersion_Widget* gl, ModelId i);
#endif

} // ns bm::mesh
} // ns bm
