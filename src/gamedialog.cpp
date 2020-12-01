#include "gamedialog.h"
#include "ui_gamedialog.h"
#include <QtMath>
#include <QGraphicsTextItem>

GameDialog::GameDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::GameDialog) {
  ui->setupUi(this);

  window()->setFixedSize(800, 800);  // No resizing
}

GameDialog::~GameDialog() { delete ui; }

void GameDialog::keyPressEvent(QKeyEvent* key) {
  if (key->key() == Qt::Key_Escape) {
    emit this->GDEscPressed();
  } else if (key->key() == Qt::Key_Space) {
    emit this->GDSpacePressed();
  }
}

void GameDialog::drawBoard() {

  scene = new QGraphicsScene(this);
  scene->setItemIndexMethod(QGraphicsScene::NoIndex);
  // scene->setSceneRect(0, 0, 400, 400);
  scene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

  ui->graphicsView->setScene(scene);
  ui->graphicsView->setRenderHint(QPainter::Antialiasing);

  // Create ellipse
  QPen ellipsePen(Qt::red);
  ellipsePen.setWidth(10);

  gameBoard =
      scene->addEllipse(-300, -300, 600, 600, ellipsePen, QBrush(Qt::gray));

  // Lower Left
  player1Body =
      scene->addRect(-10, -290, 20, 150, QPen(Qt::black), QBrush(Qt::magenta));
  player1Head =
      scene->addRect(-25, -190, 50, 50, QPen(Qt::black), QBrush(Qt::magenta));
  player1Body->setRotation(-144);
  player1Head->setRotation(-144);
  player1Body->setVisible(false);
  player1Head->setVisible(false);

  // Upper Left
  player2Body =
      scene->addRect(-10, -290, 20, 150, QPen(Qt::black), QBrush(Qt::cyan));
  player2Head =
      scene->addRect(-25, -190, 50, 50, QPen(Qt::black), QBrush(Qt::cyan));
  player2Body->setRotation(-72);
  player2Head->setRotation(-72);
  player2Body->setVisible(false);
  player2Head->setVisible(false);

  // Center
  player3Body =
      scene->addRect(-10, -290, 20, 150, QPen(Qt::black), QBrush(Qt::yellow));
  player3Head =
      scene->addRect(-25, -190, 50, 50, QPen(Qt::black), QBrush(Qt::yellow));
  player3Body->setVisible(false);
  player3Head->setVisible(false);

  // Upper Right
  player4Body =
      scene->addRect(-10, -290, 20, 150, QPen(Qt::black), QBrush(Qt::green));
  player4Head =
      scene->addRect(-25, -190, 50, 50, QPen(Qt::black), QBrush(Qt::green));
  player4Body->setRotation(72);
  player4Head->setRotation(72);
  player4Body->setVisible(false);
  player4Head->setVisible(false);

  // Lower Right
  player5Body =
      scene->addRect(-10, -290, 20, 150, QPen(Qt::black), QBrush(Qt::blue));
  player5Head =
      scene->addRect(-25, -190, 50, 50, QPen(Qt::black), QBrush(Qt::blue));
  player5Body->setRotation(144);
  player5Head->setRotation(144);
  player5Body->setVisible(false);
  player5Head->setVisible(false);

  // initializing balls
  for (int i = 0; i < 25; i++) {
    gameBalls[i] = new ball;
    gameBalls[i]->setPos(0, 0);
    scene->addItem(gameBalls[i]);
  }

  timeRemainingLabel = new QGraphicsTextItem;
  timeRemaining = new QGraphicsTextItem;

  highScore = new QGraphicsTextItem;
  player1Score = new QGraphicsTextItem;
  player2Score = new QGraphicsTextItem;
  player3Score = new QGraphicsTextItem;
  player4Score = new QGraphicsTextItem;
  player5Score = new QGraphicsTextItem;

  highScoreName = new QGraphicsTextItem;
  player1Name = new QGraphicsTextItem;
  player2Name = new QGraphicsTextItem;
  player3Name = new QGraphicsTextItem;
  player4Name = new QGraphicsTextItem;
  player5Name = new QGraphicsTextItem;

  timeRemainingLabel->setPos(-75, -425);
  timeRemainingLabel->setDefaultTextColor("red");
  timeRemainingLabel->setPlainText("TIME REMAINING:");

  timeRemaining->setPos(45, -425);
  timeRemaining->setDefaultTextColor("red");
  timeRemaining->setPlainText("60");

  highScoreName->setPos(-325, -425);
  highScoreName->setDefaultTextColor("red");
  highScoreName->setPlainText("HIGH SCORE:");

  player1Name->setPos(-325, -400);
  player1Name->setDefaultTextColor("magenta");
  player1Name->setPlainText("Player 1:");

  player2Name->setPos(-325, -375);
  player2Name->setDefaultTextColor("cyan");
  player2Name->setPlainText("Player 2:");

  player3Name->setPos(-325, -350);
  player3Name->setDefaultTextColor("yellow");
  player3Name->setPlainText("Player 3:");

  player4Name->setPos(-325, -325);
  player4Name->setDefaultTextColor("green");
  player4Name->setPlainText("Player 4:");

  player5Name->setPos(-325, -300);
  player5Name->setDefaultTextColor("blue");
  player5Name->setPlainText("Player 5:");

  highScore->setPos(-240, -425);
  highScore->setDefaultTextColor("red");
  highScore->setPlainText("0");

  player1Score->setPos(-240, -400);
  player1Score->setDefaultTextColor("magenta");
  player1Score->setPlainText("0");

  player2Score->setPos(-240, -375);
  player2Score->setDefaultTextColor("cyan");
  player2Score->setPlainText("0");

  player3Score->setPos(-240, -350);
  player3Score->setDefaultTextColor("yellow");
  player3Score->setPlainText("0");

  player4Score->setPos(-240, -325);
  player4Score->setDefaultTextColor("green");
  player4Score->setPlainText("0");

  player5Score->setPos(-240, -300);
  player5Score->setDefaultTextColor("blue");
  player5Score->setPlainText("0");

  scene->addItem(timeRemainingLabel);
  scene->addItem(timeRemaining);
  scene->addItem(highScore);
  scene->addItem(player1Score);
  scene->addItem(player2Score);
  scene->addItem(player3Score);
  scene->addItem(player4Score);
  scene->addItem(player5Score);
  scene->addItem(highScoreName);
  scene->addItem(player1Name);
  scene->addItem(player2Name);
  scene->addItem(player3Name);
  scene->addItem(player4Name);
  scene->addItem(player5Name);

  qDebug() << "Drawing Board" << endl;
}

