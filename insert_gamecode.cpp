/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include "insert_gamecode.h"
#include "ui_insert_gamecode.h"

InsertGameCode::InsertGameCode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertGameCode)
{
    ui->setupUi(this);
}

InsertGameCode::~InsertGameCode()
{
    delete ui;
}
