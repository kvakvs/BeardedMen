#include "gfx/ogl_widget.h"

#include <QMouseEvent>
#include <QTimer>
#include <cmath>

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// Protected functions
////////////////////////////////////////////////////////////////////////////////
template <typename QOpenGLFunctionsType>
MyGLWidget<QOpenGLFunctionsType>::MyGLWidget(QWidget *parent)
:QGLWidget(parent)
{
}

template <typename QOpenGLFunctionsType>
const QMatrix4x4& MyGLWidget<QOpenGLFunctionsType>::get_view_matrix() const
{
    return view_matrix_;
}

template <typename QOpenGLFunctionsType>
const QMatrix4x4& MyGLWidget<QOpenGLFunctionsType>::get_projection_matrix() const
{
    return proj_matrix_;
}

template <typename QOpenGLFunctionsType>
void MyGLWidget<QOpenGLFunctionsType>::set_camera_transform(
        QVector3D position, float pitch, float yaw)
{
    cam_pos_ = position;
//    cam_yaw_ = yaw;
//    cam_pitch_ = pitch;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////
template <typename QOpenGLFunctionsType>
void MyGLWidget<QOpenGLFunctionsType>::initializeGL()
{
    if (!this->initializeOpenGLFunctions())
    {
            std::cerr << "Could not initialize OpenGL functions" << std::endl;
            exit(EXIT_FAILURE);
    }

    //Print out some information about the OpenGL implementation.
    std::cout << "OpenGL Implementation Details:" << std::endl;
    if (this->glGetString(GL_VENDOR))
        std::cout << "\tGL_VENDOR: "
                  << this->glGetString(GL_VENDOR) << std::endl;
    if (this->glGetString(GL_RENDERER))
        std::cout << "\tGL_RENDERER: "
                  << this->glGetString(GL_RENDERER) << std::endl;
    if (this->glGetString(GL_VERSION))
        std::cout << "\tGL_VERSION: "
                  << this->glGetString(GL_VERSION) << std::endl;
    if (this->glGetString(GL_SHADING_LANGUAGE_VERSION))
        std::cout << "\tGL_SHADING_LANGUAGE_VERSION: "
                  << this->glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    //Set up the clear colour
    this->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    this->glClearDepth(1.0f);

    this->glEnable(GL_DEPTH_TEST);
    this->glDepthMask(GL_TRUE);
    this->glDepthFunc(GL_LEQUAL);
    this->glDepthRange(0.0, 1.0);

    initialize();

    // Start a timer to drive the main rendering loop.
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(0);

    elapsed_timer_.start();
}

template <typename QOpenGLFunctionsType>
void MyGLWidget<QOpenGLFunctionsType>::resizeGL(int w, int h)
{
	//Setup the viewport
	this->glViewport(0, 0, w, h);

	auto aspectRatio = w / (float)h;
	float zNear = 1.0;
	float zFar = 1000.0;

    proj_matrix_.setToIdentity();
    proj_matrix_.perspective(cam_fov_, aspectRatio, zNear, zFar);
    proj_matrix_.scale(QVector3D(-1,1,1));
}

template <typename QOpenGLFunctionsType>
void MyGLWidget<QOpenGLFunctionsType>::paintGL()
{
    // Direction : Spherical coordinates to Cartesian coordinates conversion
    auto cam_forward = get_cam_forward();

    // Right vector
    QVector3D cam_right(1.0f, 0.f, 0.f);

    // Up vector
    QVector3D cam_up = get_camera_up(cam_right, cam_forward);

    // Get the elapsed time since last frame and convert to seconds.
    float deltaTime = elapsed_timer_.restart() / 1000.0f;

    // Move forward
    if ((pressed_keys_.contains(Qt::Key_Up)) || (pressed_keys_.contains(Qt::Key_W)))
    {
        cam_pos_ += cam_forward * deltaTime * cam_move_speed;
    }
    // Move backward
    if ((pressed_keys_.contains(Qt::Key_Down)) || (pressed_keys_.contains(Qt::Key_S)))
    {
        cam_pos_ -= cam_forward * deltaTime * cam_move_speed;
    }
    // Strafe right
    if ((pressed_keys_.contains(Qt::Key_Right)) || (pressed_keys_.contains(Qt::Key_D)))
    {
        cam_pos_ += cam_right * deltaTime * cam_move_speed;
    }
    // Strafe left
    if ((pressed_keys_.contains(Qt::Key_Left)) || (pressed_keys_.contains(Qt::Key_A)))
    {
        cam_pos_ -= cam_right * deltaTime * cam_move_speed;
    }

    view_matrix_.setToIdentity();

    auto cam_focus = get_camera_focus(cam_forward);
    view_matrix_.lookAt(
        cam_pos_,   // Camera is here
        cam_focus,  // and looks here : at the same position, plus "direction"
        cam_up      // Head is up (set to 0,-1,0 to look upside-down)
        );

    //Clear the screen
    this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_frame();

    // Check for errors.
    GLenum errCode = this->glGetError();
    if (errCode != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << errCode << std::endl;
    }
}

template <typename QOpenGLFunctionsType>
void MyGLWidget<QOpenGLFunctionsType>::mousePressEvent(QMouseEvent* event)
{
    // Initialise these variables which will be used when the mouse actually moves.
    mouse_pos_ = event->pos();
    last_frame_mouse_pos_ = mouse_pos_;
}

template <typename QOpenGLFunctionsType>
void MyGLWidget<QOpenGLFunctionsType>::mouseMoveEvent(QMouseEvent* event)
{
    // Update the x and y rotations based on the mouse movement.
    mouse_pos_ = event->pos();
//    QPoint diff = mouse_pos_ - last_frame_mouse_pos_;
//    cam_yaw_ -= diff.x() * cam_rotate_speed_;
//    cam_pitch_ -= diff.y() * cam_rotate_speed_;
    last_frame_mouse_pos_ = mouse_pos_;
}

template <typename QOpenGLFunctionsType>
void MyGLWidget<QOpenGLFunctionsType>::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		close();
	}

    pressed_keys_.append(event->key());
}

template <typename QOpenGLFunctionsType>
void MyGLWidget<QOpenGLFunctionsType>::keyReleaseEvent(QKeyEvent* event)
{
    pressed_keys_.removeAll(event->key());
}
