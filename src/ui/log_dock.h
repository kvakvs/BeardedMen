#pragma once

#include <QMainWindow>
#include <QObject>
#include <QDockWidget>
//#include <QLabel>
#include <QPlainTextEdit>
#include <QFormLayout>
//#include <QStackedLayout>

namespace bm {

class GameWidget;

class BattlelogDock: public QDockWidget {
    Q_OBJECT
private:
    struct {
        QPlainTextEdit *panel;
        QFormLayout *layout;
    } main_;

public:
    BattlelogDock(QMainWindow *w);

    void setup_event_connections(GameWidget *g);
    void add_message(const QString &msg);

public slots:
    void SLOT_battle_log(const QString& msg);

private:
};

} // ns bm
