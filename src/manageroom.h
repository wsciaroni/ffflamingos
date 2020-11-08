#ifndef MANAGEROOM_H
#define MANAGEROOM_H

#include "hostmodel.h"
#include "playermodel.h"
#include "gamedialog.h"

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QStringListModel>

namespace Ui {
class ManageRoom;
}

/**
 * Used to have the host see who has joined the game, and allows the host to start the game.
 */
class ManageRoom : public QDialog
{
    Q_OBJECT

public:

    /**
    Constructor
    */
    explicit ManageRoom(QWidget *parent = nullptr);

    /**
    Destructor
    */
    ~ManageRoom();

    void passHost(HostModel* hostPlayer);

private:
    ///ui pointer
    Ui::ManageRoom *ui;

    ///Gameplay Window
    GameDialog* gameWindow;

    ///Holds the list of playernames to be displayed
    QStringList list;

    ///Model for list
    QStringListModel model;

    /// Map to hold user ids of players and their names in the table
    std::list<PlayerModel*> playerList;

private slots:
   /**
   Slot to send user to game play
   @todo Transfers players from wait room to game state.
   */
   void startGame();

   /**
   @todo will forcibly close the lobby, disconnecting all players
   */
   void closeLobby();

public slots:

   /**
   addPlayer should be called when a player has successfully connected
   @param player Pass the player class
   */
   void addPlayer(PlayerModel* player);

   /**
   removePlayer should be called when a player disconnects either via connection drop or self disconnect.
   @param player Pass the player class of the player to remove
   */
   void removePlayer(PlayerModel* player);
};

#endif // MANAGEROOM_H
