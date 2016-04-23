#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_0>

#include "gfx/ogl_widget.h"

namespace bm {

//constexpr auto GL_PROFILE = QGLFormat::CoreProfile;
constexpr auto GL_PROFILE = QGLFormat::CompatibilityProfile;
constexpr int GL_MAJOR = 3;
constexpr int GL_MINOR = 0;
using GLVersion_Funs = QOpenGLFunctions_3_0;
using GLVersion_Widget = MyGLWidget<GLVersion_Funs>;

} // namespace bm

