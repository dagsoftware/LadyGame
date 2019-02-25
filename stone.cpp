/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include "stone.h"

bool Stone::correctStroke(const S_Stroke &stroke)
{
    return ( std::find(strokes.begin(), strokes.end(), stroke) != strokes.end() ) ? (true) : (false);
}

bool Stone::findStroke(const S_Stroke &temp, S_Stroke &found)
{
    Vec_Stroke::iterator it_found;
    if ( (it_found = std::find(strokes.begin(), strokes.end(), temp)) != strokes.end() ) {
        found = *it_found;
        return (true);
    }
    return (false);
}

void Stone::rmNonStrikeStrokes()
{
    Vec_Stroke::iterator first = strokes.begin();
    Vec_Stroke::iterator last = strokes.end();
    Vec_Stroke::iterator result = first;

    while (first!=last) {
        if (first->strike != false) {
            *result = *first;
            ++result;
        }
        ++first;
    }

    strokes.erase(result, last);
}
