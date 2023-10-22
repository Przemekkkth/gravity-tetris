#include "view.h"
#include <QKeyEvent>
#include <QApplication>

View::View()
  : m_gameScene(new GameScene(this))
{
  setScene(m_gameScene);
  resize(m_gameScene->sceneRect().width()+2, m_gameScene->sceneRect().height()+2);

  setMouseTracking(true);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  m_bgMusic = new QSoundEffect(this);
  m_bgMusic->setSource(QUrl("qrc:/res/music/theme.wav"));
  m_bgMusic->setLoopCount(QSoundEffect::Infinite);
  m_bgMusic->setVolume(0.5f);
  m_bgMusic->play();
}

void View::keyReleaseEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Escape)
  {
    QApplication::instance()->quit();
  }
  else if(event->key() == Qt::Key_P)
  {
    m_bgMusic->setMuted(!m_bgMusic->isMuted());
  }
  QGraphicsView::keyReleaseEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
  QGraphicsView::mouseMoveEvent(event);
}
