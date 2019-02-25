/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include <QTime>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "draughts.h"

/**
 * @brief Nastaví parametry @a row a @a col na hodnotu, které zajistí
 * pohyb směrem @a dir.
 *
 * @param dir Směr pohybu.
 * @param row Relativní hodnota, značící zda-li je směr @a dir o řádek výše nebo níže.
 * @param col Relativní hodnota, značící zda-li je směr @a dir o sloupec vlevo nebo vpravo.
 *
 * @exception std::runtime_error Pokud byl zadán neznámý směr.
 */
void setDirection(const Board::DIRECTION &dir, int &row, int &col)
{
    switch (dir) {
    case Board::SV:
        row = +1; col = +1;
        break;
    case Board::JV:
        row = -1; col = +1;
        break;
    case Board::JZ:
        row = -1; col = -1;
        break;
    case Board::SZ:
        row = +1; col = -1;
        break;
    default:
        throw std::runtime_error("Neznamy smer.");
        break;
    }
}

/**
 * @brief Nastaví hru do výchozí pozice.
 */
void Draughts::init()
{
    /*
     * Na začátku hry je třeba vytvořit šachvnici,
     * rozmístit správně kameny a přidělit je hráčům.
     */

    // Vytvářím šachovnici
    checkerboard.Init(size, size);

    // Vytvářím hráče
    initPlayers();

    // Začíná bílý hráč
    draught = Stone::WHITE;

    // Zasobník tahů je prázdný
    record.clear();
}

/**
 * @brief Nastaví hráče do výchozí pozice.
 */
void Draughts::initPlayers()
{
    const unsigned num_stones = 3*size/2; // Pro velikost šachovnice 8x8 je 12 (3 řady, každé druhé pole)
    unsigned stone_cnt;

    // Vytvoříme bílému hráči dvanáct bílých kamenů.
    player_white = Player("White", Stone::WHITE);
    player_white.stones.clear();
    player_white.setNumStones(num_stones);
    player_white.stones.resize(num_stones, Stone(Stone::WHITE));
    stone_cnt = 0;
    for (uint r = 0; r < 3; ++r) {
        uint start = (r%2 == 0) ? (0) : (1);
        for (uint c = start; c < size; c += 2, ++stone_cnt)
            checkerboard.setField(S_Coord(r,c), &player_white.stones[stone_cnt]);
    }

    // Vytvoříme černému hráči dvanáct černých kamenů.
    player_black = Player("Black", Stone::BLACK);
    player_black.stones.clear();
    player_black.setNumStones(num_stones);
    player_black.stones.resize(num_stones, Stone(Stone::BLACK));
    stone_cnt = 0;
    for (uint r = size-3; r < size; ++r) {
        uint start = (r%2 == 0) ? (0) : (1);
        for (uint c = start; c < size; c += 2, ++stone_cnt)
            checkerboard.setField(S_Coord(r,c), &player_black.stones[stone_cnt]);
    }
}

/**
 * @brief Přejmenuje hráče podle zadaných argumentů.
 * @param white Nové jméno bílého hráče.
 * @param black Nové jméno černého hráče.
 */
void Draughts::renamePlayers(const QString &white, const QString &black)
{
    player_white.setName(white);
    player_black.setName(black);
}

/**
 * @brief Zjistí zda-li kámen-pěšák na souřadnicích @a coord může jít nebo
 * skákat směrem @a dir a nastaví @a stroke a vrátí true pokud nalezne možný tah.
 *
 * @param coord  Souřadnice kamene.
 * @param dir    Směr.
 * @param stroke Naplní se možným tahem nebo bude nedefinováno.
 * @return Vrací true pokud nalezne možný tah.
 */
bool Draughts::lookAtDirPawn(const S_Coord &coord, const Board::DIRECTION &dir, S_Stroke &stroke)
{
    bool have_stroke = false;
    Stone *stone;
    S_Coord strike_at;
    S_Coord to;

    int row, col;
    setDirection(dir, row, col);

    to = S_Coord(coord.row+row, coord.col+col);
    if (to.row >= size || to.col >= size)
        return have_stroke;

    stone = checkerboard.getField(to);
    if ( stone == NULL ) {
        stroke = S_Stroke(coord, to);
        have_stroke = true;
    }
    else if (stone->getColour() != draught) {
        strike_at = to;
        to = S_Coord(to.row+row, to.col+col);
        if (to.row >= size || to.col >= size) {
            return have_stroke;
        }
        stone = checkerboard.getField(to);
        if ( stone == NULL ) {
            stroke = S_Stroke(coord, to, strike_at);
            have_stroke = true;
        }
    }

    return have_stroke;
}

