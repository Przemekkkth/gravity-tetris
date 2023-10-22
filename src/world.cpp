#include "world.h"
#include "utils.h"
#include "piece.h"
#include "gamescene.h"
#include <QGraphicsRectItem>
#include <QRandomGenerator>
#include <QPen>
#include <QBrush>

World::World(int size_x, int size_y)
  : m_debugDraw(nullptr), m_physical_world(b2Vec2(0.f, .00015f)), m_x(size_x), m_y(size_y)
{
  createWall(0,0, Piece::SIZE, m_y*Piece::SIZE);
  createWall(Piece::SIZE*(m_x+1.2),0,Piece::SIZE,m_y*Piece::SIZE);
  createWall(0,Piece::SIZE*m_y,Piece::SIZE*(m_x+2.2),Piece::SIZE);
  m_physical_world.SetDebugDraw(&m_debugDraw);

  //  //SoundEffects
  m_explosionSE = new QSoundEffect();
  m_explosionSE->setSource(QUrl("qrc:/res/sound/explosion.wav"));
  m_explosionSE->setVolume(0.75f);

  m_spawnSE = new QSoundEffect();
  m_spawnSE->setSource(QUrl("qrc:/res/sound/spawn.wav"));
  m_spawnSE->setVolume(0.75f);
}

World::~World()
{
  delete m_explosionSE;
  delete m_spawnSE;
  for (b2Body* body=m_physical_world.GetBodyList(); body!=nullptr;)
  {
    b2Body* next = body->GetNext();
    if (body->GetType() == b2_dynamicBody)
    {
      Piece* piece = reinterpret_cast<Piece*>(body->GetUserData().pointer);
      if(piece)
      {
        delete piece;
      }
    }
    else
    {
      QGraphicsRectItem* rI = reinterpret_cast<QGraphicsRectItem*>(body->GetUserData().pointer);
      if (rI)
      {
        delete rI;
      }
    }
    body = next;
  }
}

void World::update()
{
  for (b2Body* body=m_physical_world.GetBodyList(); body!=nullptr; body=body->GetNext())
  {
    if (body->GetType() == b2_dynamicBody)
    {
      Piece* piece = reinterpret_cast<Piece*>(body->GetUserData().pointer);
      if (piece)
      {
        piece->update();
        if(!piece->isEnabled())
        {
          piece->moveY();
        }
      }
    }
  }
}

class _AABB_callback : public b2QueryCallback
{
public :
  QList<b2Fixture*> fixtures;

  virtual bool ReportFixture(b2Fixture* fixture) override
  {
    if (fixture->GetBody()->GetType() == b2_dynamicBody){
      fixtures.push_back(fixture);
    }
    return true;
  }
};

void World::updatePhysics()
{
  //1000/60
  m_physical_world.Step(1000/SETTINGS::FPS,8,3);
}

Piece *World::newPiece()
{
  m_spawnSE->play();
  int type  = QRandomGenerator::global()->bounded(0,Piece::TetriminoTypes::TYPE_SIZE-1);
  int angle = QRandomGenerator::global()->bounded(0,360);
  return new Piece(m_physical_world,m_x/2*Piece::SIZE, Piece::SIZE,static_cast<Piece::TetriminoTypes>(type),angle);
}

