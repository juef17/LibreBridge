#include <cmath>
#include <iostream>
#include <QWidget>
#include <QLabel>
#include <QFont>
#include <QFontDatabase>
#include "Common.hpp"
#include "../Bid.hpp"
#include "../Misc.hpp"

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
	QString level, suit, labelText;
	int id = QFontDatabase::addApplicationFont("./fonts/SourceCodePro-Regular.ttf");
	if(id >= 0)
	{
		QString family = QFontDatabase::applicationFontFamilies(id).at(0);
		QFont fontForBidding(family);
		l->setFont(fontForBidding);
	}
	else
	{
		if(DEBUG_COUT) std::cout << std::flush << "CAN'T LOAD FONT" << std::endl << std::flush;
	}
	switch(bid.getBetType())
	{
		case Pass:
			l->setText("Pass");
			break;
		case Normal:
			level = QString::number(bid.getLevel());
			suit = QString::fromStdString(suitToUnicodeString(bid.getSuit()));
			labelText = level;
			if(bid.getSuit() == Diamonds || bid.getSuit() == Hearts) labelText += QString("<font color=\"red\">") + suit + QString("</font>");
			else labelText += suit;
			l->setText(labelText);
			break;
		case Double:
			l->setText("X");
			break;
		case Redouble:
			l->setText("XX");
			break;
		case Invalid:
		default:
			l->setText("");
			if(DEBUG_COUT) std::cout << std::flush << "INVALID BET WHILE SETTING BID HISTORY TEXT, SHOULDN'T HAPPEN" << std::endl << std::flush;
	}
}