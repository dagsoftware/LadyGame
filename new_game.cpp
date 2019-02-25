/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include "new_game.h"
#include "ui_new_game.h"

NewGame::NewGame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewGame)
{
    ui->setupUi(this);

    ui->yourColor->setEnabled(false);
    ui->yourColorLabel->setEnabled(false);
    ui->buttonBox->buttons()[0]->setEnabled(false);

    connect( ui->single, SIGNAL(clicked()),
             this, SLOT(localTypeChanged()) );
    connect( ui->hotseat, SIGNAL(clicked()),
             this, SLOT(localTypeChanged()) );
}

NewGame::~NewGame()
{
    delete ui;
}

void NewGame::localTypeChanged()
{
    if ( ui->single->isChecked() || ui->hotseat->isChecked() ) {
        ui->buttonBox->buttons()[0]->setEnabled(true);
    } else {
        ui->buttonBox->buttons()[0]->setEnabled(false);
    }

    if ( ui->single->isChecked() ) {
        ui->yourColor->setEnabled(true);
        ui->yourColorLabel->setEnabled(true);
    } else {
        ui->yourColor->setEnabled(false);
        ui->yourColorLabel->setEnabled(false);
    }
}

void NewGame::on_tabWidget_currentChanged(int index)
{
    if (index == 0) {
        localTypeChanged();
    } else {
        ui->buttonBox->buttons()[0]->setEnabled(false);
    }
}

void NewGame::on_buttonBox_accepted()
{
    if ( ui->single->isChecked() ) {
        if ( ui->yourColor->currentIndex() == 0 ) {
            Q_EMIT accepted(Game::SINGLE, Game::HOST);
        } else {
            Q_EMIT accepted(Game::SINGLE, Game::PC);
        }
    }
    else if ( ui->hotseat->isChecked() ) {
        Q_EMIT accepted(Game::HOTSEAT, Game::HOST);
    }
    else if ( ui->host->isChecked() ) {
        Q_EMIT accepted(Game::SERVER, Game::HOST);
    }
    else if ( ui->join->isChecked() ) {
        Q_EMIT accepted(Game::CLIENT, Game::HOST);
    }

}
