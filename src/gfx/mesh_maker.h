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
Ogre::ManualObject *create_mesh_from_pv(Ogre::SceneManager *scenem,
                                        const char *name,
                                        const MeshType &pvmesh)
{
    Ogre::ManualObject *m = scenem->createManualObject(name);
    m->estimateVertexCount(pvmesh.getNoOfVertices());
    m->estimateIndexCount(pvmesh.getNoOfIndices());

    m->begin("Voxel", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    uint vcount = pvmesh.getNoOfVertices();
    Ogre::ColourValue col;

    for (uint v = 0; v < vcount; ++v) {
        auto vertex = pvmesh.getVertex(v);

        // begin new vertex
        m->position(vertex.position.getX(),
                    vertex.position.getY(),
                    vertex.position.getZ());
        m->normal(vertex.normal.getX(),
                  vertex.normal.getY(),
                  vertex.normal.getZ());
        col.setAsBGRA(vertex.data.getMaterial() << 8 | vertex.data.getDensity());
        m->colour(col);
    }

    uint tcount = pvmesh.getNoOfIndices();
    for (uint t = 0; t < tcount; t += 3) {
        m->triangle(pvmesh.getIndex(t),
                    pvmesh.getIndex(t + 1),
                    pvmesh.getIndex(t + 2));
    }
    m->end();

    printf("> created qb %u vert %u tris\n", vcount, tcount);
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
