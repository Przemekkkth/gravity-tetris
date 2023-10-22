#include "debugdraw.h"
#include "gamescene.h"
#include "utils.h"
#include <QGraphicsPolygonItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QBrush>
#include <QPen>

DebugDraw::DebugDraw(GameScene *gs)
{
  m_gameScene = gs;
  uint32 flags = b2Draw::e_shapeBit;
  flags += b2Draw::e_aabbBit;
  SetFlags(flags);
}

void DebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
  QGraphicsPolygonItem* pItem = new QGraphicsPolygonItem();
  QPolygonF polygon;
  for(int i = 0; i < vertexCount; ++i)
  {
    const b2Vec2& vertex = vertices[i];
    polygon << QPointF(CONVERTER::metersToPixels(vertex.x), CONVERTER::metersToPixels(vertex.y));
  }
  pItem->setPolygon(polygon);
  pItem->setBrush(QBrush(Qt::transparent));
  pItem->setPen(QPen(b2ColorToQColor(color, 50)));
  m_gameScene->addItem(pItem);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
  QGraphicsPolygonItem* pItem = new QGraphicsPolygonItem();
  QPolygonF polygon;
  for(int i = 0; i < vertexCount; ++i)
  {
    const b2Vec2& vertex = vertices[i];
    polygon << QPointF(CONVERTER::metersToPixels(vertex.x), CONVERTER::metersToPixels(vertex.y));
  }
  pItem->setPolygon(polygon);
  pItem->setBrush(QBrush(b2ColorToQColor(color,50)));
  pItem->setPen(QPen(b2ColorToQColor(color)));
  m_gameScene->addItem(pItem);
}

void DebugDraw::DrawCircle(const b2Vec2 &center, float radius, const b2Color &color)
{
  QGraphicsEllipseItem* eItem = new QGraphicsEllipseItem(-CONVERTER::metersToPixels(radius),
                                                         -CONVERTER::metersToPixels(radius),
                                                         2 * CONVERTER::metersToPixels(radius),
                                                         2 * CONVERTER::metersToPixels(radius));

  eItem->setTransformOriginPoint(0, 0);
  eItem->setPos(CONVERTER::metersToPixels(center.x), CONVERTER::metersToPixels(center.y));
  eItem->setBrush(Qt::transparent);
  eItem->setPen(QPen(b2ColorToQColor(color, 50), 1));
  m_gameScene->addItem(eItem);
}

void DebugDraw::DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &, const b2Color &color)
{
  QGraphicsEllipseItem* eItem = new QGraphicsEllipseItem(-CONVERTER::metersToPixels(radius),
                                                         -CONVERTER::metersToPixels(radius),
                                                         2 * CONVERTER::metersToPixels(radius),
                                                         2 * CONVERTER::metersToPixels(radius));

  eItem->setTransformOriginPoint(0, 0);
  eItem->setPos(CONVERTER::metersToPixels(center.x), CONVERTER::metersToPixels(center.y));
  eItem->setBrush(QBrush(b2ColorToQColor(color, 50)));
  eItem->setPen(QPen(b2ColorToQColor(color), 1));
  m_gameScene->addItem(eItem);
}

void DebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
  QGraphicsLineItem* lItem = new QGraphicsLineItem();
  lItem->setLine(CONVERTER::metersToPixels(p1.x), CONVERTER::metersToPixels(p1.y),
                 CONVERTER::metersToPixels(p2.x), CONVERTER::metersToPixels(p2.y));
  lItem->setPen(QPen(b2ColorToQColor(color),1));
  m_gameScene->addItem(lItem);
}

void DebugDraw::DrawTransform(const b2Transform &xf)
{
  const float lineLength = 0.4;
  b2Vec2 p = xf.p + lineLength*xf.q.GetYAxis();
  QGraphicsLineItem* lItem0 = new QGraphicsLineItem();
  lItem0->setLine(CONVERTER::metersToPixels(xf.p.x), CONVERTER::metersToPixels(xf.p.y),
                  CONVERTER::metersToPixels(p.x), CONVERTER::metersToPixels(p.y));
  lItem0->setPen(QPen(Qt::red,1));
  m_gameScene->addItem(lItem0);

  p = xf.p + lineLength * xf.q.GetYAxis();

  QGraphicsLineItem* lItem1 = new QGraphicsLineItem();
  lItem1->setLine(CONVERTER::metersToPixels(xf.p.x), CONVERTER::metersToPixels(xf.p.y),
                  CONVERTER::metersToPixels(p.x), CONVERTER::metersToPixels(p.y));
  lItem1->setPen(QPen(Qt::green,1));
  m_gameScene->addItem(lItem1);
}

void DebugDraw::DrawPoint(const b2Vec2 &p, float size, const b2Color &color)
{
  QPointF point(CONVERTER::metersToPixels(p.x), CONVERTER::metersToPixels(p.y));
  QGraphicsRectItem* rItem = new QGraphicsRectItem(0,0,size, size);
  rItem->setTransformOriginPoint(size/2, size/2);
  rItem->setPen(QPen(b2ColorToQColor(color), 1));
  rItem->setBrush(QBrush(b2ColorToQColor(color)));
  rItem->setPos(point);
  m_gameScene->addItem(rItem);
}

void DebugDraw::setGameScene(GameScene *gameScene)
{
  m_gameScene = gameScene;
}

QColor DebugDraw::b2ColorToQColor(const b2Color &color, int alpha) const
{
  return QColor((quint8)(color.r*255),
                (quint8)(color.g*255),
                (quint8)(color.b*255),
                (quint8)alpha);
}
