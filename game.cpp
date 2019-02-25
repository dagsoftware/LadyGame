/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QRegExp>

#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game),
    draughts()
{
    ui->setupUi(this);

    game_state = RUN;
    createBoard();
    redrawBoard();

    stroke_from = false;

    connect( &buttonGroup, SIGNAL(buttonClicked(int)),
             this, SLOT(checkUserClick(int)) );
}

Game::~Game()
{
    delete ui;
}

void Game::createBoard()
{
    ui->gridLayout->setSpacing(0);

    QSizePolicy label_row_sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    QSizePolicy label_col_sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QFont label_font;
    label_font.setFamily(QString::fromUtf8("Arial"));
    label_font.setPointSize(18);
    label_font.setBold(true);
    label_font.setWeight(75);

    // Add Row Labels
    for (int r = 1; r < 9 ; ++r) {
        QLabel *label1 = new QLabel(this);
        QLabel *label2 = new QLabel(this);
        label1->setText( QString::number(9-r) );
        label1->setSizePolicy(label_row_sizePolicy);
        label1->setFont(label_font);
        label1->setMinimumSize(QSize(28, 0));
        label1->setAlignment(Qt::AlignCenter);

        label2->setText( label1->text() );
        label2->setSizePolicy( label1->sizePolicy() );
        label2->setFont( label1->font() );
        label2->setMinimumSize( label1->minimumSize() );
        label2->setAlignment( label1->alignment() );
        ui->gridLayout->addWidget(label1, r, 0, Qt::AlignCenter);
        ui->gridLayout->addWidget(label2, r, 9, Qt::AlignCenter);
    }

    // Add Col Labels
    for (int c = 1; c < 9 ; ++c) {
        QLabel *label1 = new QLabel(this);
        QLabel *label2 = new QLabel(this);
        label1->setText( QChar('A'+c-1) );
        label1->setSizePolicy(label_col_sizePolicy);
        label1->setFont(label_font);
        label1->setMinimumSize(QSize(0, 28));
        label1->setAlignment(Qt::AlignCenter);

        label2->setText( label1->text() );
        label2->setSizePolicy( label1->sizePolicy() );
        label2->setFont( label1->font() );
        label2->setMinimumSize( label1->minimumSize() );
        label2->setAlignment( label1->alignment() );
        ui->gridLayout->addWidget(label1, 0, c, Qt::AlignCenter);
        ui->gridLayout->addWidget(label2, 9, c, Qt::AlignCenter);
    }

    QSizePolicy field_sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    field_sizePolicy.setHorizontalStretch(0);
    field_sizePolicy.setVerticalStretch(0);
    field_sizePolicy.setHeightForWidth(true);

    // Add Buttons
    for (int r = 0; r < 8 ; ++r) {
        for (int c = 0; c < 8; ++c) {
            QPushButton *button = new QPushButton(this);
//            button->setBackgroundRole();
            button->setSizePolicy(field_sizePolicy);
            button->setMinimumSize(QSize(48, 48));
            button->setMaximumSize(QSize(48, 48));
            if ( !(r%2 || c%2) || (r%2 && c%2) )
                button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
            else
                button->setStyleSheet(QString::fromUtf8("background-color: rgb(100, 100, 100);"));
            buttonGroup.addButton( button, (7-r)*8+c );
            ui->gridLayout->addWidget(button, r+1, c+1);
        }
    }
}

void Game::checkUserClick(int index)
{
    if ( type == REPLAY || curPlayerType() != HOST ) {
        return;
    }

    Board board = draughts.getCheckerboard();
    S_Coord coord;
    coord.col = index % 8;
    coord.row = (index - coord.col) / 8;
    Stone *stone = board.getField(coord);
    bool stroke_to = false;

    if ( stone == NULL ) {
        // Pokud již bylo nastaveno výchozí pole, nastavím cílové.
        if (stroke_from) {
            stroke.to = coord;
            stroke_to = true;
        } else {
            return;
        }
    }
    else if ( stone->getColour() != draughts.whoHaveTurn() ) {
        return;
    }
    else {
        if (stroke_from) {
            buttonGroup.button(stroke.from.row*8+stroke.from.col)->setStyleSheet(QString::fromUtf8("background-color: rgb(100, 100, 100);"));
        }
        stroke.from = coord;
        stroke_from = true;
        buttonGroup.button(index)->setStyleSheet(QString::fromUtf8("background-color: rgb(100, 100, 250);"));
    }

    if (stroke_to) {
        try {
            draughts.playerMoveStone(stroke);
            redrawBoard();
            if ( game_type == SINGLE && draughts.getCurPlayer().getNumStones() > 0 ) {
                draughts.pcMoveStone();
                redrawBoard();
            }
            stroke_from = false;
        }
        catch (std::runtime_error e) {
            buttonGroup.button(stroke.from.row*8+stroke.from.col)->setStyleSheet(QString::fromUtf8("background-color: rgb(100, 100, 100);"));
            stroke_from = false;
            QMessageBox::information( this, tr("Warning"),
                                      tr("%1").arg(e.what()) );
        }
    }
}

