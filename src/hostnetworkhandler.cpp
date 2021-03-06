#include "hostnetworkhandler.h"
#include "error.h"
#include <QNetworkProxy>

HostNetworkHandler::HostNetworkHandler(QObject* parent) : QObject(parent) {
  connect(&tcpServer, &QTcpServer::newConnection, this,
          &HostNetworkHandler::onNewTCPConnection);

  udpServer.setProxy(QNetworkProxy::NoProxy);
}

HostNetworkHandler::~HostNetworkHandler() {
  stopTCPServer();

  const QObjectList& list = tcpServer.children();
  for (int i = 0; i < list.length(); i++) {
    QTcpSocket* tcp = dynamic_cast<QTcpSocket*>(list[i]);

    if (tcp != nullptr) {
      tcp->close();
    }
  }
}

bool HostNetworkHandler::startTCPServer(QHostAddress hostAddress, int portIn) {
  if (tcpServer.isListening()) {
    stopTCPServer();
  }
  port = portIn;
  return tcpServer.listen(hostAddress, port);
}

void HostNetworkHandler::stopTCPServer() {
  if (tcpServer.isListening()) {
    tcpServer.close();
  }
}

QTcpServer* HostNetworkHandler::getTCPServer() { return &tcpServer; }

void HostNetworkHandler::onNewTCPConnection() {

  QTcpSocket* tcpSocket =
      tcpServer.nextPendingConnection();  // Pass this tcp socket to the

  tcpSocket->setProxy(QNetworkProxy::NoProxy);

  connect(tcpSocket, &QTcpSocket::connected, this,
          &HostNetworkHandler::onTCPConnected);
  connect(tcpSocket, &QTcpSocket::disconnected, this,
          &HostNetworkHandler::onTCPDisconnected);
  connect(tcpSocket, &QTcpSocket::readyRead, this,
          &HostNetworkHandler::onTCPDataReady);
}

void HostNetworkHandler::onTCPConnected() {
  QTcpSocket* tcpSocket = dynamic_cast<QTcpSocket*>(sender());
  if (tcpSocket != NULL) {
    tcpSocket->setProxy(QNetworkProxy::NoProxy);
  }
}

void HostNetworkHandler::onTCPDisconnected() {
  QTcpSocket* tcpSocket = dynamic_cast<QTcpSocket*>(sender());
  emit this->hostHandleGuestTerminated(tcpSocket);
}

void HostNetworkHandler::onTCPDataReady() {
  QTcpSocket* tcpSocket = dynamic_cast<QTcpSocket*>(sender());
  tcpSocket->setProxy(QNetworkProxy::NoProxy);
  while (tcpSocket->bytesAvailable()) {
    tcpSocket->setProxy(QNetworkProxy::NoProxy);
    // Read from socket here
    PacketType pType;
    BlockReader(tcpSocket).stream() >> pType;
    if (pType == PacketType::PROVIDEROOMCODE) {
      NPProvideRoomCode provideRoomCodePacket;
      QString roomcode;
      int uid;
      QString name;
      BlockReader(tcpSocket).stream() >> uid >> roomcode >> name;
      provideRoomCodePacket.setRoomCode(roomcode);
      provideRoomCodePacket.setUID(uid);
      provideRoomCodePacket.setName(name);
      emit this->provideRoomCode(provideRoomCodePacket, tcpSocket);
    } else if (pType == PacketType::SPACEPRESSED) {
      NPSpacePressed spacePressedPacket;
      int uid;
      BlockReader(tcpSocket).stream() >> uid;
      spacePressedPacket.setUID(uid);
      emit this->spacePressed(spacePressedPacket, tcpSocket);
    } else if (pType == PacketType::NULLPACKETTYPE) {
      // Throw an error
      error* throwError = new error;
      throwError->throwErrorMsg("ERROR: Received a NULL packet type");
      throwError->exec();
      delete throwError;
    }
  }
}

int HostNetworkHandler::getPort() { return port; }

void HostNetworkHandler::sendRoomCodeStatus(NPRoomCodeStatus roomCodeStatus,
                                            QTcpSocket* socket) {
  bool status = roomCodeStatus.getRoomCodeStatus();
  BlockWriter(socket).stream() << PacketType::ROOMCODESTATUS;
  BlockWriter(socket).stream() << status;
}

void HostNetworkHandler::sendWelcomeToRoom(NPWelcomeToRoom welcomeToRoom,
                                           QTcpSocket* socket) {
  BlockWriter(socket).stream() << PacketType::WELCOMETOROOM;
  QString* names = welcomeToRoom.getNames();
  for (int i = 0; i < 6; i++) {
    BlockWriter(socket).stream() << names[i];
  }
}

void HostNetworkHandler::sendInGameInfo(NPInGameInfo inGameInfo,
                                        QTcpSocket* socket) {
  BlockWriter(socket).stream() << PacketType::INGAMEINFO;
  for (int i = 0; i < 25; i++) {
    BlockWriter(socket).stream() << inGameInfo.getBallPosX(i)
                                 << inGameInfo.getBallPosY(i);
  }
  for (int i = 0; i < 6; i++) {
    BlockWriter(socket).stream() << inGameInfo.getPlayerScore(i)
                                 << inGameInfo.isPlayerExtended(i);
  }
  BlockWriter(socket).stream() << inGameInfo.getTimeRemaining();
}

void HostNetworkHandler::sendEndGameInfo(NPEndGameInfo endGameInfo,
                                         QTcpSocket* socket) {
  BlockWriter(socket).stream() << PacketType::ENDGAMEINFO;
  BlockWriter(socket).stream() << endGameInfo.getHighScoreName()
                               << endGameInfo.getHighScore()
                               << endGameInfo.getWinnerName()
                               << endGameInfo.getWinnerScore();
}
