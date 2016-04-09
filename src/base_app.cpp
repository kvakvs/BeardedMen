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

#include "base_app.h"

void BaseApp::setShader(QSharedPointer<QGLShaderProgram> shader)
{
    shader_ = shader;
}

void BaseApp::initialize()
{
    shader_ = QSharedPointer<QGLShaderProgram>(new QGLShaderProgram);

    // This is basically a simple fallback vertex shader which does the most basic rendering possible.
    // PolyVox examples are able to provide their own shaders to demonstrate certain effects if desired.
    if (!shader_->addShaderFromSourceFile(QGLShader::Vertex, ":/shader/example.vert"))
    {
        std::cerr << shader_->log().toStdString() << std::endl;
        exit(EXIT_FAILURE);
    }

    // This is basically a simple fallback fragment shader which does the most basic rendering possible.
    // PolyVox examples are able to provide their own shaders to demonstrate certain effects if desired.
    if (!shader_->addShaderFromSourceFile(QGLShader::Fragment, ":/shader/example.frag"))
    {
        std::cerr << shader_->log().toStdString() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Bind the position semantic - this is defined in the vertex shader above.
    shader_->bindAttributeLocation("position", 0);

    // Bind the other semantics. Note that these don't actually exist in our example shader above! However, other
    // example shaders may choose to provide them and having the binding code here does not seem to cause any problems.
    shader_->bindAttributeLocation("normal", 1);
    shader_->bindAttributeLocation("material", 2);

    if (!shader_->link())
    {
        std::cerr << shader_->log().toStdString() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Now do any initialization for the specific example.
    initializeExample();
}

void BaseApp::renderOneFrame()
{
    // Our example framework only uses a single shader for the scene (for all meshes).
    shader_->bind();

    // These two matrices are constant for all meshes.
    shader_->setUniformValue("viewMatrix", viewMatrix());
    shader_->setUniformValue("projectionMatrix", projectionMatrix());

    // Iterate over each mesh which the user added to our list, and render it.
    for (OpenGLMeshData meshData : mesh_data_)
    {
        //Set up the model matrrix based on provided translation and scale.
        QMatrix4x4 modelMatrix;
        modelMatrix.translate(meshData.translation);
        modelMatrix.scale(meshData.scale);
        shader_->setUniformValue("modelMatrix", modelMatrix);

        // Bind the vertex array for the current mesh
        glBindVertexArray(meshData.vertexArrayObject);
        // Draw the mesh
        glDrawElements(GL_TRIANGLES, meshData.noOfIndices, meshData.indexType, 0);
        // Unbind the vertex array.
        glBindVertexArray(0);
    }

    // We're done with the shader for this frame.
    shader_->release();
}
