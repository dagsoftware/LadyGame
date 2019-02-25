/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "stroke.h"
#include "stone.h"

class Server: public QObject
{
    Q_OBJECT
public:
    Server(QObject * parent = 0);
    ~Server();

    QString getIP();
    QString getPort();
    void sendInit(Stone::COLOUR my_colour, const std::vector<S_Stroke> &record);
    void sendMove(const S_Stroke &stroke);
    void sendEndConn();

    S_Stroke recv_stroke;

Q_SIGNALS:
    void receivedStroke();
    void connClosed();

public slots:
    void acceptConnection();
    void startRead();
private:
    QTcpServer server;
    QTcpSocket* client;
};

#endif // SERVER_H
