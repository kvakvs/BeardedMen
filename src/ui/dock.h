#pragma once

#include <QMainWindow>
#include <QObject>
#include <QDockWidget>
#include <QLabel>

#include <QFormLayout>
#include <QStackedLayout>

#include "gfx/game_widget.h"

namespace bm {

class CNCDock: public QDockWidget {
    Q_OBJECT
private:
    struct {
        QWidget *panel;
        QFormLayout *layout;
    } main_;

    // Keyboard hints panel
    struct {
        QFrame *frame;
        QStackedLayout *stacked_layout;
        QVBoxLayout *layout;
    } keyframe_;

public:
    CNCDock(QMainWindow *w);

    void setup_event_connections(GameWidget *g);

public slots:
    void SLOT_cursor_changed(const QPoint &p, int depth);
    void SLOT_keyboard_fsm_changed(bm::KeyFSM fsm);

private:
    // Process string s, replacing {K} with <font color...<u>K</u></font>
    // and make a QLabel with this. Adds it to keyframe_.layout
    void shortcut_label(const char *s);
    void add_pos_labels();
    void add_fsm_keys_default();
    void add_fsm_keys_digging();
};

} // ns bm
