#include "dock.h"
#include "util/str.h"

#define bmKEY(T) "<font color='#66f'><u>" T "</></font>"

namespace bm {

CNCDock::CNCDock(QMainWindow *w): QDockWidget(w) {
    //this->setStyleSheet("QDockWidget { padding-left: -20px }");

    main_.panel = new QWidget(this);
    main_.panel->setFixedWidth(220);
    this->setWidget(main_.panel);
    main_.layout = new QFormLayout(main_.panel);
    //main_.layout->setContentsMargins(0, 0, 0, 0);

    keyframe_.frame = new QFrame(main_.panel);
    keyframe_.frame->setFrameShadow(QFrame::Raised);
    keyframe_.frame->setFrameShape(QFrame::StyledPanel);
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
    QString text = tr("(%1; %2) Depth: %3");
    text = text.arg(p.x()).arg(p.y()).arg(depth);
    this->setWindowTitle(text);
}

void CNCDock::reset_keyboard_panel() {
    auto l_widget = new QWidget(main_.panel);
    keyframe_.layout = new QVBoxLayout(l_widget);
    //keyframe_.layout->setContentsMargins(0, 0, 0, 0);
    delete keyframe_.stacked_layout->currentWidget();
    keyframe_.stacked_layout->addWidget(l_widget);
}

void CNCDock::SLOT_keyboard_fsm_changed(KeyFSM fsm_state)
{
    reset_keyboard_panel();
    switch(fsm_state) {
    case KeyFSM::Default:
        add_fsm_keys_default();
        break;
    case KeyFSM::Designations:
        add_fsm_keys_digging();
        break;
    }

}

QString CNCDock::highlight_keys(const QString& str0) {
    return util::re_replace(str0, "\\{(.+)\\}",
        [](auto match) -> QString {
            return QString("<font color='#f66'><u>") + match + "</></font>";
        });
}

void CNCDock::shortcut_label(const QString& str0)
{
    keyframe_.layout->addWidget(new QLabel(highlight_keys(str0)));
}

void CNCDock::breadcrumbs(const QString& crumbs) {
    auto l = new QLabel(QString("<b>") + crumbs + "</b>"
                        + highlight_keys(tr(" {Esc}↩")));
    l->setStyleSheet("background: #aaa;padding: 4px;");
    keyframe_.layout->addWidget(l);
}

void CNCDock::add_fsm_keys_default() {
    //shortcut_label("{B}uilding");
    shortcut_label(tr("{D}esignations"));
    shortcut_label(tr("{Arrows} move | {+} {-} depth"));
}

void CNCDock::add_fsm_keys_digging() {
    breadcrumbs(tr("Designations >"));
    shortcut_label(tr("{D} mine|{R}amp"));
    shortcut_label(tr("{M} mark"));
    //shortcut_label("stairs ({U}p {D}own {J}-both)");
}


} // ns bm
