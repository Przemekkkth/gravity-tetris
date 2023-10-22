#ifndef WORLD_H
#define WORLD_H
#include <QSoundEffect>
#include <box2d/box2d.h>
#include "debugdraw.h"

class GameScene;
class Piece;
class World
{
public:
  World(int size_x,int size_y);
  ~World();
  void update();
  void updatePhysics();
  Piece* newPiece();
  int clearLines(bool& del,const Piece& current);
  void updateGravity(int level);
  bool isGameOver() const;
  void draw(GameScene& gs);
  void drawDebug();
  DebugDraw m_debugDraw;
private:
  b2World m_physical_world;
  void createWall(int pos_x, int pos_y, int size_x, int size_y);

  const int m_x;
  const int m_y;
  QSoundEffect *m_explosionSE, *m_spawnSE;
};

#endif // WORLD_H
