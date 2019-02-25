/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QButtonGroup>

#include "draughts.h"

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT
    
public:
    enum TYPE { GAME, REPLAY };
    enum GAME_TYPE { SINGLE, HOTSEAT, SERVER, CLIENT };
    enum GAME_STATE { RUN, END };
    enum PLAYER_TYPE { HOST, GUEST, PC };

    explicit Game(QWidget *parent = 0);
    ~Game();

    void createBoard();
    void redrawBoard();
    void refreshRec();
    void getHint();

    void save(const QString &fileName);
    void load(const QString &fileName);

    void initToReplay() { draughts.initToReplay(); redrawBoard(); }
    void replayStep();
    void replayBack() { draughts.replayBack(); redrawBoard(); }
    uint getReplayNumCurTurn() { return draughts.getReplayNumCurTurn(); }
    uint getReplayNumEndTurn() { return draughts.getRecord()->size(); }

    TYPE getType() { return type; }
    void setType(TYPE type) { this->type = type; }

    GAME_TYPE getGameType() { return game_type; }
    void setGame(GAME_TYPE _game_type, PLAYER_TYPE _player_white);

    GAME_STATE getGameState() { return game_state; }

Q_SIGNALS:
    void replayEnded();

private slots:
    void checkUserClick(int index);

private:
    PLAYER_TYPE curPlayerType();
    void initPC();

    Ui::Game *ui;
    TYPE type;
    GAME_TYPE game_type;
    GAME_STATE game_state;
    QButtonGroup buttonGroup;
    Draughts draughts;

    PLAYER_TYPE player_white;
    PLAYER_TYPE player_black;

    S_Stroke stroke;
    bool stroke_from;
};

#endif // GAME_H
