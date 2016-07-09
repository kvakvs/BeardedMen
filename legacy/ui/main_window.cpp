#include "main_window.h"
#include <QApplication>
#include <QDesktopWidget>

namespace bm {

GameMainWindow::GameMainWindow(): QMainWindow(nullptr) {
    QRect desktop = QApplication::desktop()->screenGeometry();
    setGeometry(desktop.width() / 4, desktop.height() / 4,
                desktop.width() / 2, desktop.height() / 2);

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

    /*QFile file(":/stylesheet.qss");
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        auto style = file.readAll();
        this->setStyleSheet(style);
        file.close();
    }*/
\
    // Battle Log
    log_dock_ = new BattlelogDock(this);
    log_dock_->setFeatures(QDockWidget::DockWidgetVerticalTitleBar);
    log_dock_->setAllowedAreas(Qt::BottomDockWidgetArea
                               | Qt::TopDockWidgetArea);
    log_dock_->setup_event_connections(gl_widget_);
    addDockWidget(Qt::BottomDockWidgetArea, log_dock_);
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
