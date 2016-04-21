/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 David Williams and Matthew Williams

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#include "gfx/loader.h"
#include <QOpenGLFunctions_3_2_Core>

namespace bm {

OpenglMesh::Ptr Loader::create_mesh_from_raw_(
        GLVersion_Widget* gl,
        GLsizeiptr n_verts, void *raw_vertex_data, size_t sizeof_vertex,
        GLsizeiptr n_indices, void *raw_index_data, size_t sizeof_index,
        size_t sizeof_vertex_data,
        void *offsetof_vertex_pos,
        void *offsetof_vertex_normal,
        void *offsetof_vertex_data,
        const Vec3f &trans, const Vec3f &scale)
{
    // Mesh is an opengl resource, so it needs to have access to gl context
    // which is privately managed by this widget
    OpenglMesh::Ptr result = std::make_shared<OpenglMesh>(gl);

    // Create the VAO for the mesh
    gl->glGenVertexArrays(1, &(result->vert_array_));
    gl->glBindVertexArray(result->vert_array_);

    // The GL_ARRAY_BUFFER will contain the list of vertex positions
    gl->glGenBuffers(1, &(result->vert_buf_));
    gl->glBindBuffer(GL_ARRAY_BUFFER, result->vert_buf_);
    gl->glBufferData(GL_ARRAY_BUFFER, n_verts * sizeof_vertex,
                     raw_vertex_data, GL_STATIC_DRAW);

    // and GL_ELEMENT_ARRAY_BUFFER will contain the indices
    gl->glGenBuffers(1, &(result->indx_buf_));
    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->indx_buf_);
    gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof_index,
                     raw_index_data, GL_STATIC_DRAW);

    // Every surface extractor outputs valid positions for the vertices, so
    // tell OpenGL how these are laid out
    gl->glEnableVertexAttribArray(0);  // Attrib '0' is the vertex positions
    gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof_vertex,
                              offsetof_vertex_pos);
    // take the first 3 floats from every
    //      sizeof(decltype(vecVertices)::value_type)

    // Some surface extractors also generate normals, so tell OpenGL how
    // these are laid out. If a surface extractor does not generate normals
    // then nonsense values are written into the buffer here and sghould be
    // ignored by the shader. This is mostly just to simplify this example
    // code - in a real application you will know whether your chosen surface
    // extractor generates normals and can skip uploading them if not.
    gl->glEnableVertexAttribArray(1);  // Attrib '1' is the vertex normals.
    gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof_vertex,
                              offsetof_vertex_normal);

    // Finally a surface extractor will probably output additional data.
    // This is highly application dependant. For this example code we're
    // just uploading it as a set of bytes which we can read individually,
    // but real code will want to do something specialised here.
    gl->glEnableVertexAttribArray(2);  // Shader attribute '2'
    GLint size = (std::min)(sizeof_vertex_data, size_t(4));
    // Can't upload more that 4 components (vec4 is GLSL's biggest type)
    gl->glVertexAttribIPointer(2, size, GL_UNSIGNED_BYTE, sizeof_vertex,
                               offsetof_vertex_data);

    // We're done uploading and can now unbind.
    gl->glBindVertexArray(0);

    // A few additional properties can be copied across for use during
    // rendering.
    result->indx_count_ = n_indices;
    result->translation_ = QVector3D(trans.getX(), trans.getY(),
                                     trans.getZ());
    result->scale_ = QVector3D(scale.getX(), scale.getY(),
                               scale.getZ());

    // Set 16 or 32-bit index buffer size.
    result->indx_type_ = (sizeof_index == 2) ? GL_UNSIGNED_SHORT
                                             : GL_UNSIGNED_INT;
    return result;
}

ShaderPtr Loader::load_shader(const char *name)
{
    auto shad = QSharedPointer<QGLShaderProgram>(new QGLShaderProgram);
    std::string v_name = std::string(":/shader/") + name + ".vert";
    std::string f_name = std::string(":/shader/") + name + ".frag";

    if (not shad->addShaderFromSourceFile(QGLShader::Vertex,
                                       v_name.c_str())) {
        std::cerr << shad->log().toStdString() << std::endl;
        exit(EXIT_FAILURE);
    }
    if (not shad->addShaderFromSourceFile(QGLShader::Fragment,
                                       f_name.c_str())) {
        std::cerr << shad->log().toStdString() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Bind the position semantic - this is defined in the vertex shader above.
    shad->bindAttributeLocation("position", 0);

    // Bind the other semantics. Note that these don't actually exist in our
    // example shader above! However, other
    // example shaders may choose to provide them and having the binding code
    // here does not seem to cause any problems.
    shad->bindAttributeLocation("normal", 1);
    shad->bindAttributeLocation("material", 2);

    if (not shad->link()) {
        std::cerr << shad->log().toStdString() << std::endl;
        exit(EXIT_FAILURE);
    }
    return shad;
}

} // namespace bm
