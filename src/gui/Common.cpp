#include <cmath>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include "Common.hpp"
#include "../Bid.hpp"

void setTitle(QWidget *w, QString s)
{
	if(s.length()) w->setWindowTitle(s);
	else w->setWindowTitle(QString("LibreBridge %1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD));
}

int getSeedTextMaxWidth()
{
	QFont font;
	font.setFamily(font.defaultFamily());
	QFontMetrics fontMetrics(font);
	RANDOMNESS_SIZE l = (RANDOMNESS_SIZE)(pow(2, 8*sizeof(RANDOMNESS_SIZE))-1);
	QString s = QString("%1").arg(l);
	return fontMetrics.width(s.append("9"));
}

int getSeedTextMaxLength()
{
	return (int)(ceil(8*sizeof(RANDOMNESS_SIZE)*log10(2)));
}

void copyWindowGeometry(QWidget *w1, QWidget *w2)
{
	w2->restoreGeometry(w1->saveGeometry());
}

void setBidHistoryText(QLabel *l, Bid bid)
{
	//l->setText("RAMOUTZ");
	
	// QString("♥")
	// http://stackoverflow.com/questions/30973781/qt-add-custom-font-from-resource
}

void setBiddingButtonGraphics(QPushButton *b, Suit s)
{
	switch(s)
	{
		case Clubs:
			b->setText("♣");
			break;
		case Diamonds:
			b->setText("♦");
			break;
		case Hearts:
			b->setText("♥");
			break;
		case Spades:
			b->setText("♠");
			break;
		case NoTrump:
		default:
			b->setText("NT");
	}
	if(s == Diamonds || s == Hearts) b->setStyleSheet("QPushButton {color: red;}");
	QFont font = b->font();
	font.setPointSize(12);
	b->setFont(font);
}