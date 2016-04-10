#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>

#include "ogl_widget.h"

namespace nrdf {

using GLVersion_Funs = QOpenGLFunctions_3_2_Core;
using GLVersion_Widget = MyGLWidget<GLVersion_Funs>;

} // nrdf

