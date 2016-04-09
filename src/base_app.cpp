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
#include <QOpenGLFunctions_3_2_Core>

namespace nrdf {

void BaseWidget::initialize() {
    // Now do any initialization for the specific example.
    initializeExample();
}

void BaseWidget::renderOneFrame()
{
}

BaseWidget::ShaderPtr BaseWidget::load_shader(const char *name)
{
    auto shad = QSharedPointer<QGLShaderProgram>(new QGLShaderProgram);
    std::string v_name = std::string(":/shader/") + name + ".vert";
    std::string f_name = std::string(":/shader/") + name + ".frag";

    if (!shad->addShaderFromSourceFile(QGLShader::Vertex,
                                       v_name.c_str())) {
        std::cerr << shad->log().toStdString() << std::endl;
        exit(EXIT_FAILURE);
    }
    if (!shad->addShaderFromSourceFile(QGLShader::Fragment,
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

    if (!shad->link()) {
        std::cerr << shad->log().toStdString() << std::endl;
        exit(EXIT_FAILURE);
    }
    return shad;
}

} // ns nrdf