/**
 * @brief Zjistí zda-li kámen-dáma na souřadnicích @a coord může jít nebo skákat
 * směrem @a dir a přímo dámě nastaví možné tahy tímto směrem.
 *
 * @param coord  Souřadnice kamene.
 * @param dir    Směr.
 * @return Vrací true pokud nalezne útočící tah.
 */
bool Draughts::lookAtDirDame(const S_Coord &coord, const Board::DIRECTION &dir)
{
    bool any_strike = false;
    Stone *stone_on_coord = checkerboard.getField(coord);
    Stone *stone_on_dir;
    S_Coord strike_at;
    S_Coord to;

    int row, col;
    setDirection(dir, row, col);

    to = S_Coord(coord.row+row, coord.col+col);
    while (to.row < size && to.col < size) {
        stone_on_dir = checkerboard.getField(to);
        if ( stone_on_dir == NULL ) {
            stone_on_coord->setStroke(S_Stroke(coord, to));
        }
        else if (stone_on_dir->getColour() != draught) {
            strike_at = to;
            to = S_Coord(to.row+row, to.col+col);

            while (to.row < size && to.col < size) {
                stone_on_dir = checkerboard.getField(to);
                if ( stone_on_dir != NULL )
                    break;
                any_strike = true;
                stone_on_coord->setStroke(S_Stroke(coord, to, strike_at));
                to = S_Coord(to.row+row, to.col+col);
            }

            break;
        }
        else {
            break;
        }

        to = S_Coord(to.row+row, to.col+col);
    }

    return any_strike;
}

/**
 * @brief Nastaví kamenu na souřadnicích @a coord možné tahy.
 * @param coord Souřadnice kamene.
 * @return Vrací true pokud byl zaznamenán útočící tah.
 */
bool Draughts::setCorrectStroke(const S_Coord &coord)
{
    bool any_strike = false;
    Stone *stone = checkerboard.getField(coord);
    S_Stroke stroke;
    std::vector<Board::DIRECTION> dir;

    if (stone->getType() == Stone::PAWN) {
        if (stone->getColour() == Stone::WHITE) {
            dir.push_back(Board::SV); dir.push_back(Board::SZ);
        }
        else {
            dir.push_back(Board::JV); dir.push_back(Board::JZ);
        }
        for (uint i = 0; i < dir.size(); ++i) {
            if ( lookAtDirPawn(coord, dir[i], stroke) ) {
                if (stroke.strike)
                    any_strike = true;
                stone->setStroke(stroke);
            }
        }
    }
    else { // Jinak táhnu s dámou
        dir.push_back(Board::SV); dir.push_back(Board::JV); dir.push_back(Board::JZ); dir.push_back(Board::SZ);
        for (uint i = 0; i < dir.size(); ++i) {
            if ( lookAtDirDame(coord, dir[i]) ) {
                any_strike = true;
            }
        }
    }

    return any_strike;
}

/**
 * @brief Nalezne a nastaví všem kamenům aktuálního hráče všechny možné tahy.
 */
void Draughts::findCorrectStrokes()
{
    bool any_strike = false;
    Stone *stone;

    for (uint r = 0; r < size; ++r) {
        uint start = (r%2 == 0) ? (0) : (1);
        for (uint c = start; c < size; c += 2) {
            stone = checkerboard.getField(S_Coord(r,c));
            if ( stone == NULL || stone->getColour() != draught )
                continue;
            if ( setCorrectStroke(S_Coord(r,c)) )
                any_strike = true;
        }
    }

    Player *cur_player = (draught == Stone::WHITE) ? (&player_white) : (&player_black);
    if (any_strike) {
        for (unsigned i=0; i < cur_player->stones.size(); ++i) {
            cur_player->stones[i].rmNonStrikeStrokes();
        }
    }

    possible.clear();
    // Kameny, se kterými je možno táhnout, se uloží do pole
    for (unsigned i=0; i < cur_player->stones.size(); ++i) {
        if ( cur_player->stones[i].getStrokes()->size() > 0 ) {
            possible.push_back( &cur_player->stones[i] );
        }
    }
}

