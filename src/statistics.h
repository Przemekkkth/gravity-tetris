#ifndef STATISTICS_H
#define STATISTICS_H
#include <QGraphicsSimpleTextItem>


class GameScene;
class Statistics
{
public:
  Statistics();
  ~Statistics();

  void addLines(int lines);
  unsigned int getLevel() const;
  bool isGameOver() const;
  void setGameOver(bool);
  void draw(GameScene& gs);
  void setOffset(int x, int y);
private:
  unsigned int m_nbRows;
  unsigned int m_nbScore;
  unsigned int m_nbLvl;

  bool m_isGameOver;
  int m_offsetX, m_offsetY;
  QGraphicsSimpleTextItem *m_textRows;
  QGraphicsSimpleTextItem *m_textScore;
  QGraphicsSimpleTextItem *m_textLvl;
  QGraphicsSimpleTextItem *m_textGameOver;
};

#endif // STATISTICS_H
