#pragma once

//#include <QObject>
#include <QMainWindow>

#include "game.h"
#include "ui/dock.h"

namespace bm {

class GameMainWindow: public QMainWindow {
    Q_OBJECT
public:
    GameMainWindow(): QMainWindow(nullptr) {
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
        cnc_dock_->resize(200, 400);
        cnc_dock_->setup_event_connections(gl_widget_);
        addDockWidget(Qt::RightDockWidgetArea, cnc_dock_);
    }
    virtual void keyPressEvent( QKeyEvent* event) override {
        gl_widget_->keyPressEvent(event);
        if (event->isAccepted() == false) {
            QMainWindow::keyPressEvent(event);
        }
    }
    virtual void keyReleaseEvent(QKeyEvent* event) override {
        gl_widget_->keyReleaseEvent(event);
        if (event->isAccepted() == false) {
            QMainWindow::keyReleaseEvent(event);
        }
    }

private:
    bm::GameWidget *gl_widget_;
    CNCDock* cnc_dock_;

    void setup_event_connections() {
    }
};

} // ns bm
