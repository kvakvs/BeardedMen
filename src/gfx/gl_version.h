#pragma once

#include <QOpenGLWidget>

#define DESIRED_GL 33

#if DESIRED_GL == 33
#include <QOpenGLFunctions_3_3_Core>
#endif //33

#include "gfx/ogl_widget.h"

namespace bm {

constexpr int GL_MAJOR = DESIRED_GL / 10;
constexpr int GL_MINOR = DESIRED_GL % 10;

#if DESIRED_GL == 33
using GLVersion_Funs = QOpenGLFunctions_3_3_Core;
constexpr auto GL_PROFILE = QGLFormat::CoreProfile;
#else
constexpr auto GL_PROFILE = QGLFormat::CompatibilityProfile;
#endif

using GLVersion_Widget = MyGLWidget<GLVersion_Funs>;

} // namespace bm