/**
 * @brief Nalezne útok v tahu @a stroke a nastaví ho do @a strike.
 * Použitelné při síťové hře a načítání ze souboru.
 *
 * @param stroke Aktuální tah.
 * @param strike Souřadnice oběti na kterou bylo útočeno.
 * @return Vrací true v případě nalezení útoku.
 */
bool Draughts::getStrikeCoord(const S_Stroke &stroke, S_Coord &strike)
{
    bool any_strike = false;
    Stone *stone;

    int row = stroke.to.row - stroke.from.row;
    int col = stroke.to.col - stroke.from.col;
    row /= abs(row);
    col /= abs(col);

    strike = S_Coord(stroke.from.row+row, stroke.from.col+col);
    while (strike.row != stroke.to.row && strike.col != stroke.to.col) {
        stone = checkerboard.getField(strike);
        if ( stone != NULL ) {
            any_strike = true;
            break;
        }

        strike = S_Coord(strike.row+row, strike.col+col);
    }

    return any_strike;
}

/**
 * @brief Když hráč (uživatel) přesune kámen, zajistí korektnost tahu @a stroke a předání tahu protihráči.
 * @param stroke Tah provedený uživatelem.
 */
void Draughts::playerMoveStone(const S_Stroke &stroke)
{
    S_Stroke found;
    Stone *stone = checkerboard.getField(stroke.from);
    if ( stone == NULL || ! stone->findStroke(stroke, found) ) {
        throw std::runtime_error("This move isn't possible!");
    }

    // Zaznamenám tah.
    record.push_back(found);

    // Pokud hráč útočí, odstraním protihráči figurku.
    if (found.strike) {
        Player *sec_player = (draught != Stone::WHITE) ? (&player_white) : (&player_black);
        ( checkerboard.getField(found.strike_at) )->setDeathTurn(record.size());
        checkerboard.clearField(found.strike_at);
        sec_player->setNumStones( sec_player->getNumStones()-1 );
    }
    checkerboard.swapField(found.from, found.to);

    // Pokud se kámen dostal na horní resp. dolní řádek stává se dámou.
    if ( (found.to.row == 0 && draught == Stone::BLACK) ||
         (found.to.row == size-1 && draught == Stone::WHITE) ) {
        if ( checkerboard.getField(found.to)->getType() == Stone::PAWN ) {
            checkerboard.getField(found.to)->setUpgTurn(record.size());
            checkerboard.getField(found.to)->setType(Stone::DAME);
        }
    }

    // Vyčistím možné tahy aktuálního hráče.
    Player *cur_player = (draught == Stone::WHITE) ? (&player_white) : (&player_black);
    for (unsigned i=0; i < cur_player->stones.size(); ++i) {
        cur_player->stones[i].clearStrokes();
    }

    // Předávám tah dalšímu hráči.
    draught = (draught == Stone::WHITE) ? (Stone::BLACK) : (Stone::WHITE);
}

/**
 * @brief Při síťové hře nebo načítání ze souboru zajistí provedení tahu a předání tahu protihráči.
 * @param stroke Tah načtený ze souboru nebo ze sítě.
 */
void Draughts::moveStone(const S_Stroke &stroke)
{
    S_Stroke found = stroke;
    S_Coord strike;

    if ( getStrikeCoord(found, strike) ) {
        found.strike = true;
        found.strike_at = strike;
    }
    else {
        found.strike = false;
    }

    // Zaznamenám tah.
    record.push_back(found);

    // Pokud hráč útočí, odstraním protihráči figurku.
    if (found.strike) {
        Player *sec_player = (draught != Stone::WHITE) ? (&player_white) : (&player_black);
        ( checkerboard.getField(found.strike_at) )->setDeathTurn(record.size());
        checkerboard.clearField(found.strike_at);
        sec_player->setNumStones( sec_player->getNumStones()-1 );
    }
    checkerboard.swapField(found.from, found.to);

    // Pokud se kámen dostal na horní resp. dolní řádek stává se dámou.
    if ( (found.to.row == 0 && draught == Stone::BLACK) ||
         (found.to.row == size-1 && draught == Stone::WHITE) ) {
        if ( checkerboard.getField(found.to)->getType() == Stone::PAWN ) {
            checkerboard.getField(found.to)->setUpgTurn(record.size());
            checkerboard.getField(found.to)->setType(Stone::DAME);
        }
    }

    // Předávám tah dalšímu hráči.
    draught = (draught == Stone::WHITE) ? (Stone::BLACK) : (Stone::WHITE);
}



