#include "npprovideroomcode.h"
#include <QDebug>

NPProvideRoomCode::NPProvideRoomCode(/* args */)
{
}

NPProvideRoomCode::~NPProvideRoomCode()
{
}

void NPProvideRoomCode::setRoomCode(QString roomcode) {
    roomCode = roomcode;
}

QString NPProvideRoomCode::getRoomCode() {
    return roomCode;
}
