#include "CardLayout.hpp"
#include "../Player.hpp"
#include <iostream>

CardLayout::CardLayout(Player *p, std::vector<CardWidget*> *h, QWidget *parent): QLayout(parent)
{
	handWidgets = h;
	player = p;
}

int CardLayout::count() const
{
	return list.size();
}

QLayoutItem *CardLayout::itemAt(int idx) const
{
	return list.value(idx);
}

QLayoutItem *CardLayout::takeAt(int idx)
{
	return idx >= 0 && idx < list.size() ? list.takeAt(idx) : 0;
}

void CardLayout::addItem(QLayoutItem *item)
{
	list.append(item);
}

CardLayout::~CardLayout()
{
	QLayoutItem *item;
	while((item = takeAt(0))) delete item;
}

void CardLayout::setGeometry(const QRect &r)
{
	QLayout::setGeometry(r);
	
	int listSize = list.size();
	if(!listSize) return;

	int xStart = r.x();
	int yStart = r.y();
	QLayoutItem *item = list.at(0);
	QRect itemRect = (item ? item->geometry() : QRect(0, 0, 0, 0));
	Position playerPosition = player->getPosition();
	int i = 0;
	int suitOrder[4] = {-1, -1, -1, -1}; // Suit-1 for index, gives order for that suit, -1 = none added yet
	int cardCountX[4] = {-1, -1, -1, -1}; // which "level" of card are we at horizontally, -1 = none added yet
	int currentOrder = 0; // which "level" of suit are we at vertically or horizontally
	
	switch(playerPosition)
	{
		case North:
		case South:
			xStart += (r.width() - listSize * spacing() - (player->countSuits()) * itemRect.width()) / 2;
			break;
		case East:
			xStart += r.width() - itemRect.width();
		case West:
			yStart += (r.height() - itemRect.height() - (player->countSuits() - 1) * 2 * spacing()) / 2;
			break;
		default: break;
	}
	
	for (auto &o : *handWidgets)
	{
		Card card = o->getCard();
		Suit suit = card.getSuit();
		int suitMinus1 = suit - 1;
		if(suitOrder[suitMinus1] == -1)
		{
			suitOrder[suitMinus1] = currentOrder;
			currentOrder++;
		}
		cardCountX[suitMinus1]++;
		int x = 0;
		int y = 0;

		// For x
		switch(playerPosition)
		{
			case North:
			case South:
				x = i * spacing() + std::max(0, currentOrder-1) * itemRect.width();
				break;
			case East:
				x = -(player->countCards(suit) - cardCountX[suitMinus1] - 1) * spacing();
				break;
			case West:
				x = cardCountX[suitMinus1] * spacing();
				break;
			default: break;
		}

		// For y
		switch(playerPosition)
		{
			case East:
			case West:
				y = suitOrder[suitMinus1] * 2 * spacing();
				break;
			default: break;
		}
		
		QRect geom(xStart + x, yStart + y, itemRect.width(), itemRect.height());
		o->setGeometry(geom);
		i++;
	}
}

QSize CardLayout::sizeHint() const
{
	QSize s(0, 0);
	int n = list.count();
	int i = 0;
	while(i < n)
	{
		QLayoutItem *o = list.at(i);
		s = s.expandedTo(o->sizeHint());
		++i;
	}
	return s + (n-1)*QSize(spacing(), 0);
}

QSize CardLayout::minimumSize() const
{
	QSize s(0, 0);
	int n = list.count();
	int i = 0;
	while(i < n)
	{
		QLayoutItem *o = list.at(i);
		s = s.expandedTo(o->minimumSize());
		++i;
	}
	return s + (n-1)*QSize(spacing(), 0);
}

int CardLayout::spacing() const
{
	return 16;
}