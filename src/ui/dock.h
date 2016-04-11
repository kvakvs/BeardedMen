#pragma once

#include <QMainWindow>
#include <QObject>
#include <QFormLayout>
#include <QDockWidget>
#include <QLabel>

#include "game.h"

namespace bm {

class CNCDock: public QDockWidget {
    Q_OBJECT
private:
    QLabel *l_pos_;
    QLabel *l_depth_;

public:
    CNCDock(QMainWindow *w): QDockWidget("Command and Control", w) {
        auto panel = new QWidget(this);
        this->setWidget(panel);

        auto lay = new QFormLayout(panel);
        //panel->setLayout(lay);

        l_pos_ = new QLabel("(0; 0)", panel);
        lay->addRow("Position", l_pos_);
        l_depth_ = new QLabel("1", panel);
        lay->addRow("Depth", l_depth_);
    }

    void setup_event_connections(GameWidget *g) {
        QObject::connect(g, &GameWidget::SIG_cursor_changed,
                         this, &CNCDock::SLOT_cursor_changed);
    }

public slots:
    void SLOT_cursor_changed(const QPoint &p, int depth) {
        l_pos_->setText(QString("(%1; %2)").arg(p.x()).arg(p.y()));
        l_depth_->setText(QString("%1").arg(depth));
    }
};

} // ns bm
