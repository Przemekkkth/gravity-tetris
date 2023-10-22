#ifndef VIEW_H
#define VIEW_H
#include <QGraphicsView>
#include <QSoundEffect>
#include "gamescene.h"

class View : public QGraphicsView
{
  Q_OBJECT
public:
  explicit View();

signals:

private:
  GameScene* m_gameScene;
  QSoundEffect *m_bgMusic;
protected:
  virtual void keyReleaseEvent(QKeyEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // VIEW_H
