/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include <QApplication>
#include <iostream>
#include <stdexcept>

#include "game-cli.h"

/**
 * Struktura obsahující hodnoty parametrů příkazové řádky.
 */
struct TParams {
    bool replay;
    GameCLI::GAME_TYPE game_type; ///< Typ hry.
    QString loadFile; ///< Soubor pro načtení.
    Stone::COLOUR my_colour;      ///< Barva hostujícího hráče.
};

/**
 * Zpracuje argumenty příkazového řádku a vrátí je ve struktuře TParams.
 * Pokud je formát argumentů chybný, ukončí program s chybovým kódem.
 * @param[in] argc Počet argumentů.
 * @param[in] argv Pole textových řetězců s argumenty.
 * @return Vrací analyzované argumenty příkazového řádku.
 */
TParams getParams(int argc, char *argv[])
{
    TParams result;

    result.loadFile = "";
    result.replay = false;
    const std::string example1 = "single white";
    const std::string example2 = "hotseat";
    const std::string example3 = "replay souboj.txt";
    const std::string arguments1 = "{single white|black}|{hotseat} [loadfile txt or xml]";
    const std::string arguments2 = "replay <loadfile txt or xml>";
    std::string err_msg = "Bad arguments.\n";
    err_msg += "Usage: "; err_msg += argv[0]; err_msg += " "+arguments1;
    err_msg += "\nUsage: "; err_msg += argv[0]; err_msg += " "+arguments2;
    err_msg += "\nExample: "; err_msg += argv[0]; err_msg += " "+example1 + std::string(" souboj.txt");
    err_msg += "\n         "; err_msg += argv[0]; err_msg += " "+example2;
    err_msg += "\n         "; err_msg += argv[0]; err_msg += " "+example3;

    if (argc >= 2 && argc <= 4) {
        std::string arg1;
        std::string arg2;
        arg1 = argv[1];
        if (arg1 == "hotseat") {
            result.game_type = GameCLI::HOTSEAT;
            if (argc == 3) {
                result.loadFile = argv[2];
            }
            else if (argc > 3) {
                throw std::runtime_error(err_msg);
            }
        }
        else if (arg1 == "single") {
            if (argc == 3 || argc == 4) {
                arg2 = argv[2];
                if (arg2 != "white" && arg2 != "black") {
                    throw std::runtime_error(err_msg);
                }
                if (argc == 4) {
                    result.loadFile = argv[3];
                }
                result.game_type = GameCLI::SINGLE;
                result.my_colour = (arg2 == "white") ? (Stone::WHITE) : (Stone::BLACK);
            }
            else {
                throw std::runtime_error(err_msg);
            }
        }
        else if (arg1 == "replay") {
            if (argc == 3) {
                result.replay = true;
                result.loadFile = argv[2];
            }
            else {
                throw std::runtime_error(err_msg);
            }
        }
        else {
            throw std::runtime_error(err_msg);
        }
    }
    else { // moc nebo málo argumentů
        throw std::runtime_error(err_msg);
    }

    return result;
}

int main(int argc, char *argv[])
{
    GameCLI game;
    TParams params;

    try {
        params = getParams(argc, argv);
    }
    catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if ( params.replay) {
        game.setType(GameCLI::REPLAY);
        try {
            game.load(params.loadFile);
        }
        catch (std::runtime_error e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }

        game.initToReplay();
        while (game.getGameState() != GameCLI::QUIT) {
            try {
                game.promptReplay();
            }
            catch (std::runtime_error e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
    else {
        game.setType(GameCLI::GAME);
        try {
            if (params.loadFile != "") {
                game.load(params.loadFile);
                game.printBoard();
            }
            game.setGame(params.game_type, params.my_colour);
        }
        catch (std::runtime_error e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }

        while (game.getGameState() != GameCLI::QUIT) {
            try {
                game.prompt();
            }
            catch (std::runtime_error e) {
                std::cout << e.what() << std::endl;
            }
        }
    }

    return 0;
}