/**
 * @brief Nastaví hru, aby se mohla přehrát od začátku.
 */
void Draughts::initToReplay()
{
    checkerboard.Init(size, size);
    initPlayers();
    draught = Stone::WHITE;
    replay_cur_turn = 0;
}

/**
 * @brief Přehraje další tah ze záznamu.
 */
void Draughts::replayStep()
{
    if (replay_cur_turn == record.size()) {
        return;
    }
    S_Stroke found = record[replay_cur_turn++];

    // Pokud hráč útočí, odstraním protihráči figurku.
    if (found.strike) {
        Player *sec_player = (draught != Stone::WHITE) ? (&player_white) : (&player_black);
        checkerboard.getField(found.strike_at)->setDeathTurn(replay_cur_turn-1);
        checkerboard.clearField(found.strike_at);
        sec_player->setNumStones( sec_player->getNumStones()-1 );
    }
    checkerboard.swapField(found.from, found.to);

    // Pokud se kámen dostal na horní resp. dolní řádek stává se dámou.
    if ( (found.to.row == 0 && draught == Stone::BLACK) ||
         (found.to.row == size-1 && draught == Stone::WHITE) ) {
        if ( checkerboard.getField(found.to)->getType() == Stone::PAWN ) {
            checkerboard.getField(found.to)->setUpgTurn(replay_cur_turn-1);
            checkerboard.getField(found.to)->setType(Stone::DAME);
        }
    }

    // Předávám tah dalšímu hráči.
    draught = (draught == Stone::WHITE) ? (Stone::BLACK) : (Stone::WHITE);
}

/**
 * @brief Provede v opačném pořadí předchozí tah a tím se dostane o tah zpět.
 */
void Draughts::replayBack()
{
    if (replay_cur_turn == 0) {
        return;
    }
    S_Stroke found = record[--replay_cur_turn];

    // Vracím tah předchozímu hráči.
    draught = (draught == Stone::WHITE) ? (Stone::BLACK) : (Stone::WHITE);

    // Pokud se stal některý kámen dámou, musím ho změnit zpět.
    if ( (found.to.row == 0 && draught == Stone::BLACK) ||
         (found.to.row == size-1 && draught == Stone::WHITE) ) {
        if (checkerboard.getField(found.to)->getUpgTurn() == replay_cur_turn) {
            checkerboard.getField(found.to)->setType(Stone::PAWN);
        }
    }

    // Pokud hráč útočil, vrátím protihráči figurku.
    if (found.strike) {
        Player *sec_player = (draught != Stone::WHITE) ? (&player_white) : (&player_black);
        unsigned stone;
        for (stone = 0; stone < sec_player->stones.size(); ++stone) {
            if (sec_player->stones[stone].getDeathTurn() == replay_cur_turn) {
                break;
            }
        }
        checkerboard.setField(found.strike_at, &sec_player->stones[stone]);
        sec_player->setNumStones( sec_player->getNumStones()+1 );
    }
    checkerboard.swapField(found.from, found.to);
}



/**
 * @brief Uloží hru v základní notaci.
 * @param out_stream Výstupní stream.
 */
void Draughts::saveRecord(std::ostream &out_stream)
{
    for (uint i = 0; i < record.size(); ++i) {
        if ( i%2 == 0 ) {
            out_stream << (i/2)+1 << ". ";
            out_stream << record[i].stroke2str() << " ";
        }
        if ( i%2 ) {
            out_stream << record[i].stroke2str() << "\n";
        }
    }
    if (record.size() % 2) {
        // Černý hráč neukončil tah.
        out_stream << "\n";
    }
    out_stream << std::flush;
}

/**
 * @brief Umožňuje ze souboru prostého textu načíst záznam tahů v základní notaci.
 * @param in_stream Vstupní stream.
 */
