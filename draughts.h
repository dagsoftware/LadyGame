/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef DRAUGHTS_H
#define DRAUGHTS_H

#include <algorithm>
#include <QString>
#include <QXmlStreamReader>

#include "board.h"

/**
 * @brief Třída hráče.
 */
class Player
{
public:
    typedef Stone::COLOUR COLOUR;
    std::vector<Stone> stones;
private:
    COLOUR colour;
    QString name;
    unsigned num_stones;
public:
    Player() {}
    Player(const QString name, const COLOUR colour) { this->name = name; this->colour = colour; }

    /** Vrací barvu hráče. */
    COLOUR getColour() const { return colour; }

    /** Nastaví barvu hráče podle @a colour. */
    void setColour(const COLOUR &colour) { this->colour = colour; }


    /** Vrací jméno hráče. */
    QString &getName() { return name; }

    /** Nastaví jméno hráče podle @a name. */
    void setName(const QString &name) { this->name = name; }


    /** Vrací počet kamenů hráče. */
    unsigned getNumStones() const { return num_stones; }

    /** Nastaví počet kamenů hráče podle @a num. */
    void setNumStones(const unsigned num) { num_stones = num; }
};

/**
 * @brief Třída hry dáma.
 */
class Draughts
{
    Board checkerboard;  // Šachovnice.
    Player player_white; // Bílý hráč.
    Player player_black; // Černý hráč.
    Stone::COLOUR draught;        // Která barava je na tahu?
    std::vector<S_Stroke> record; // Záznam tahů.
    std::vector<Stone *> possible; // Kameny, se kterými je možno táhnout
    unsigned replay_cur_turn; // Značí aktuální tah při přehrávání.
    const unsigned size;      // Velikost šachovnice.

    bool lookAtDirPawn(const S_Coord &coord, const Board::DIRECTION &dir, S_Stroke &stroke);
    bool lookAtDirDame(const S_Coord &coord, const Board::DIRECTION &dir);
    bool setCorrectStroke(const S_Coord &coord);
    bool getStrikeCoord(const S_Stroke &stroke, S_Coord &strike);

    bool xmlStrokeParse(QXmlStreamReader &xml);
public:
    Draughts() : size(8) { init(); } // Velikost šachovnice 8x8.
    void init();
    void findCorrectStrokes();

    void initPlayers();
    void renamePlayers(const QString &white, const QString &black);
    void playerMoveStone(const S_Stroke &stroke);
    void pcMoveStone() { S_Stroke stroke; if (!getHint(stroke)) return; playerMoveStone(stroke); }

    void moveStone(const S_Stroke &stroke);

    void initToReplay();
    void replayStep();
    void replayBack();
    uint getReplayNumCurTurn() { return replay_cur_turn; }
    std::vector<S_Stroke> *getRecord() { return &record; }

    void saveRecord(std::ostream &out_stream);
    void loadRecord(std::istream &in_stream);

    void saveRecordXML(const QString &filename);
    void loadRecordXML(const QString &filename);

    const Board &getCheckerboard() { return checkerboard; }
    const Player &getWhitePlayer() { return player_white; }
    const Player &getBlackPlayer() { return player_black; }
    const Player &getCurPlayer() { if ( draught == Stone::WHITE ) return player_white; else return player_black; }
    Stone::COLOUR whoHaveTurn() { return draught; }
    bool getHint(S_Stroke &hint);
};

#endif // DRAUGHTS_H
