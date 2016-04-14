#include "dock.h"
#include "util_str.h"

#define bmKEY(T) "<font color='#66f'><u>" T "</></font>"

namespace bm {

CNCDock::CNCDock(QMainWindow *w): QDockWidget(w) {
    main_.panel = new QWidget(this);
    main_.panel->setFixedWidth(200);
    this->setWidget(main_.panel);

    main_.layout = new QFormLayout(main_.panel);

    keyframe_.frame = new QFrame(main_.panel);
    keyframe_.frame->setGeometry(QRect(0, 0, 100, 100));
    keyframe_.stacked_layout = new QStackedLayout(keyframe_.frame);

    main_.layout->addWidget(keyframe_.frame);
    SLOT_keyboard_fsm_changed(KeyFSM::Default);
}

void CNCDock::setup_event_connections(GameWidget *g) {
    // Connect cursor movement to info update on panel
    QObject::connect(g, &GameWidget::SIG_cursor_changed,
                     this, &CNCDock::SLOT_cursor_changed);
    // Connect keyboard changes to displayed keys on panel
    QObject::connect(g, &GameWidget::SIG_keyboard_fsm_changed,
                     this, &CNCDock::SLOT_keyboard_fsm_changed);
}

void CNCDock::SLOT_cursor_changed(const QPoint &p, int depth) {
    // TODO: Move text to dock title
    QString text("(%1; %2) Depth: %3");
    text = text.arg(p.x()).arg(p.y()).arg(depth);
    this->setWindowTitle(text);
}

void CNCDock::SLOT_keyboard_fsm_changed(KeyFSM fsm_state)
{
    auto l_widget = new QWidget(main_.panel);
    keyframe_.layout = new QVBoxLayout(l_widget);
    keyframe_.layout->setContentsMargins(0, 0, 0, 0);
    delete keyframe_.stacked_layout->currentWidget();
    keyframe_.stacked_layout->addWidget(l_widget);

    switch(fsm_state) {
    case KeyFSM::Default:
        add_fsm_keys_default();
        break;
    case KeyFSM::Digging:
        add_fsm_keys_digging();
        break;
    }
}

void CNCDock::shortcut_label(const char *str0)
{
    QString str = util::re_replace(str0, "\\{(.)\\}",
        [](auto match) -> QString {
            return QString("<font color='#f66'><u>") + match + "</></font>";
        });

    keyframe_.layout->addWidget(new QLabel(str));
}

void CNCDock::add_fsm_keys_default() {
    shortcut_label("{B}uilding, {D}ig, set {O}rders");
}

void CNCDock::add_fsm_keys_digging() {
    shortcut_label("{M}ining, {C}hannel, {R}amp");
    shortcut_label("stairs ({U}p, {D}own, {J}-both)");
}


} // ns bm
