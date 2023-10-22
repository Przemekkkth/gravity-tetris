#include "gamescene.h"
#include "utils.h"
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QDir>
#include <QPainter>

GameScene::GameScene(QObject *parent)
  : QGraphicsScene(parent), m_currentPiece(nullptr), m_world(10, 20), m_debugEnabled(false)
{
  onUserCreate();
}

GameScene::~GameScene()
{

}

void GameScene::loop()
{
  m_deltaTime = m_elapsedTimer.elapsed();
  m_elapsedTimer.restart();

  m_loopTime += m_deltaTime;
  while(m_loopTime > m_loopSpeed)
  {
    m_loopTime -= m_loopSpeed;
    handlePlayerInput();
    update();
    updatePhysics();
    clear();
    resetStatus();
  }
  draw();
}

void GameScene::onUserCreate()
{
  setBackgroundBrush(COLOR_STYLE::BACKGROUND);
  for(int i = 0; i < 256; ++i)
  {
    m_keys[i] = new KeyStatus();
  }
  m_mouse = new MouseStatus();
  setSceneRect(0,0, SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());
  connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
  m_timer.start(int(1000.0f/FPS));
  m_elapsedTimer.start();

  m_stats.setOffset(Piece::SIZE*(10+3),Piece::SIZE);
  m_currentPiece = m_world.newPiece();
  m_currentPiece->setEnabled(true);

  m_world.m_debugDraw.setGameScene(this);
  m_timeSinceLastFall.start();

  //SoundEffects
  m_levelupSE = new QSoundEffect(this);
  m_levelupSE->setSource(QUrl("qrc:/res/sound/levelup.wav"));
  m_levelupSE->setVolume(0.75f);
}

void GameScene::renderScene()
{
  static int index = 0;
  QString fileName = QDir::currentPath() + QDir::separator() + "screen" + QString::number(index++) + ".png";
  QRect rect = sceneRect().toAlignedRect();
  QImage image(rect.size(), QImage::Format_ARGB32);
  image.fill(Qt::transparent);
  QPainter painter(&image);
  render(&painter);
  image.save(fileName);
  qDebug() << "saved " << fileName;
}

void GameScene::handlePlayerInput()
{
  if(m_keys[KEYBOARD::KeysMapper[Qt::Key_Z]]->m_released)
  {
    renderScene();//uncoment if want to make screenshots
  }
  if(m_keys[KEYBOARD::KeysMapper[Qt::Key_Space]]->m_released)
  {
    m_currentPiece->setEnabled(false);
    m_currentPiece = m_world.newPiece();
    m_currentPiece->setEnabled(true);
    m_timeSinceLastFall.restart();
  }
  else if(m_keys[KEYBOARD::KeysMapper[Qt::Key_A]]->m_held)
  {
    m_moveDirection = -1;
  }
  else if(m_keys[KEYBOARD::KeysMapper[Qt::Key_D]]->m_held)
  {
    m_moveDirection = +1;
  }
  else if(m_keys[KEYBOARD::KeysMapper[Qt::Key_W]]->m_held)
  {
    m_rotateDirection = +1;
  }
  else if(m_keys[KEYBOARD::KeysMapper[Qt::Key_S]]->m_held)
  {
    m_rotateDirection = -1;
  }
  else if(m_keys[KEYBOARD::KeysMapper[Qt::Key_O]]->m_released)
  {
    m_debugEnabled = !m_debugEnabled;
  }
}

void GameScene::resetStatus()
{
  for(int i = 0; i < 256; ++i)
  {
    m_keys[i]->m_released = false;
  }
  for(int i = 0; i < 256; ++i)
  {
    m_keys[i]->m_pressed = false;
  }
  m_mouse->m_released = false;
}

void GameScene::draw()
{
  clear();
  m_world.draw(*this);
  m_stats.draw(*this);
  if(m_debugEnabled)
  {
    clear();
    m_world.drawDebug();
  }
}

void GameScene::update()
{
  if(m_currentPiece != nullptr)
  {
    m_currentPiece->rotate(m_rotateDirection);
    m_currentPiece->moveX(m_moveDirection);

    bool new_piece;

    unsigned int old_level = m_stats.getLevel();
    m_stats.addLines(m_world.clearLines(new_piece, *m_currentPiece));
    if(m_stats.getLevel() != old_level) {
      m_levelupSE->play();
    }

    if(new_piece || float(m_timeSinceLastFall.elapsed()/1000.0f) > std::max(1.0f, 10-m_stats.getLevel()*0.2f))
    {
      m_currentPiece->setEnabled(false);
      m_currentPiece = m_world.newPiece();
      m_currentPiece->setEnabled(true);
      m_timeSinceLastFall.restart();
    }
  }
  m_world.update();
  m_rotateDirection = 0;
  m_moveDirection =   0;
  m_stats.setGameOver(m_world.isGameOver());
}

void GameScene::updatePhysics()
{
  m_world.updateGravity(m_stats.getLevel());
  m_world.updatePhysics();
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
  if(KEYBOARD::KeysMapper.contains(event->key()))
  {
    if(event->isAutoRepeat())
    {
      m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held = true;
      m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = false;
    }
    else
    {
      m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = true;
      m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held    = false;
    }
  }
  QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
  if(KEYBOARD::KeysMapper.contains(event->key()))
  {
    if(!event->isAutoRepeat())
    {
      m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held = false;
      m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = false;
      m_keys[KEYBOARD::KeysMapper[event->key()]]->m_released = true;
    }

  }
  QGraphicsScene::keyReleaseEvent(event);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  m_mouse->m_x = event->scenePos().x();
  m_mouse->m_y = event->scenePos().y();
  m_mouse->m_pressed = true;
  QGraphicsScene::mousePressEvent(event);
}

void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  m_mouse->m_x = event->scenePos().x();
  m_mouse->m_y = event->scenePos().y();
  QGraphicsScene::mouseMoveEvent(event);
}

void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  m_mouse->m_x = event->scenePos().x();
  m_mouse->m_y = event->scenePos().y();
  m_mouse->m_pressed = false;
  m_mouse->m_released = true;
  QGraphicsScene::mouseReleaseEvent(event);
}