int World::clearLines(bool &del, const Piece &current)
{
  int nb_lines = 0;
  _AABB_callback callback;
  del = false;

  for (int y=0;y<=m_y;++y)
  {
    b2AABB aabb;

    aabb.lowerBound = b2Vec2(CONVERTER::pixelsToMeters(0),CONVERTER::pixelsToMeters((y+0.49)*Piece::SIZE));
    aabb.upperBound = b2Vec2(CONVERTER::pixelsToMeters(m_x*Piece::SIZE),CONVERTER::pixelsToMeters((y+0.51)*Piece::SIZE));

    m_physical_world.QueryAABB(&callback,aabb);

    if ((int)callback.fixtures.size() >= m_x)
    {
      for (b2Fixture* fixture : callback.fixtures)
      {
        b2Body* body = fixture->GetBody();
        del |= body == current.getBody();
        if (body->GetFixtureList()->GetNext() != nullptr)
        {
          QGraphicsPolygonItem* shape = reinterpret_cast<QGraphicsPolygonItem*>(fixture->GetUserData().pointer);
          body->DestroyFixture(fixture);
          if (shape){
            delete shape;
            shape = nullptr;
          }
        }
        else
        {
          Piece* piece = reinterpret_cast<Piece*>(body->GetUserData().pointer);
          if (piece)
          {
            delete piece;
            piece = nullptr;
          }
        }

        fixture = nullptr;
      }
      ++nb_lines;
    }
    callback.fixtures.clear();
  }
  if(nb_lines > 0){
    m_explosionSE->play();
  }
  return nb_lines;
}

void World::updateGravity(int level)
{
  //original gravity b2Vec2(0.f, .00015f)
  m_physical_world.SetGravity(b2Vec2(0.0f, .00015f+(level*.00007f)));
}

bool World::isGameOver() const
{
  for (const b2Body* body=m_physical_world.GetBodyList(); body!=nullptr; body=body->GetNext())
  {
    if (body->GetType() == b2_staticBody){
      continue;
    }
    if (body->GetPosition().y < 0){
      return true;
    }
  }
  return false;
}

void World::draw(GameScene &gs)
{
  for (const b2Body* body=m_physical_world.GetBodyList(); body!=nullptr; body=body->GetNext())
  {
    if(body->GetType() == b2_dynamicBody)
    {
      Piece* piece = reinterpret_cast<Piece*>(body->GetUserData().pointer);
      if(piece)
      {
        piece->draw(gs);
      }
    }
    else
    {
      QGraphicsRectItem* rI = reinterpret_cast<QGraphicsRectItem*>(body->GetUserData().pointer);
      if(rI)
      {
        QGraphicsRectItem* newItem = new QGraphicsRectItem();
        newItem->setRect(rI->rect());
        newItem->setPos(rI->pos());
        newItem->setTransformOriginPoint(rI->transformOriginPoint());
        newItem->setPen(rI->pen());
        newItem->setBrush(rI->brush());
        gs.addItem(newItem);
      }
    }
  }
}

void World::drawDebug()
{
  m_physical_world.DebugDraw();
}

void World::createWall(int pos_x, int pos_y, int size_x, int size_y)
{
  b2BodyDef bodyDef;
  bodyDef.position.Set(CONVERTER::pixelsToMeters(pos_x), CONVERTER::pixelsToMeters(pos_y));
  bodyDef.type = b2_staticBody;

  b2PolygonShape b2shape;
  float sx = CONVERTER::pixelsToMeters(size_x)/2.0;
  float sy = CONVERTER::pixelsToMeters(size_y)/2.0;
  b2shape.SetAsBox(sx,sy
                   ,b2Vec2(sx,sy),0);


  b2FixtureDef fixtureDef;
  fixtureDef.density = 1.0;
  fixtureDef.friction = 0.8;
  fixtureDef.restitution= 0.1;
  fixtureDef.shape = &b2shape;

  b2Body* body = m_physical_world.CreateBody(&bodyDef);
  body->CreateFixture(&fixtureDef);
  QGraphicsRectItem* shape = new QGraphicsRectItem(-size_x/2.0, -size_y/2.0, size_x, size_y);
  shape->setTransformOriginPoint(QPointF(0.0f, 0.0f));
  shape->setPos(QPointF(pos_x+size_x/2.0,pos_y+size_y/2.0));
  shape->setPen(QPen(QColor(50,50,50)));
  shape->setBrush(QBrush(QColor(50,50,50)));
  intptr_t pointer = reinterpret_cast<intptr_t>(shape);
  body->GetUserData().pointer = pointer;
}

