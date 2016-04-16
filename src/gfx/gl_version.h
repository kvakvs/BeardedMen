#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_0>

#include "gfx/ogl_widget.h"

namespace bm {

using GLVersion_Funs = QOpenGLFunctions_3_0;
using GLVersion_Widget = MyGLWidget<GLVersion_Funs>;

} // namespace bm

