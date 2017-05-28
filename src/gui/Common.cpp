#include <cmath>
#include <QWidget>
#include <QLabel>
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
	l->setText("RAMOUTZ");
	//ql->setText("<font color=\"blue\">Hello</font> <font color=\"red\">World</font><font color=\"green">!</font>");
	
	// QString("♥")
	// http://stackoverflow.com/questions/30973781/qt-add-custom-font-from-resource
}