#include <QPixmap>
#include <QIcon>
#include <QSize>
#include <QString>
#include <iostream>
#include "CardWidget.hpp"
#include "PlayWindow.hpp"

CardWidget::CardWidget(Card c, QWidget *parent) : QPushButton("", parent)
{
	this->parent = (PlayWindow*)parent;
	card = c;
	QPixmap pixmap(QString::fromStdString(card.getImagePath()));
	setIcon(QIcon(pixmap));
	QSize size = pixmap.rect().size();
	setIconSize(size);
	resize(size);
	setMinimumSize(size);
	setFlat(true);
	connect(this, SIGNAL (clicked()), this, SLOT (clickCardWidget()));
}

Card CardWidget::getCard()
{
	return card;
}

void CardWidget::clickCardWidget()
{
	if(!parent->getCardsAreClickable()) return;
	if(DEBUG_COUT)
	{
		std::cout << std::flush;
		card.printCard();
		std::cout << std::flush;
	}
}