/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include <QString>
#include <QRegExp>

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <stdexcept>

#include "game-cli.h"

GameCLI::GameCLI() :
    draughts()
{
    game_state = RUN;
    printBoard();
}

void GameCLI::printHelp()
{
    std::cout <<
                 "\nWhat needs to happen:\n"
                 "=====================\n"
                 "help            - Display this help.\n"
                 "hint            - Display random possible stroke.\n"
                 "history         - Display history.\n"
                 "save \"filename\" - Save game only with suffix txt or xml.\n"
                 "quit            - Quit game.\n"
                 "\n"
                 "c3-b4           - Example of stroke.\n"
                 << std::endl;
}

void GameCLI::prompt()
{
    S_Stroke stroke;
    std::string answer;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "What needs to happen? (Type 'help' for help.)" << std::endl;
    std::getline(std::cin, answer);

    if      ( answer == "help" )
        printHelp();
    else if ( answer == "hint" && game_state == RUN )
        printHint();
    else if ( answer == "history" )
        printHistory();
    else if ( answer == "quit" )
        game_state = QUIT;
    else if ( answer.size() == 5 && game_state == RUN ) {
        stroke.str2stroke( answer );
        draughts.playerMoveStone(stroke);
        printBoard();
        if ( game_type == SINGLE && draughts.getCurPlayer().getNumStones() > 0 ) {
            draughts.pcMoveStone();
            printBoard();
        }
    }
    else if ( answer.size() > 7 ) {
        if ( answer.substr(0, 4) == "save" ) {
            save( QString::fromStdString(answer.substr(6, answer.size()-7)) );
        }
        else {
            std::cout << "Unknown command." << std::endl;
        }
    }
    else {
        std::cout << "Unknown command." << std::endl;
    }
}

void GameCLI::printHelpReplay()
{
    std::cout <<
                 "\nWhat needs to happen:\n"
                 "=====================\n"
                 "help            - Display this help.\n"
                 "back            - Back last draught.\n"
                 "step            - Next draught.\n"
                 "history         - Display history.\n"
                 "quit            - Quit game.\n"
                 << std::endl;
}

void GameCLI::promptReplay()
{
    std::string answer;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "What needs to happen? (Type 'help' for help.)" << std::endl;
    std::getline(std::cin, answer);

    if      ( answer == "help" )
        printHelpReplay();
    else if ( answer == "back")
        replayBack();
    else if ( answer == "step")
        replayStep();
    else if ( answer == "history" )
        printHistory();
    else if ( answer == "quit" )
        game_state = QUIT;
    else {
        std::cout << "Unknown command." << std::endl;
    }
}

void GameCLI::printBoard()
{
//    a b c d e f g h
//    _-_-_-_-_-_-_-_
//  8| |b| |b| |b| |b|8
//  7|b| |b| |b| |b| |7
//  6| |b| |b| |b| |b|6
//  5| | | | | | | | |5
//  4| | | | | | | | |4
//  3|w| |w| |w| |w| |3
//  2| |w| |w| |w| |w|2
//  1|w| |w| |w| |w| |1
//    ^-^-^-^-^-^-^-^
//    a b c d e f g h

    const std::string row_label("  a b c d e f g h");
    const std::string top_border("  _-_-_-_-_-_-_-_");
    const std::string bottom_border("  ^-^-^-^-^-^-^-^");

    Board board = draughts.getCheckerboard();
    Player player_white = draughts.getWhitePlayer();
    Player player_black = draughts.getBlackPlayer();
    Stone *stone;
    QString row = "%1|%2|%3|%4|%5|%6|%7|%8|%9|%1";
    QString cur_row = row;

    std::cout << row_label << '\n' << top_border << '\n';
    for (int r = 7; r >= 0 ; --r) {
        cur_row = row;
        cur_row = cur_row.arg(r+1);
        for (int c = 0; c < 8; ++c) {
            stone = board.getField(S_Coord(r, c));
            if ( stone != NULL ) {
                if (stone->getColour() == Stone::WHITE) {
                    cur_row = cur_row.arg( (stone->getType() == Stone::DAME) ? ('W') : ('w') );
                } else {
                    cur_row = cur_row.arg( (stone->getType() == Stone::DAME) ? ('B') : ('b') );
                }
            } else {
                cur_row = cur_row.arg(' ');
            }
        }
        std::cout << cur_row.toStdString() << '\n';
    }
    std::cout << bottom_border << '\n' << row_label << std::endl;

    draughts.findCorrectStrokes();
    if (player_black.getNumStones() == 0) {
        game_state = END;
        std::cout << "Congratulations! White player won!" << std::endl;
    }
    else if (player_white.getNumStones() == 0) {
        game_state = END;
        std::cout << "Congratulations! Black player won!" << std::endl;
    }
//    else {
//        draughts.findCorrectStrokes();
//    }
}

void GameCLI::printHint()
{
    S_Stroke hint;

    if ( ! draughts.getHint(hint) ) {
        std::cout << "Not any possible hint." << std::endl;
        return;
    }

    std::cout << hint.stroke2str() << std::endl;
    return;
}

void GameCLI::printHistory()
{
    std::stringstream stream;
    draughts.saveRecord(stream);
    std::cout << stream.str() << std::endl;
}

void GameCLI::save(const QString &fileName)
{
    std::ofstream outfile;

    if ( fileName.contains(QRegExp("^.*[.]xml$", Qt::CaseInsensitive)) ) {
        draughts.saveRecordXML(fileName);
    }
    else if ( fileName.contains(QRegExp("^.*[.]txt$", Qt::CaseInsensitive)) ) {
        outfile.open( fileName.toUtf8().data() );
        draughts.saveRecord(outfile);
        outfile.close();
    }
    else {
        throw std::runtime_error("Unknown file type.");
    }
}

void GameCLI::load(const QString &fileName)
{
    std::ifstream infile;

    if ( fileName.contains(QRegExp("^.*[.]xml$", Qt::CaseInsensitive)) ) {
        draughts.loadRecordXML(fileName);
    }
    else if ( fileName.contains(QRegExp("^.*[.]txt$", Qt::CaseInsensitive)) ) {
        infile.open( fileName.toUtf8().data() );
        draughts.loadRecord(infile);
        infile.close();
    }
    else {
        throw std::runtime_error("Unknown file type.");
    }
}

void GameCLI::replayStep()
{
    if ( draughts.getReplayNumCurTurn() == draughts.getRecord()->size() ) {
        std::cout << "Replay done." << std::endl;
        return;
    }
    draughts.replayStep();
    printBoard();
    return;
}

GameCLI::PLAYER_TYPE GameCLI::curPlayerType()
{
    if ( draughts.whoHaveTurn() == Stone::WHITE ) {
        return player_white;
    } else {
        return player_black;
    }
}

void GameCLI::initPC()
{
    if ( curPlayerType() == PC ) {
        draughts.pcMoveStone();
        printBoard();
    }
}

void GameCLI::setGame(GAME_TYPE _game_type, Stone::COLOUR my_color)
{
    game_type = _game_type;
    draughts.findCorrectStrokes();

    switch ( game_type ) {
    case SINGLE:
        if ( my_color == Stone::WHITE ) {
            player_white = HOST;
            player_black = PC;
        } else {
            player_white = PC;
            player_black = HOST;
        }
        initPC();
        break;
    case HOTSEAT:
        player_white = HOST;
        player_black = HOST;
        break;
    case SERVER:
        // TODO
        break;
    case CLIENT:
        // TODO
        break;
    default:
        throw std::runtime_error("Neošetřený enum GAME_TYPE.");
        break;
    }
}
