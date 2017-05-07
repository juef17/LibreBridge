#include <cmath>
#include "Common.hpp"

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
	RANDOMNESS_SIZE l = (RANDOMNESS_SIZE)(pow(2, 8*sizeof(RANDOMNESS_SIZE)));
	QString s = QString("%1").arg(l);
	return fontMetrics.width(s.append("9"));
}

void copyWindowGeometry(QWidget *w1, QWidget *w2)
{
	w2->restoreGeometry(w1->saveGeometry());
}