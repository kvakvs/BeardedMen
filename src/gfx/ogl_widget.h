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

#pragma once

#include <QElapsedTimer>
#include <QGLWidget>
#include <QMatrix4x4>

// This is a very basic class for getting an OpenGL example up and running with
// Qt5. It simply displays an OpenGL widget and implements an FPS-style camera
// as well as other very basic functionality. User code can derive from this
// and override the provided virtual functions to implement functionality.
// The class is templatized so users can specify the OpenGL version via the
// appropriate QOpenGLFunctions.
template <typename QOpenGLFunctionsType>
class MyGLWidget : public QGLWidget, public QOpenGLFunctionsType {
protected:
    // Protected constructor because this widget should not be created directly
    // - it should only be subclassed.
    MyGLWidget(QWidget* parent);

    // Derived classes should override these to provide functionality.
    virtual void initialize() {}
    virtual void render_frame() {}

public:
    // Getters for properties defined by this widget.
    const QMatrix4x4& viewMatrix();
    const QMatrix4x4& projectionMatrix();

    // Setters for properties defined by this widget.
    void setCameraTransform(QVector3D position, float pitch, float yaw);

    // Mouse handling
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

    // Keyboard handling
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

protected:
    // Qt OpenGL functions
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    // Matrices
    QMatrix4x4 view_matrix_;
    QMatrix4x4 proj_matrix_;

    // Mouse data
    QPoint last_frame_mouse_pos_;
    QPoint mouse_pos_;

    // Keyboard data
    QList<int> pressed_keys_;

    // For input handling and movement
    float cam_move_speed = 50.0f;
    float cam_rotate_speed_ = 0.005f;

    // Camera properties
    QVector3D cam_pos_ = QVector3D(0, 0, -100);
    float cam_yaw_ = 0.0f;
    float cam_pitch_ = 0.0f;
    float cam_fov_ = 60.0f;

    QElapsedTimer elapsed_timer_;

    QVector3D get_cam_forward() const {
        return QVector3D(
                    std::cos(cam_pitch_) * std::sin(cam_yaw_),
                    std::sin(cam_pitch_),
                    std::cos(cam_pitch_) * std::cos(cam_yaw_)
                    );
    }
};

#include "ogl_widget.inl"

