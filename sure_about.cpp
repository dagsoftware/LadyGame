/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include "sure_about.h"
#include "ui_sure_about.h"

SureAbout::SureAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SureAbout)
{
    ui->setupUi(this);
}

SureAbout::~SureAbout()
{
    delete ui;
}
