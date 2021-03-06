#include "playerlist.h"

PlayerList::PlayerList() {}

PlayerList::~PlayerList() {
  for (PlayerModel* temp : playerList.keys()) {
    if (temp) {
      if (temp->getTCPSocket()) {
        temp->getTCPSocket()->close();
      }
      delete temp;
    }
  }
}

bool PlayerList::addPlayer(PlayerModel* playerModel) {
  if (playerList.contains(playerModel)) {
    // Return false, because we don't want to add a player twice
    return false;
  } else {
    playerList.insert(playerModel, playerModel->getUID());
    return true;  // Successful insertion
  }
}

bool PlayerList::removePlayer(PlayerModel* playerModel) {
  if (playerList.contains(playerModel)) {
    playerList.remove(playerModel);
    return true;  // Successful removal
  } else {
    return false;  // Unsuccessful removal
  }
}

bool PlayerList::removePlayer(int uid) {
  if (playerList.key(uid) && playerList.contains(playerList.key(uid))) {
    playerList.remove(playerList.key(uid));  // Remove the key
    return true;                             // Successfully found and removed
  } else {
    return false;  // If unsuccessful (not found) return false
  }
}

PlayerModel* PlayerList::getPlayer(int uid) {
  return playerList.key(uid);  // Return the player model with uid
}

PlayerModel* PlayerList::getPlayer(QTcpSocket* socket) {
  for (PlayerModel* temp : playerList.keys()) {
    if (temp->getTCPSocket() == socket) {
      return temp;
    }
  }
  return nullptr;
}

int PlayerList::getNewPlayerID() {
  for (int i = 1; i < 6; i++) {
    if (!getPlayer(i))  // If the player does not exist yet, return that number
    {
      return i;
    }
  }
  return -1;
}

PlayerModel* PlayerList::getPlayerByPosition(PlayerPosition position) {
  // Iterate through the map, and if we find a position return it.

  for (PlayerModel* temp : playerList.keys()) {
    if (temp->getPositionId() == position) {
      return temp;  // Return the player at the given position
    }
  }
  return nullptr;  // If no player at the given position, return null
}

int PlayerList::getPlayerCount() {
  return playerList.keys().length();  // Return the current number of keys
}

int PlayerList::getMaxUID() {
  int max = 0;
  for (PlayerModel* temp : playerList.keys()) {
    if (temp->getUID() > max) {
      max = temp->getUID();
    }
  }
  return max;
}
void PlayerList::setPositionsAndStartGame(HostNetworkHandler* handlerIn) {
  int pos = 2;
  connect(this, &PlayerList::sendStartGame, handlerIn,
          &HostNetworkHandler::sendWelcomeToRoom);
  for (PlayerModel* temp : playerList.keys()) {
    if (temp->getUID() != 0) {
      temp->setPositionId(static_cast<PlayerPosition>(pos));
      NPWelcomeToRoom packet;
      packet.setPositionId(pos);
      emit this->sendStartGame(packet, temp->getTCPSocket());
    } else {
      temp->setPositionId(static_cast<PlayerPosition>(1));
    }
  }
}
