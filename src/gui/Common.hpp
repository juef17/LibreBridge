#ifndef COMMON_HPP
#define COMMON_HPP

#include <QString>
#include <string>
#include "../LibreBridge.hpp"
#include "../Bid.hpp"
class QWidget;
class QLabel;
class QPushButton;

#define MIN_WIDTH 480
#define MIN_HEIGHT 320
#define SEED_TEXT_MAX_LENGTH (8 * sizeof(RANDOMNESS_SIZE) * 3010299956/10000000000 + 1)

void setTitle(QWidget *w, QString s = "");
int getSeedTextMaxWidth();
int getSeedTextMaxLength();
void copyWindowGeometry(QWidget *w1, QWidget *w2);
void setBidHistoryText(QLabel *l, Bid bid);
void setBiddingButtonGraphics(QPushButton *b, Suit s);

#endif