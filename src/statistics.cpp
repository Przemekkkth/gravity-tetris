#include "statistics.h"
#include "utils.h"
#include "gamescene.h"
#include "fontmanager.h"

Statistics::Statistics()
  : m_nbRows(0), m_nbScore(0), m_nbLvl(0), m_isGameOver(false),
    m_offsetX(0), m_offsetY(0)
{
  QFont guiFont = FontManager::Instance()->getFont(FontManager::FontID::GUI);
  guiFont.setPixelSize(SETTINGS::FONT_SIZE);
  QColor guiFontColor(Qt::white);

  m_textRows = new QGraphicsSimpleTextItem();
  m_textRows->setText("rows : 0");
  m_textRows->setFont(guiFont);
  m_textRows->setPen(QPen(guiFontColor));
  m_textRows->setBrush(QBrush(guiFontColor));
  m_textRows->setPos(0,0);

  m_textScore = new QGraphicsSimpleTextItem();
  m_textScore->setText("score : 0");
  m_textScore->setFont(guiFont);
  m_textScore->setPen(QPen(guiFontColor));
  m_textScore->setBrush(QBrush(guiFontColor));
  m_textScore->setPos(0,SETTINGS::FONT_SIZE+1);

  m_textLvl = new QGraphicsSimpleTextItem();
  m_textLvl->setText("lvl : 0");
  m_textLvl->setFont(guiFont);
  m_textLvl->setPen(QPen(guiFontColor));
  m_textLvl->setBrush(QBrush(guiFontColor));
  m_textLvl->setPos(0, (SETTINGS::FONT_SIZE + 1)*2);

  m_textGameOver = new QGraphicsSimpleTextItem();
  guiFont.setPixelSize(72);
  m_textGameOver->setText("Game Over");
  m_textGameOver->setPen(QPen(guiFontColor));
  m_textGameOver->setBrush(QBrush(guiFontColor));
  m_textGameOver->setPos(0, 0);
}

Statistics::~Statistics()
{
  delete m_textRows;
  m_textRows = nullptr;

  delete m_textScore;
  m_textScore = nullptr;

  delete m_textLvl;
  m_textLvl = nullptr;

  delete m_textGameOver;
  m_textGameOver = nullptr;
}

void Statistics::addLines(int lines)
{
  if (lines > 0)
  {
    //update number of lines
    m_nbRows += lines;
    m_textRows->setText(QString("rows : " + QString::number(m_nbRows)));

    //update the score
    m_textScore->setText(QString("score : ") + QString::number(m_nbScore));

    switch (lines) {
    case 1  : m_nbScore  += 40   * (m_nbLvl+1); break;
    case 2  : m_nbScore  += 100  * (m_nbLvl+1); break;
    case 3  : m_nbScore  += 300  * (m_nbLvl+1); break;
    default : m_nbScore  += 1200 * (m_nbLvl+1); break;
    }

    m_nbLvl = m_nbRows / 10;

    //update the lvl
    m_textLvl->setText("lvl : " + QString::number(m_nbLvl));
  }
}

unsigned int Statistics::getLevel() const
{
  return m_nbLvl;
}

bool Statistics::isGameOver() const
{
  return m_isGameOver;
}

void Statistics::setGameOver(bool val)
{
  m_isGameOver = val;
}

void Statistics::draw(GameScene &gs)
{
  if (!m_isGameOver) {
    QGraphicsSimpleTextItem* tR = new QGraphicsSimpleTextItem;
    tR->setText( m_textRows->text());
    tR->setFont( m_textRows->font());
    tR->setPen(  m_textRows->pen());
    tR->setBrush(m_textRows->brush());
    tR->setPos(  m_textRows->pos().x() + m_offsetX, m_textRows->pos().y() + m_offsetY);
    gs.addItem(tR);

    QGraphicsSimpleTextItem* tS = new QGraphicsSimpleTextItem;
    tS->setText( m_textScore->text());
    tS->setFont( m_textScore->font());
    tS->setPen(  m_textScore->pen());
    tS->setBrush(m_textScore->brush());
    tS->setPos(  m_textScore->pos().x() + m_offsetX, m_textScore->pos().y() + m_offsetY);
    gs.addItem(tS);

    QGraphicsSimpleTextItem* tL = new QGraphicsSimpleTextItem;
    tL->setText( m_textLvl->text());
    tL->setFont( m_textLvl->font());
    tL->setPen(  m_textLvl->pen());
    tL->setBrush(m_textLvl->brush());
    tL->setPos(  m_textLvl->pos().x() + m_offsetX, m_textLvl->pos().y() + m_offsetY);
    gs.addItem(tL);
  }
  else {
    QGraphicsSimpleTextItem* tG = new QGraphicsSimpleTextItem;
    tG->setText( m_textGameOver->text());
    tG->setFont( m_textGameOver->font());
    tG->setPen(  m_textGameOver->pen());
    tG->setBrush(m_textGameOver->brush());
    tG->setPos(  m_textGameOver->pos().x() + m_offsetX, m_textGameOver->pos().y() + m_offsetY);
    gs.addItem(tG);
  }
}

void Statistics::setOffset(int x, int y)
{
  m_offsetX = x;
  m_offsetY = y;
}
