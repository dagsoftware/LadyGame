/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef DAMA_H
#define DAMA_H

#include <QTimer>
#include <QTimeEdit>
#include <QLabel>
#include <QSpinBox>
#include <QMainWindow>

#include "game.h"

namespace Ui {
class Dama;
}

class Dama : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Dama(QWidget *parent = 0);
    ~Dama();
    
private slots:
    void on_actionNew_triggered();
    void dialogNewIsFinished(Game::GAME_TYPE game_type, Game::PLAYER_TYPE player_white);
    void on_actionLoad_triggered();
    void dialogLoadIsFinished(Game::GAME_TYPE game_type, Game::PLAYER_TYPE player_white);
    void on_actionSave_triggered();
    void on_actionAbout_game_triggered();
    void on_actionAbort_triggered();
    void closeTab(int tab_id);

    void on_actionLoadReplay_triggered();

    void on_tabWidget_currentChanged(QWidget *arg1);

    void on_actionBack_triggered();

    void on_actionNext_triggered();
    void timeoutReplayTimer();

    void on_actionHint_triggered();

    void on_actionPlay_auto_triggered();

    void on_actionPause_triggered();

    void on_actionBack_to_origin_triggered();

private:
    void setReplayCurTurn();

    Ui::Dama *ui;

    QTimer *replay_timer;
    QTimeEdit *replay_delay;
    QSpinBox *replay_upto;
    QLabel *replay_cur_turn;
};

#endif // DAMA_H
