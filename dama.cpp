/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include <QFileDialog>
#include <QMessageBox>
#include <stdexcept>

#include "dama.h"
#include "ui_dama.h"

#include "sure_about.h"
#include "about.h"
#include "new_game.h"

Dama::Dama(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dama)
{
    ui->setupUi(this);
    ui->tabWidget->setTabsClosable(true);
    ui->actionSave->setEnabled(false);
    ui->actionAbort->setEnabled(false);
    ui->actionHint->setEnabled(false);
    ui->replayToolBar->setEnabled(false);
    ui->replayToolBar->setVisible(false);

    replay_timer = new QTimer(this);
    replay_delay = new QTimeEdit(this);
    replay_upto = new QSpinBox(this);
    replay_cur_turn = new QLabel(this);

    replay_delay->setDisplayFormat( QString("ss.zzz") );
    replay_delay->setToolTip( tr("Step time (sec.ms)") );
    replay_delay->setTime( QTime(0,0,1) );
    ui->replayToolBar->insertWidget( ui->actionPlay_auto, replay_delay );
    replay_upto->setToolTip( tr("Up to draught no. (0 = all)") );
    ui->replayToolBar->insertWidget( ui->actionPlay_auto, replay_upto );
    replay_cur_turn->setToolTip( tr("Current/End draught no.") );
    replay_cur_turn->setText( QString("Current/End") );
    ui->replayToolBar->addWidget( replay_cur_turn );

    connect( ui->actionQuit, SIGNAL(triggered()),
             qApp, SLOT(quit()) );
    connect( ui->tabWidget, SIGNAL(tabCloseRequested(int)),
             this, SLOT(closeTab(int)) );
    connect( replay_timer, SIGNAL(timeout()),
             this, SLOT(timeoutReplayTimer()) );
}

Dama::~Dama()
{
    delete ui;
}

void Dama::on_actionNew_triggered()
{
    NewGame *dialog_new_game = new NewGame(this);
    QObject::connect( dialog_new_game, SIGNAL(accepted(Game::GAME_TYPE,Game::PLAYER_TYPE)), this, SLOT(dialogNewIsFinished(Game::GAME_TYPE,Game::PLAYER_TYPE)) );
    dialog_new_game->show();
}

void Dama::dialogNewIsFinished(Game::GAME_TYPE game_type, Game::PLAYER_TYPE player_white)
{
    Game *game = new Game(this);
    game->setType(Game::GAME);
    game->setGame(game_type, player_white);
    ui->tabWidget->addTab(game, QString::fromUtf8("New Game") );
}

void Dama::on_actionLoad_triggered()
{
    NewGame *dialog_load_game = new NewGame(this);
    dialog_load_game->setWindowTitle( tr("Load Game") );
    QObject::connect( dialog_load_game, SIGNAL(accepted(Game::GAME_TYPE,Game::PLAYER_TYPE)), this, SLOT(dialogLoadIsFinished(Game::GAME_TYPE,Game::PLAYER_TYPE)) );
    dialog_load_game->show();
}

void Dama::dialogLoadIsFinished(Game::GAME_TYPE game_type, Game::PLAYER_TYPE player_white)
{
    QString fileName = QFileDialog::getOpenFileName( this, tr("Open File"), "", tr("Dama2013 (*.txt *.xml)") );

    if ( fileName.isEmpty() ) {
        return;
    }

    Game *game = new Game(this);
    game->setType(Game::GAME);
    try {
        game->load(fileName);
        game->redrawBoard();
        game->setGame(game_type, player_white);
        ui->tabWidget->addTab(game, QString::fromUtf8("Loaded Game") );
    }
    catch (std::runtime_error e) {
        game->close();
        QMessageBox::information( this, tr("Warning"),
                                  tr("%1").arg(e.what()) );
    }
}

void Dama::on_actionLoadReplay_triggered()
{
    QString fileName = QFileDialog::getOpenFileName( this, tr("Open File"), "", tr("Dama2013 (*.txt *.xml)") );

    if ( fileName.isEmpty() ) {
        return;
    }

    Game *game = new Game(this);
    QObject::connect( game, SIGNAL(replayEnded()), this, SLOT(on_actionPause_triggered()) );
    game->setType(Game::REPLAY);
    game->load(fileName);
    game->initToReplay();
    ui->tabWidget->addTab(game, QString::fromUtf8("Replay") );
}

