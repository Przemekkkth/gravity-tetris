#ifndef PIECE_H
#define PIECE_H
#include <QColor>
#include "box2d/box2d.h"

class GameScene;
class Piece
{
public:
  enum TetriminoTypes {O=0,I,S,Z,L,J,T,TYPE_SIZE}; //< different kind of pieces
  static const int SIZE;
  static const int SIZE_2;
  static const QColor COLORS[TetriminoTypes::TYPE_SIZE]; //< different colors for each kind of piece

  Piece(b2World& world,int pos_x,int pos_y,TetriminoTypes type,float rotation);
  virtual ~Piece();

  void update();
  void rotate(float angle);
  void moveX(int direction);
  void moveY();
  void setEnabled(bool value);
  bool isEnabled() const;

  b2Body* getBody() const;

  void draw(GameScene& gs);
private:
    b2Fixture* createPart(int pos_x,int pos_y,TetriminoTypes type); ///< position is relaive to the piece int the matric coordonate (0 to 3)
    b2Body * _body;
    b2World& _world;
    bool m_enabled;
};

#endif // PIECE_H
