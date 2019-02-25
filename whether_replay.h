/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef WHETHER_REPLAY_H
#define WHETHER_REPLAY_H

#include <QDialog>

namespace Ui {
class WhetherReplay;
}

class WhetherReplay : public QDialog
{
    Q_OBJECT
    
public:
    explicit WhetherReplay(QWidget *parent = 0);
    ~WhetherReplay();
    
private:
    Ui::WhetherReplay *ui;
};

#endif // WHETHER_REPLAY_H
