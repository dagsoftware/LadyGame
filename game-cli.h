/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef GAMECLI_H
#define GAMECLI_H

#include "draughts.h"

class GameCLI
{
public:
    enum TYPE { GAME, REPLAY };
    enum GAME_TYPE { SINGLE, HOTSEAT, SERVER, CLIENT };
    enum GAME_STATE { RUN, END, QUIT };
    enum PLAYER_TYPE { HOST, GUEST, PC };

    GameCLI();

    void printBoard();
    void printHint();
    void printHistory();

    void printHelp();
    void prompt();

    void printHelpReplay();
    void promptReplay();

    void save(const QString &fileName);
    void load(const QString &fileName);

    void initToReplay() { draughts.initToReplay(); printBoard(); }
    void replayStep();
    void replayBack() { draughts.replayBack(); printBoard(); }
    uint getReplayNumCurTurn() { return draughts.getReplayNumCurTurn(); }
    uint getReplayNumEndTurn() { return draughts.getRecord()->size(); }

    TYPE getType() { return type; }
    void setType(TYPE type) { this->type = type; }

    GAME_TYPE getGameType() { return game_type; }
    GAME_STATE getGameState() { return game_state; }
    void setGame(GAME_TYPE _game_type, Stone::COLOUR my_color);

private:
    PLAYER_TYPE curPlayerType();
    void initPC();

    TYPE type;
    GAME_TYPE game_type;
    GAME_STATE game_state;
    Draughts draughts;

    PLAYER_TYPE player_white;
    PLAYER_TYPE player_black;
};

#endif // GAMECLI_H
