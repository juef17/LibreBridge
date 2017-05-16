#include <QPixmap>
#include <QIcon>
#include <QSize>
#include <QString>
#include "CardWidget.hpp"
#include "../Card.hpp"

CardWidget::CardWidget(Card *card, QWidget *parent) : QPushButton("", parent)
{
	QPixmap pixmap(QString::fromStdString(card->getImagePath()));
	setIcon(QIcon(pixmap));
	QSize size = pixmap.rect().size();
	setIconSize(size);
	resize(size);
	setMinimumSize(size);
	setFlat(true);
	connect(this, SIGNAL (clicked()), this, SLOT (clickCardWidget()));
}

void CardWidget::clickCardWidget()
{
	//TODO
}