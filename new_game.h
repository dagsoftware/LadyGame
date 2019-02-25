/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef NEW_GAME_H
#define NEW_GAME_H

#include <QDialog>
#include "game.h"

namespace Ui {
class NewGame;
}

class NewGame : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewGame(QWidget *parent = 0);
    ~NewGame();

Q_SIGNALS:
    void accepted(Game::GAME_TYPE game_type, Game::PLAYER_TYPE player_white);

private slots:
    void localTypeChanged();

    void on_tabWidget_currentChanged(int index);

    void on_buttonBox_accepted();

private:
    Ui::NewGame *ui;
};

#endif // NEW_GAME_H