void GameDialog::extendHead(int playerPos) {
  // qDebug() << "Extending " << playerPos << endl;
  switch (playerPos) {
    case 0:
      break;
    case 1:
      player1Head->moveBy(-50 * qSin(-144 * M_PI / 180),
                          50 * qCos(-144 * M_PI / 180));
      break;
    case 2:
      player2Head->moveBy(-50 * qSin(-72 * M_PI / 180),
                          50 * qCos(-72 * M_PI / 180));
      break;
    case 3:
      player3Head->moveBy(0, 50);
      break;
    case 4:
      player4Head->moveBy(-50 * qSin(72 * M_PI / 180),
                          50 * qCos(72 * M_PI / 180));
      break;
    case 5:
      player5Head->moveBy(-50 * qSin(144 * M_PI / 180),
                          50 * qCos(144 * M_PI / 180));
      break;
    default:
      break;
  }
}

void GameDialog::retractHead(int playerPos) {
  // qDebug() << "Retracting " << playerPos << endl;
  switch (playerPos) {
    case 0:
      break;
    case 1:
      player1Head->moveBy(50 * qSin(-144 * M_PI / 180),
                          -50 * qCos(-144 * M_PI / 180));
      break;
    case 2:
      player2Head->moveBy(50 * qSin(-72 * M_PI / 180),
                          -50 * qCos(-72 * M_PI / 180));
      break;
    case 3:
      player3Head->moveBy(0, -50);
      break;
    case 4:
      player4Head->moveBy(50 * qSin(72 * M_PI / 180),
                          -50 * qCos(72 * M_PI / 180));
      break;
    case 5:
      player5Head->moveBy(50 * qSin(144 * M_PI / 180),
                          -50 * qCos(144 * M_PI / 180));
      break;
    default:
      break;
  }
}

