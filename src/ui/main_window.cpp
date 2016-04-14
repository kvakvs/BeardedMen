#include "main_window.h"

namespace bm {

GameMainWindow::GameMainWindow(): QMainWindow(nullptr) {
    setGeometry(0, 0, 800, 600);

    gl_widget_ = new bm::GameWidget(this);
    gl_widget_->setGeometry(geometry());
    setCentralWidget(gl_widget_);
    setup_event_connections();

    cnc_dock_ = new CNCDock(this);
    cnc_dock_->setFeatures(0 //QDockWidget::DockWidgetMovable
                           //| QDockWidget::DockWidgetFloatable
                           //| QDockWidget::DockWidgetVerticalTitleBar
                           );
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    cnc_dock_->setup_event_connections(gl_widget_);
    addDockWidget(Qt::RightDockWidgetArea, cnc_dock_);
}

void GameMainWindow::keyPressEvent(QKeyEvent *event) {
    gl_widget_->keyPressEvent(event);
    if (event->isAccepted() == false) {
        QMainWindow::keyPressEvent(event);
    }
}

void GameMainWindow::keyReleaseEvent(QKeyEvent *event) {
    gl_widget_->keyReleaseEvent(event);
    if (event->isAccepted() == false) {
        QMainWindow::keyReleaseEvent(event);
    }
}


} // ns bm
