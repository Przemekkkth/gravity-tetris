#include "piece.h"
#include "utils.h"
#include "gamescene.h"
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QBrush>
#include <QPen>

const int Piece::SIZE = 32;
const int Piece::SIZE_2 = SIZE/2;
const QColor Piece::COLORS[Piece::TetriminoTypes::TYPE_SIZE]= {
  Qt::blue,
  Qt::red,
  Qt::green,
  Qt::cyan,
  Qt::magenta,
  Qt::white,
  QColor(195,132,58)
};

Piece::Piece(b2World &world, int pos_x, int pos_y, TetriminoTypes type, float rotation)
  : _world(world), m_enabled(false)
{
  b2BodyDef bodyDef;

  bodyDef.position.Set(CONVERTER::pixelsToMeters(pos_x), CONVERTER::pixelsToMeters(pos_y));
  bodyDef.type = b2_dynamicBody;
  bodyDef.angle = CONVERTER::degToRad(rotation);

  _body = world.CreateBody(&bodyDef);

  switch(type)
  {
  case TetriminoTypes::O :
  {
    createPart(0,0,type);
    createPart(0,1,type);
    createPart(1,0,type);
    createPart(1,1,type);
  }break;
  case TetriminoTypes::I :
  {
    createPart(0,0,type);
    createPart(1,0,type);
    createPart(2,0,type);
    createPart(3,0,type);
  }break;
  case TetriminoTypes::S :
  {
    createPart(0,1,type);
    createPart(1,1,type);
    createPart(1,0,type);
    createPart(2,0,type);
  }break;
  case TetriminoTypes::Z :
  {
    createPart(0,0,type);
    createPart(1,0,type);
    createPart(1,1,type);
    createPart(2,1,type);
  }break;
  case TetriminoTypes::L :
  {
    createPart(0,1,type);
    createPart(0,0,type);
    createPart(1,0,type);
    createPart(2,0,type);
  }break;
  case TetriminoTypes::J :
  {
    createPart(0,0,type);
    createPart(1,0,type);
    createPart(2,0,type);
    createPart(2,1,type);
  }break;
  case TetriminoTypes::T :
  {
    createPart(0,0,type);
    createPart(1,0,type);
    createPart(1,1,type);
    createPart(2,0,type);
  }break;
  default:break;
  }

  _body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
  update();
}

Piece::~Piece()
{
  for(b2Fixture* fixture=_body->GetFixtureList();fixture!=nullptr;fixture=fixture->GetNext())
  {
    QGraphicsPolygonItem* pItem = reinterpret_cast<QGraphicsPolygonItem*>(fixture->GetUserData().pointer);
    if(pItem)
    {
      fixture->GetUserData().pointer = 0;
      delete pItem;
    }
  }
  _world.DestroyBody(_body);
}

void Piece::update()
{
  const b2Transform& xf = _body->GetTransform();

  for(b2Fixture* fixture = _body->GetFixtureList(); fixture != nullptr;fixture=fixture->GetNext())
  {
    QGraphicsPolygonItem* shape = reinterpret_cast<QGraphicsPolygonItem*>(fixture->GetUserData().pointer);

    const b2PolygonShape* b2shape = static_cast<b2PolygonShape*>(fixture->GetShape());
    const uint32 count = b2shape->m_count;
    QPolygonF polygon;
    for(uint32 i=0; i < count; ++i)
    {
      b2Vec2 vertex = b2Mul(xf, b2shape->m_vertices[i]);
      polygon << QPointF(CONVERTER::metersToPixels(vertex.x), CONVERTER::metersToPixels(vertex.y));
    }
    shape->setPolygon(polygon);
  }
}

void Piece::rotate(float angle)
{
  _body->ApplyTorque(CONVERTER::degToRad(angle)/25.0f,true);
}

void Piece::moveX(int direction)
{
  _body->ApplyForceToCenter(b2Vec2(CONVERTER::pixelsToMeters(direction)/50,0),true);
}

void Piece::moveY()
{
  _body->ApplyForceToCenter(b2Vec2(0,0.001),true);
}

void Piece::setEnabled(bool value)
{
  m_enabled = value;
}

bool Piece::isEnabled() const
{
  return m_enabled;
}

b2Body *Piece::getBody() const
{
  return _body;
}

void Piece::draw(GameScene &gs)
{
  for(const b2Fixture* fixture=_body->GetFixtureList();fixture!=nullptr;fixture=fixture->GetNext())
  {
    QGraphicsPolygonItem *shape = reinterpret_cast<QGraphicsPolygonItem*>(fixture->GetUserData().pointer);
    if(shape){
      QGraphicsPolygonItem *newItem = new QGraphicsPolygonItem();
      newItem->setPos(newItem->pos());
      newItem->setBrush(shape->brush());
      newItem->setPen(shape->pen());
      newItem->setPolygon(shape->polygon());
      gs.addItem(newItem);
    }
  }
}

b2Fixture *Piece::createPart(int pos_x, int pos_y, TetriminoTypes type)
{
  b2PolygonShape b2shape;


  b2shape.SetAsBox(CONVERTER::pixelsToMeters(SIZE_2), //width/2
                   CONVERTER::pixelsToMeters(SIZE_2), //height/2
                   b2Vec2(CONVERTER::pixelsToMeters(SIZE_2+(pos_x*SIZE)), //center.x
                          CONVERTER::pixelsToMeters(SIZE_2+(pos_y*SIZE))),//center.y
                   0);//angle

  b2FixtureDef fixtureDef;
  fixtureDef.density = 1.0;
  fixtureDef.friction = 0.5;
  fixtureDef.restitution= 0.4;
  fixtureDef.shape = &b2shape;

  b2Fixture* fixture = _body->CreateFixture(&fixtureDef);

  QGraphicsPolygonItem *shape = new QGraphicsPolygonItem(QPolygonF());
  shape->setBrush(QBrush(Piece::COLORS[type]));
  shape->setPen(QPen(QColor(128,128,128),1));

  uintptr_t bodyId = reinterpret_cast<uintptr_t>(shape);
  fixture->GetUserData().pointer = bodyId;

  return fixture;
}
