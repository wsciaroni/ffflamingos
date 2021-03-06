#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include <QDialog>
#include "playermodel.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QtGui>
#include <QDebug>
#include <QPen>
#include <QBrush>
#include "ball.h"

namespace Ui {
class GameDialog;
}

/**
 * Window that houses the game play.
 * Used on both the Host and the Guest to display the map, info, and such in the
 * game.
 */
class GameDialog : public QDialog {
  Q_OBJECT

 public:
  /**
   * Constructor used to dynamically allocate memory
   * @param parent
   */
  explicit GameDialog(QWidget *parent = nullptr);

  /**
   * Destructor used to deallocate memory
   */
  ~GameDialog();

  /**
   * Draws the board onto the dialog
   */
  void drawBoard();

  /**
   * Extends the given players head
   * @param PlayerPosition to retract
   */
  void extendHead(int playerPos);

  /**
   * Retracts the given players head
   * @param PlayerPosition to retract
   */
  void retractHead(int playerPos);

  /**
  * Place the given player on the board
  * @param PlayerPosition to place
  */
  void spawnPlayer(int playerPos);

  /**
   * Calls spawn player for each player
   */
  void spawnAllPlayers();

  /**
   * Used to draw the balls in the appropriate places on the GameDialog
   * @param xPos Array of X coordinates of each ball
   * @param yPos Array of Y coordinates of each ball
   */
  void setBallPos(qint32 xPos[25], qint32 yPos[25]);

  /**
   * Used to set the scores and time on the dialog
   * @param scores Array of the scores with 0 being the high score and 1-5 being
   * player scores
   * @param timeRemaining Number of seconds remaining in the game.
   */
  void updateInfo(qint32 scores[6], qint32 timeRemainingValue);

  /**
   * Used to set the names of each player
   */
  void setNames(QString names[6]);

  /**
   * Determines what balls are in collision with the specific player
   */
  bool *determineCapturedBalls(PlayerPosition pos);

 private:
  Ui::GameDialog *ui;
  QGraphicsScene *scene;
  QGraphicsEllipseItem *gameBoard;

  QGraphicsItem *player1Body;
  QGraphicsItem *player1Head;
  QGraphicsItem *player2Body;
  QGraphicsItem *player2Head;
  QGraphicsItem *player3Body;
  QGraphicsItem *player3Head;
  QGraphicsItem *player4Body;
  QGraphicsItem *player4Head;
  QGraphicsItem *player5Body;
  QGraphicsItem *player5Head;
  ball *gameBalls[25];
  QGraphicsItem *hiddenSquare;

  QGraphicsTextItem *timeRemainingLabel;
  QGraphicsTextItem *timeRemaining;

  QGraphicsTextItem *highScore;
  QGraphicsTextItem *player1Score;
  QGraphicsTextItem *player2Score;
  QGraphicsTextItem *player3Score;
  QGraphicsTextItem *player4Score;
  QGraphicsTextItem *player5Score;

  QGraphicsTextItem *highScoreName;
  QGraphicsTextItem *player1Name;
  QGraphicsTextItem *player2Name;
  QGraphicsTextItem *player3Name;
  QGraphicsTextItem *player4Name;
  QGraphicsTextItem *player5Name;

  QPointF head1_retracted;
  QPointF head2_retracted;
  QPointF head3_retracted;
  QPointF head4_retracted;
  QPointF head5_retracted;

  QPointF head1_extended;
  QPointF head2_extended;
  QPointF head3_extended;
  QPointF head4_extended;
  QPointF head5_extended;

 protected:
  /**
   * @brief A Key Press Event handler (will be used for escape)
   * @param key
   */
  void keyPressEvent(QKeyEvent *key);

signals:
  /**
   * Informs FlowChamp that the user has pressed the space bar
   */
  void GDSpacePressed();

  /**
   * Informs FlowChamp that the user has pressed the escape key
   */
  void GDEscPressed();

  /**
   * Informs FlowChamp that the user desires to leave the game
   */
  void GDQuitGame();

 public
slots:
  /**
   * Slot to be triggered when
   */
  void HandleInfoIn(QString highScoreHolder, qint32 highScore,
                    QString winnerName, qint32 winnerScore);
};

#endif  // GAMEDIALOG_H
