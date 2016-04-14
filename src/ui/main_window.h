#pragma once

//#include <QObject>
#include <QMainWindow>

#include "gfx/game_widget.h"
#include "ui/dock.h"

namespace bm {

class GameMainWindow: public QMainWindow {
    Q_OBJECT
public:
    GameMainWindow();
    virtual void keyPressEvent( QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

private:
    bm::GameWidget *gl_widget_;
    CNCDock* cnc_dock_;

    void setup_event_connections() {
    }
};

} // ns bm
