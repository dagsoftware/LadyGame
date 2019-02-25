/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef STROKE_H
#define STROKE_H

#include <string>
#include <QString>

/**
 * @brief The S_Coord struct
 */
struct S_Coord
{
    unsigned row, col;

    S_Coord() : row(0), col(0) {}
    S_Coord(unsigned _row, unsigned _col) : row(_row), col(_col) {}

    bool operator < (S_Coord right) const { return (row < right.row || (row == right.row && col < right.col)) ? (true) : (false); }
    bool operator == (S_Coord right) const { return (row == right.row && col == right.col) ? (true) : (false); }
};

/**
 * @brief The S_Stroke struct
 */
struct S_Stroke
{
    S_Coord from;      // Ze souřadnice.
    S_Coord to;        // Na souřadnici.
    bool strike;       // Obsahuje "true" pokud byl zaznamenán útok.
    S_Coord strike_at; // Na jaké souřadnice byl zaznamenán útok.

    S_Stroke() {}
    S_Stroke(const std::string &str) { str2stroke(str); }
    S_Stroke(const QString &qstr) { str2stroke(qstr); }
    S_Stroke(S_Coord _from, S_Coord _to) :
        from(_from), to(_to), strike(false) {}
    S_Stroke(S_Coord _from, S_Coord _to, S_Coord _strike_at) :
        from(_from), to(_to), strike(true), strike_at(_strike_at) {}

    std::string stroke2str();
    QString stroke2qstr() { return QString::fromStdString(stroke2str()); }
    void str2stroke(const std::string &str);
    void str2stroke(const QString &qstr);

    bool operator == (S_Stroke right) const { return (from == right.from && to == right.to) ? (true) : (false); }
};

#endif // STROKE_H
