/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QTcpSocket>

#include "stroke.h"
#include "stone.h"

class Client: public QObject
{
    Q_OBJECT
public:
    Client(QObject* parent = 0);
    ~Client();
    void start(QString address, quint16 port);

    void sendMove(const S_Stroke &stroke);
    void sendEndConn();

    S_Stroke recv_stroke;
    std::vector<S_Stroke> recv_record;
    Stone::COLOUR my_colour;


Q_SIGNALS:
    void receivedStroke();
    void receivedInitData();
    void connClosed();

public slots:
    void startRead();
private:
    QTcpSocket client;
};

#endif // CLIENT_H
