/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include <sstream>
#include <stdexcept>

#include <QString>
#include <QRegExp>

#include <assert.h>

#include "stroke.h"

char num2alfa(unsigned num)
{
    assert( num <= ('z'-'a') );
    return 'a'+num;
}

unsigned alfa2num(char alfa)
{
    assert(alfa >= 'a' && alfa <= 'z');
    return alfa-'a';
}

/**
 * @brief S_Stroke::stroke2str
 * @return Vrací tah čitelný uživatelem (např. "a3-b4")
 */
std::string S_Stroke::stroke2str()
{
    std::stringstream stream;
    stream << num2alfa(from.col) << from.row + 1;
    stream << ( (strike)?('x'):('-') );
    stream << num2alfa(to.col) << to.row + 1;
    return stream.str();
}

/**
 * @brief S_Stroke::str2stroke
 * @param str Řetězec odpovídající regulárnímu výrazu "^[a-h][1-8][-x][a-h][1-8]$"
 */
void S_Stroke::str2stroke(const std::string &str)
{
    QString qstr = QString::fromStdString(str);
    str2stroke(qstr);
}

/**
 * @brief S_Stroke::str2stroke
 * @param str Řetězec odpovídající regulárnímu výrazu "^[a-h][1-8][-x][a-h][1-8]$"
 */
void S_Stroke::str2stroke(const QString &qstr)
{
    if ( ! qstr.contains( QRegExp("^[a-h][1-8][-x][a-h][1-8]$") ) ) {
        throw std::runtime_error("Incorrectly written move.");
    }
    from.col = alfa2num( qstr[0].toAscii() );
    from.row = qstr[1].digitValue() - 1;
    strike = ( qstr[2] == QChar('x') ) ? (true) : (false);
    to.col = alfa2num( qstr[3].toAscii() );
    to.row = qstr[4].digitValue() - 1;
}
