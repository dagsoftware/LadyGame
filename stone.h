/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef STONE_H
#define STONE_H

#include <algorithm>
#include <vector>
#include "stroke.h"

/**
 * @brief The Stone class
 */
class Stone
{
public:
    typedef std::vector<S_Stroke> Vec_Stroke;
    enum COLOUR { WHITE, BLACK };
    enum TYPE { PAWN, DAME };

    Stone() : type(PAWN), numDeathTurn(0), numUpgTurn(0) {}
    Stone(const COLOUR _colour) : colour(_colour), type(PAWN), numDeathTurn(0), numUpgTurn(0) {}

    COLOUR getColour() { return colour; }
    void setColour(const COLOUR &colour) { this->colour = colour; }
    TYPE getType() { return type; }
    void setType(const TYPE &type) { this->type = type; }

    Vec_Stroke *getStrokes() { return &strokes; }
    bool correctStroke(const S_Stroke &stroke);
    bool findStroke(const S_Stroke &temp, S_Stroke &found);
    void setStroke(const S_Stroke &stroke) { strokes.push_back(stroke); }
    void rmNonStrikeStrokes();
    void clearStrokes() { strokes.clear(); }

    void setDeathTurn(const uint &numDeathTurn) { this->numDeathTurn = numDeathTurn; }
    uint getDeathTurn() { return numDeathTurn; }

    void setUpgTurn(const uint &numUpgTurn) { this->numUpgTurn = numUpgTurn; }
    uint getUpgTurn() { return numUpgTurn; }

private:
    COLOUR colour;
    TYPE type;          // Typ kamene.
    Vec_Stroke strokes; // Možné tahy.
    unsigned numDeathTurn; // Tah ve kterém byl zničen.
    unsigned numUpgTurn;   // Tah ve kterém byl povýšen.
};

#endif // STONE_H
