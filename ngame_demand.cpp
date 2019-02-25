/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include "ngame_demand.h"
#include "ui_ngame_demand.h"

NetGameDemand::NetGameDemand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetGameDemand)
{
    ui->setupUi(this);
}

NetGameDemand::~NetGameDemand()
{
    delete ui;
}