void Dama::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save File"),
                                                     "", tr("Dama2013-TXT (*.txt);;Dama2013-XML (*.xml)") );
    if ( fileName.isEmpty() ) {
        return;
    }

    Game *game = (Game *) ui->tabWidget->currentWidget();
    game->save(fileName);
}

void Dama::on_actionAbout_game_triggered()
{
    About about;
    about.exec();
}

void Dama::on_actionAbort_triggered()
{
    SureAbout confirm_abort;
    int result = confirm_abort.exec();
    if (result == QDialog::Accepted) {
        ui->tabWidget->removeTab( ui->tabWidget->currentIndex() );
    }
}

void Dama::closeTab(int tab_id)
{
    SureAbout confirm_abort;
    int result = confirm_abort.exec();
    if (result == QDialog::Accepted) {
        ui->tabWidget->removeTab( tab_id );
    }
}

void Dama::on_tabWidget_currentChanged(QWidget *arg1)
{
    on_actionPause_triggered();

    if ( ui->tabWidget->count() <= 0 ) {
        ui->actionSave->setEnabled(false);
        ui->actionAbort->setEnabled(false);
        ui->actionHint->setEnabled(false);

        ui->replayToolBar->setEnabled(false);
        ui->replayToolBar->setVisible(false);
        return;
    }

    Game *game = (Game *) arg1;
    if ( game->getType() == Game::REPLAY ) {
        ui->actionSave->setEnabled(false);
        ui->actionAbort->setEnabled(true);
        ui->actionHint->setEnabled(false);

        setReplayCurTurn();
        ui->replayToolBar->setEnabled(true);
        ui->replayToolBar->setVisible(true);
    }
    else {
        ui->actionSave->setEnabled(true);
        ui->actionAbort->setEnabled(true);
        ui->actionHint->setEnabled(true);

        ui->replayToolBar->setEnabled(false);
        ui->replayToolBar->setVisible(false);
    }

    return;
}

void Dama::on_actionBack_triggered()
{
    Game *game = (Game *) ui->tabWidget->currentWidget();
    game->replayBack();
    setReplayCurTurn();
}

void Dama::on_actionNext_triggered()
{
    Game *game = (Game *) ui->tabWidget->currentWidget();
    game->replayStep();
    setReplayCurTurn();
}

void Dama::timeoutReplayTimer()
{
    Game *game = (Game *) ui->tabWidget->currentWidget();
    uint cur = game->getReplayNumCurTurn();
    uint upto = (uint) replay_upto->value();

    if ( upto == 0 || cur < upto ) {
        on_actionNext_triggered();
        if (cur+1 == upto) {
            on_actionPause_triggered();
        }
    } else {
        on_actionPause_triggered();
    }
}

void Dama::on_actionHint_triggered()
{
    Game *game = (Game *) ui->tabWidget->currentWidget();
    game->getHint();
}

void Dama::on_actionPlay_auto_triggered()
{
    ui->actionPlay_auto->setEnabled(false);
    replay_delay->setEnabled(false);
    replay_upto->setEnabled(false);

    int time = QTime().msecsTo( replay_delay->time() );

    replay_timer->start( time );
}

void Dama::on_actionPause_triggered()
{
    replay_timer->stop();

    ui->actionPlay_auto->setEnabled(true);
    replay_delay->setEnabled(true);
    replay_upto->setEnabled(true);
}

void Dama::setReplayCurTurn()
{
    Game *game = (Game *) ui->tabWidget->currentWidget();
    uint cur = game->getReplayNumCurTurn();
    uint end = game->getReplayNumEndTurn();
    replay_upto->setMaximum(end);
    replay_cur_turn->setText( QString().setNum(cur) + QString("/") + QString().setNum(end) );
}

void Dama::on_actionBack_to_origin_triggered()
{
    Game *game = (Game *) ui->tabWidget->currentWidget();
    game->initToReplay();
    setReplayCurTurn();
}
