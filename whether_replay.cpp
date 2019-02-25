/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include "whether_replay.h"
#include "ui_whether_replay.h"

WhetherReplay::WhetherReplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WhetherReplay)
{
    ui->setupUi(this);
}

WhetherReplay::~WhetherReplay()
{
    delete ui;
}