void GameDialog::spawnPlayer(int playerPos) {
  switch (playerPos) {
    case 0:
      break;
    case 1:
      player1Body->setVisible(true);
      player1Head->setVisible(true);
    case 2:
      player2Body->setVisible(true);
      player2Head->setVisible(true);
    case 3:
      player3Body->setVisible(true);
      player3Head->setVisible(true);
    case 4:
      player4Body->setVisible(true);
      player4Head->setVisible(true);
    case 5:
      player5Body->setVisible(true);
      player5Head->setVisible(true);
    default:
      break;
  }
}

void GameDialog::spawnAllPlayers() {
  for (int i = 1; i <= 5; i++) {
    spawnPlayer(i);
  }
}

void GameDialog::setBallPos(qint32 xPos[25], qint32 yPos[25]) {
  /// @todo draw balls at each coordinate pair.
  for (int i = 0; i < 25; i++) {
    gameBalls[i]->setPos(xPos[i], yPos[i]);
  }
}

void GameDialog::updateInfo(qint32 scores[6], qint32 timeRemainingValue) {
  timeRemaining->setPlainText(QString::number(timeRemainingValue, 10));

  for (int i = 0; i < 6; i++) {
    switch (i) {
      case 0:
        highScore->setPlainText(QString::number(scores[i], 10));
        break;
      case 1:
        player1Score->setPlainText(QString::number(scores[i], 10));
        break;
      case 2:
        player2Score->setPlainText(QString::number(scores[i], 10));
        break;
      case 3:
        player3Score->setPlainText(QString::number(scores[i], 10));
        break;
      case 4:
        player4Score->setPlainText(QString::number(scores[i], 10));
        break;
      case 5:
        player5Score->setPlainText(QString::number(scores[i], 10));
        break;
      default:
        break;
    }
  }
}
void GameDialog::setNames(QString names[6]) {
  for (int i = 0; i < 6; i++) {
    if (names[i] == "") {
      continue;
    } else {
      switch (i) {
        case 0:
          highScoreName->setPlainText(names[i]);
          break;
        case 1:
          player1Name->setPlainText(names[i]);
          break;
        case 2:
          player2Name->setPlainText(names[i]);
          break;
        case 3:
          player3Name->setPlainText(names[i]);
          break;
        case 4:
          player4Name->setPlainText(names[i]);
          break;
        case 5:
          player5Name->setPlainText(names[i]);
          break;
        default:
          break;
      }
    }
  }
}

bool* GameDialog::determineCapturedBalls(PlayerPosition pos) {
  static bool isColliding[25];
  for (int i = 0; i < 25; i++) {
    isColliding[i] = false;
  }

  QGraphicsItem* player = NULL;
  switch (pos) {
    case PlayerPosition::LOWER_LEFT:
      player = player1Head;
      break;
    case PlayerPosition::UPPER_LEFT:
      player = player2Head;
      break;
    case PlayerPosition::CENTER:
      player = player3Head;
      break;
    case PlayerPosition::UPPER_RIGHT:
      player = player4Head;
      break;
    case PlayerPosition::LOWER_RIGHT:
      player = player5Head;
      break;
    default:
      return NULL;
  }
  for (int i = 0; i < 25; i++) {
    isColliding[i] =
        player->collidesWithItem(gameBalls[i], Qt::IntersectsItemShape);
  }
  return isColliding;
}

void GameDialog::HandleInfoIn(/*TBD*/) {}