void Draughts::loadRecord(std::istream &in_stream)
{
    QString qstr;
    QStringList list;
    std::string line;

    init();

    while ( in_stream.good() ) {
        std::getline(in_stream, line);
        qstr = QString::fromStdString(line);
        list = qstr.split(" ");
        if ( list.size() > 1 ) {
            moveStone(S_Stroke(list[1]));
            // list: [ "1.", "a5-b6", "c7xa5" ]
            if ( list.size() > 2 && ! list[2].isEmpty() ) {
                moveStone(S_Stroke(list[2]));
            }
        }
    }
}

/**
 * @brief Uloží hru ve vlastním formátu XML.
 * @param filename Jméno výstupního souboru.
 */
void Draughts::saveRecordXML(const QString &filename)
{
    QFile file(filename);
    /*open a file */
    if ( ! file.open(QIODevice::WriteOnly) ) {
        throw std::runtime_error("Cannot open file to write.");
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("strokes");
    for (uint i = 0; i < record.size(); ++i) {
        // Začátek tahu
        if ( i%2 == 0 ) {
            xmlWriter.writeStartElement("stroke");
            xmlWriter.writeAttribute( "num", QString::number((i/2)+1) );

            xmlWriter.writeStartElement("white_player");
            xmlWriter.writeCharacters( record[i].stroke2qstr() );
            xmlWriter.writeEndElement();
        }

        if ( i%2 ) {
            xmlWriter.writeStartElement("black_player");
            xmlWriter.writeCharacters( record[i].stroke2qstr() );
            xmlWriter.writeEndElement();

            // Konec tahu
            xmlWriter.writeEndElement();
        }
    }
    if (record.size() % 2) {
        // Černý hráč neukončil tah.
        xmlWriter.writeEndElement();
    }
    // Konec elementu "strokes"
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
}

bool Draughts::xmlStrokeParse(QXmlStreamReader &xml)
{
    if (xml.tokenType() != QXmlStreamReader::StartElement ||
            xml.name() != "stroke") {
        return (false);
    }
    if (xml.readNext() != QXmlStreamReader::StartElement ||
            xml.name() != "white_player") {
        return (false);
    }
    if (xml.readNext() != QXmlStreamReader::Characters) {
        return (false);
    }
    moveStone(S_Stroke(xml.text().toString()));
    if (xml.readNext() != QXmlStreamReader::EndElement ||
            xml.name() != "white_player") {
        return (false);
    }
    xml.readNext();
    if (xml.tokenType() == QXmlStreamReader::StartElement &&
            xml.name() == "black_player") {
        if (xml.readNext() != QXmlStreamReader::Characters) {
            return (false);
        }
        moveStone(S_Stroke(xml.text().toString()));
        if (xml.readNext() != QXmlStreamReader::EndElement ||
                xml.name() != "black_player") {
            return (false);
        }
        xml.readNext();
    }
    if (xml.tokenType() != QXmlStreamReader::EndElement ||
            xml.name() != "stroke") {
        return (false);
    }
    return (true);
}

/**
 * @brief Načte ze souboru ve vlastním formátu XML tahy.
 * @param filename Jméno zdrojového souboru.
 */
void Draughts::loadRecordXML(const QString &filename)
{
    QFile file(filename);
    /*open a file */
    if ( ! file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        throw std::runtime_error("Cannot open file to read.");
    }

    QXmlStreamReader xml(&file);
    while ( !xml.atEnd() && !xml.hasError() ) {
        /* Přečtu další element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* Pokud narazím na začátek dokumentu, pokračuji na další prvek.*/
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        /* Pokud narazím na začátek elementu, podívám se zda-li ho můžu přečíst. */
        if(token == QXmlStreamReader::StartElement) {
            if(xml.name() == "strokes") {
                continue;
            }
            if(xml.name() == "stroke") {
                xmlStrokeParse(xml);
            }
        }
    }
    if ( xml.hasError() ) {
        throw std::runtime_error( xml.errorString().toStdString() );
    }
    xml.clear();
}

bool Draughts::getHint(S_Stroke &hint)
{
    Stone *stone;
    Stone::Vec_Stroke *strokes;
    qsrand( (uint) QTime::currentTime().msec() );

    if ( possible.empty() ) {
        return (false);
    }

    stone = possible[qrand() % possible.size()];
    strokes = stone->getStrokes();
    hint = (*strokes)[qrand() % strokes->size()];

    return (true);
}