void Game::redrawBoard()
{
    Board board = draughts.getCheckerboard();
    Player player_white = draughts.getWhitePlayer();
    Player player_black = draughts.getBlackPlayer();
    Stone *stone;
    QIcon ButtonIcon;

    for (int r = 0; r < 8 ; ++r) {
        for (int c = 0; c < 8; ++c) {
            stone = board.getField(S_Coord(r, c));
            if ( stone != NULL ) {
                QString stone_type = "pawn";
                if (stone->getType() == Stone::DAME) {
                    stone_type = "dame";
                }
                if (stone->getColour() == Stone::WHITE) {
                    ButtonIcon = QIcon( QString(":/img/stone/white_") + stone_type + QString(".png") );
                } else {
                    ButtonIcon = QIcon( QString(":/img/stone/black_") + stone_type + QString(".png") );
                }
                buttonGroup.button(r*8+c)->setIcon(ButtonIcon);
                QSize size = buttonGroup.button(r*8+c)->size();
                size.setWidth( size.height() );
                buttonGroup.button(r*8+c)->setIconSize(size);
                buttonGroup.button(r*8+c)->setStyleSheet(QString::fromUtf8("background-color: rgb(100, 100, 100);"));
            } else {
                buttonGroup.button(r*8+c)->setIcon(QIcon());
                if ( !(r%2 || c%2) || (r%2 && c%2) )
                    buttonGroup.button(r*8+c)->setStyleSheet(QString::fromUtf8("background-color: rgb(100, 100, 100);"));
                else
                    buttonGroup.button(r*8+c)->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
            }
        }
    }

    refreshRec();
    draughts.findCorrectStrokes();
    if (game_state != END) {
        if (player_black.getNumStones() == 0) {
            game_state = END;
            QMessageBox::information( this, tr("Congratulations!"), tr("White player won!") );
        }
        else if (player_white.getNumStones() == 0) {
            game_state = END;
            QMessageBox::information( this, tr("Congratulations!"), tr("Black player won!") );
        }
    }
}

void Game::refreshRec()
{
    std::stringstream stream;
    draughts.saveRecord(stream);
    ui->textBrowser->setText( stream.str().c_str() );
}

void Game::getHint()
{
    redrawBoard();
    S_Stroke hint;

    if ( ! draughts.getHint(hint) ) {
        return;
    }

    buttonGroup.button(hint.from.row*8+hint.from.col)->setStyleSheet(QString::fromUtf8("background-color: rgb(100, 250, 100);"));
    buttonGroup.button(hint.to.row*8+hint.to.col)->setStyleSheet(QString::fromUtf8("background-color: rgb(100, 250, 100);"));
}

void Game::save(const QString &fileName)
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
        QMessageBox::information( this, tr("Warning"), tr("Unknown file type.") );
    }
}

void Game::load(const QString &fileName)
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

void Game::replayStep()
{
    if ( draughts.getReplayNumCurTurn() == draughts.getRecord()->size() ) {
        Q_EMIT replayEnded();
        return;
    }
    draughts.replayStep();
    redrawBoard();
    return;
}

Game::PLAYER_TYPE Game::curPlayerType()
{
    if ( draughts.whoHaveTurn() == Stone::WHITE ) {
        return player_white;
    } else {
        return player_black;
    }
}

void Game::initPC()
{
    if ( curPlayerType() == PC ) {
        redrawBoard();
        draughts.pcMoveStone();
        redrawBoard();
    }
}

void Game::setGame(GAME_TYPE _game_type, PLAYER_TYPE _player_white)
{
    game_type = _game_type;
    player_white = _player_white;

    switch ( game_type ) {
    case SINGLE:
        if ( player_white == HOST ) {
            player_black = PC;
        } else {
            player_black = HOST;
        }
        initPC();
        break;
    case HOTSEAT:
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
