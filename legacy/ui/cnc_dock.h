#pragma once

#include <QMainWindow>
#include <QObject>
#include <QDockWidget>
#include <QLabel>

#include <QFormLayout>
#include <QStackedLayout>

namespace bm {

class GameWidget;
enum class KeyFSM: int;

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
    static QString highlight_keys(const QString& str0);
    void shortcut_label(const QString& s);
    void breadcrumbs(const QString& crumbs);

    void add_pos_labels();

    void add_fsm_keys_default();
    void add_fsm_keys_digging();
    void reset_keyboard_panel();
};

} // ns bm
