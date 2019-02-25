/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include <stdexcept>

#include "board.h"

/**
 * @brief Vrátí ukazatel na kámen ležící na políčku @a index.
 * @param index Index do vektoru představujícího matici.
 * @return Vrací ukazatel na kámen nebo NULL.
 *
 * @exception std::runtime_error Pokud je index mimo matici.
 */
Stone *Board::getField(const uint &index)
{
    if ( index >= matrix.size() ) {
        throw std::runtime_error("Spatne zadane souradnice.");
    }
    return matrix[index];
}

/**
 * @brief Vrátí ukazatel na kámen ležící na políčku se souřadnicemi @a coord.
 * Přetížení Board::getField.
 *
 * @param coord Souřadnice na zdrojové políčko.
 * @return Vrací ukazatel na kámen nebo NULL.
 *
 * @exception std::runtime_error Pokud jsou souřadnice mimo matici.
 */
Stone *Board::getField(const S_Coord &coord)
{
    if ( coord.row >= rows || coord.col >= cols) {
        throw std::runtime_error("Spatne zadane souradnice.");
    }
    return matrix[coord.row*cols + coord.col];
}

/**
 * @brief Nastaví na políčko dané souřadnicemi @a coord ukazatel na kámen @a src.
 * @param coord Souřadnice na cílové políčko.
 * @param src   Ukazatel na kámen.
 * @return Vrací uložený ukazatel na kámen @a src.
 *
 * @exception std::runtime_error Pokud jsou souřadnice mimo matici.
 */
Stone *Board::setField(const S_Coord &coord, Stone *src)
{
    if ( coord.row >= rows || coord.col >= cols) {
        throw std::runtime_error("Spatne zadane souradnice.");
    }
    return matrix[coord.row*cols + coord.col] = src;
}

/**
 * @brief Vymění políčko se souřadnicemi @a from s políčkem se souřadnicemi @to.
 * @param from Souřadnice na první políčko k prohození.
 * @param to   Souřadnice na druhé políčko k prohození.
 *
 * @exception std::runtime_error Pokud jsou souřadnice mimo matici.
 */
void Board::swapField(const S_Coord &from, const S_Coord &to)
{
    Stone *swap = getField(to);
    setField( to, getField(from) );
    setField( from, swap );
}
