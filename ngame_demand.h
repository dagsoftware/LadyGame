/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef NGAME_DEMAND_H
#define NGAME_DEMAND_H

#include <QDialog>

namespace Ui {
class NetGameDemand;
}

class NetGameDemand : public QDialog
{
    Q_OBJECT
    
public:
    explicit NetGameDemand(QWidget *parent = 0);
    ~NetGameDemand();
    
private:
    Ui::NetGameDemand *ui;
};

#endif // NGAME_DEMAND_H
