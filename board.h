/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef BOARD_H
#define BOARD_H

#include "stone.h"

/**
 * @brief Třída hrací desky.
 */
class Board
{
    unsigned rows;
    unsigned cols;
    std::vector<Stone *> matrix;
public:
    enum DIRECTION {SV, JV, JZ, SZ, DirFirst=SV, DirLast=SZ};

    Board() {}

    /**
     * @brief Konstruktor, který zároveň inicializuje hrací desku na dané rozměry.
     * @param _rows Počet řádků.
     * @param _cols Počet sloupců.
     */
    Board(unsigned _rows, unsigned _cols) :
        rows(_rows), cols(_cols) { matrix.resize(rows*cols, NULL); }

    /**
     * @brief Inicializuje hrací desku na nové rozměry.
     * @param rows Počet řádků.
     * @param cols Počet sloupců.
     */
    void Init(const unsigned rows, const unsigned cols)
    {
        this->rows=rows; this->cols=cols; Init();
    }

    /**
     * @brief Vratí hrací desku do počátečního stavu. Deska bude poté čistá.
     */
    void Init()
    {
        matrix.clear(); matrix.resize(rows*cols, NULL);
    }

    //Stone *operator [] (uint pos) { return (matrix[pos]); }

    Stone *getField(const uint &index);
    Stone *getField(const S_Coord &coord);
    Stone *setField(const S_Coord &coord, Stone *src);

    /**
     * @brief Nastaví políčko se souřadnicemi @a coord na NULL, tedy odstraní z něj kámen.
     * @param coord Souřadnice na cílové políčko.
     *
     * @exception std::runtime_error Pokud jsou souřadnice mimo matici.
     */
    void clearField(const S_Coord &coord) { setField(coord, NULL); }
    void swapField(const S_Coord &from, const S_Coord &to);
};

#endif // BOARD_H
