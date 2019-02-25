/**
 * Dáma 2013
 *
 * @author Doležal Jan    <xdolez52@stud.fit.vutbr.cz>
 * @author Ďanovský Ján   <xdanov01@stud.fit.vutbr.cz>
 */
#include <iostream>
#include <QHostAddress>
#include "client.h"

Client::Client(QObject* parent): QObject(parent)
{
    connect( &client, SIGNAL(readyRead()),
             this, SLOT(startRead()) );
}

Client::~Client()
{
    client.close();
}

void Client::start(QString address, quint16 port)
{
    QHostAddress addr(address);
    client.connectToHost(addr, port);
}
void Client::startRead()
{
    QByteArray block;
    block = client.read(client.bytesAvailable());
    std::cout << block.constData() << std::endl;
    switch (block[0]) {
    case 1:
        recv_stroke.from = S_Coord(block[2], block[1]);
        recv_stroke.to = S_Coord(block[4], block[3]);
        emit receivedStroke();
        break;
    case 2:
        my_colour = (block[1]) ? (Stone::WHITE) : (Stone::BLACK);
        for (int i = 0; i < block[2]; ++i) {
            // od block[3] a výše jsou data
            uint b = i*4 + 3;
            S_Coord from = S_Coord(block[b+1], block[b]);
            S_Coord to   = S_Coord(block[b+3], block[b+2]);
            recv_record.push_back( S_Stroke(from, to) );
        }
        emit receivedInitData();
        break;
    case 3:
        client.close(); // TODO FIXME BUG
        emit connClosed();
        break;
    default:
        break;
    }
}

void Client::sendMove(const S_Stroke &stroke)
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
    client.write(block);
}

void Client::sendEndConn()
{
    QByteArray block;
    // Typ zprávy.
    block.append( 3 );
    client.write(block);
}
