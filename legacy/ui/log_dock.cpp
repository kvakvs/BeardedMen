#include "log_dock.h"
#include "util/str.h"
#include "gfx/game_widget.h"

#include <QFontMetrics>

namespace bm {

BattlelogDock::BattlelogDock(QMainWindow *w): QDockWidget(w) {
    this->setWindowTitle(tr("Log"));

    main_.panel = new QPlainTextEdit(this);
    main_.panel->setReadOnly(true);
    main_.panel->setEnabled(false);

    QFontMetrics fm(this->font());
    main_.panel->setFixedHeight(fm.height() * 4);

    this->setWidget(main_.panel);
    main_.layout = new QFormLayout(main_.panel);
}

void BattlelogDock::setup_event_connections(GameWidget *g) {
    // Connect cursor movement to info update on panel
    QObject::connect(g, &GameWidget::SIG_battle_log,
                     this, &BattlelogDock::SLOT_battle_log);
}

void BattlelogDock::SLOT_battle_log(const QString& msg) {
    add_message(msg);
}

void BattlelogDock::add_message(const QString& msg) {
    main_.panel->insertPlainText("\n");
    main_.panel->insertPlainText(msg);
}


} // ns bm
