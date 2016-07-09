#pragma once

//#include <QObject>
#include <QMainWindow>

#include "gfx/game_widget.h"
#include "ui/cnc_dock.h"
#include "ui/log_dock.h"

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
    BattlelogDock* log_dock_;

    void setup_event_connections() {
    }
};

} // ns bm
