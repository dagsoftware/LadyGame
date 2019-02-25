/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef INSERT_GAMECODE_H
#define INSERT_GAMECODE_H

#include <QDialog>

namespace Ui {
class InsertGameCode;
}

class InsertGameCode : public QDialog
{
    Q_OBJECT
    
public:
    explicit InsertGameCode(QWidget *parent = 0);
    ~InsertGameCode();
    
private:
    Ui::InsertGameCode *ui;
};

#endif // INSERT_GAMECODE_H
