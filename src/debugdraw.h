#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <box2d/box2d.h>
#include <QColor>

class GameScene;
class DebugDraw : public b2Draw
{
public:
  DebugDraw(GameScene* gs);
  virtual ~DebugDraw() = default;

  DebugDraw(const DebugDraw&) = delete;
  DebugDraw& operator=(const DebugDraw&) = delete;

  virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
  virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
  virtual void DrawCircle(const b2Vec2 &center, float radius, const b2Color& color);
  virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2&, const b2Color& color);
  virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
  virtual void DrawTransform(const b2Transform& xf);
  virtual void DrawPoint(const b2Vec2 &p, float size, const b2Color &color);
  void setGameScene(GameScene* gameScene);
private:
  QColor b2ColorToQColor(const b2Color& color,int alpha = 255) const;
  GameScene *m_gameScene;
};

#endif // DEBUGDRAW_H
