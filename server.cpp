/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include "server.h"
#include <iostream>

Server::Server(QObject* parent): QObject(parent)
{
    connect( &server, SIGNAL(newConnection()),
             this, SLOT(acceptConnection()) );

    server.listen();
}

Server::~Server()
{
    server.close();
}

QString Server::getIP()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    if (ipAddress.isEmpty()) {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    return ipAddress;
}

QString Server::getPort()
{
    return QString::number( server.serverPort() );
}

void Server::acceptConnection()
{
    client = server.nextPendingConnection();

    connect(client, SIGNAL(readyRead()),
            this, SLOT(startRead()));
}

void Server::startRead()
{
    QByteArray block;
    block = client->read(client->bytesAvailable());
    std::cout << block.constData() << std::endl;
    switch (block[0]) {
    case 1:
        recv_stroke.from = S_Coord(block[2], block[1]);
        recv_stroke.to = S_Coord(block[4], block[3]);
        emit receivedStroke();
        break;
    case 3:
        client->close();
        server.close(); // TODO FIXME BUG
        emit connClosed();
        break;
    default:
        break;
    }
}

void Server::sendInit(Stone::COLOUR my_colour, const std::vector<S_Stroke> &record)
{
    char colour = (my_colour == Stone::WHITE) ? (0) : (1);
    QByteArray block;
    block.append( 2 ); // Typ zprávy.
    block.append( colour ); // Barva hostitele.
    block.append( record.size() ); // Počet záznamů od začátku.
    for (uint i = 0; i < record.size(); ++i) {
        block.append( record[i].from.col );
        block.append( record[i].from.row );
        block.append( record[i].to.col );
        block.append( record[i].to.row );
    }
    client->write(block);
}

void Server::sendMove(const S_Stroke &stroke)
{
    QByteArray block;
    // Typ zprávy.
    block.append( 1 );
    // Pohyb ze souřadnic...
    block.append( stroke.from.col );
    block.append( stroke.from.row );
    // ... na souřadnice.
    block.append( stroke.to.col );
    block.append( stroke.to.row );
    client->write(block);
}

void Server::sendEndConn()
{
    QByteArray block;
    // Typ zprávy.
    block.append( 3 );
    client->write(block);
}
