/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef SURE_ABOUT_H
#define SURE_ABOUT_H

#include <QDialog>

namespace Ui {
class SureAbout;
}

class SureAbout : public QDialog
{
    Q_OBJECT
    
public:
    explicit SureAbout(QWidget *parent = 0);
    ~SureAbout();
    
private:
    Ui::SureAbout *ui;
};

#endif // SURE_ABOUT_H
