#include "PlayedCardsLayout.hpp"
#include "../Player.hpp"
#include "../Game.hpp"
#include <iostream>

PlayedCardsLayout::PlayedCardsLayout(std::vector<CardWidget*> *pcw, Game *g, QWidget *parent): QLayout(parent)
{
	cardWidgets = pcw;
	game = g;
}

int PlayedCardsLayout::count() const
{
	return list.size();
}

QLayoutItem *PlayedCardsLayout::itemAt(int idx) const
{
	return list.value(idx);
}

QLayoutItem *PlayedCardsLayout::takeAt(int idx)
{
	return idx >= 0 && idx < list.size() ? list.takeAt(idx) : 0;
}

void PlayedCardsLayout::addItem(QLayoutItem *item)
{
	list.append(item);
}

PlayedCardsLayout::~PlayedCardsLayout()
{
	QLayoutItem *item;
	while((item = takeAt(0))) delete item;
}

void PlayedCardsLayout::setGeometry(const QRect &r)
{
	QLayout::setGeometry(r);
	
	int listSize = list.size();
	if(!listSize) return;
	
	int xCenter = r.x() + r.width() / 2;
	int yCenter = r.y() + r.height() / 2;
	QLayoutItem *item = list.at(0);
	QRect itemRect = (item ? item->geometry() : QRect(0, 0, 0, 0));
	
	for (auto &o : *cardWidgets)
	{
		Card card = o->getCard();
		Position position = game->getPositionFromCard(card, true);
		int x = -itemRect.width()/2;
		int y = -itemRect.height()/2;

		switch(position)
		{
			case North:
				y += -spacing();
				break;
			case South:
				y += spacing();
				break;
			case East:
				x += spacing();
				break;
			case West:
				x += -spacing();
				break;
			default: break;
		}
		
		QRect geom(xCenter + x, yCenter + y, itemRect.width(), itemRect.height());
		o->setGeometry(geom);
		o->raise();
	}
}

QSize PlayedCardsLayout::sizeHint() const
{
	return minimumSize();
	/*QSize s(0, 0);
	int n = list.count();
	int i = 0;
	while(i < n)
	{
		QLayoutItem *o = list.at(i);
		s = s.expandedTo(o->sizeHint());
		++i;
	}
	return s + (n-1)*QSize(spacing(), 0);*/
}

QSize PlayedCardsLayout::minimumSize() const
{
	QSize s(0, 0);
	int n = list.count();
	int i = 0;
	QRect itemRect;
	while(i < n)
	{
		QLayoutItem *o = list.at(i);
		if(!i) itemRect = (o ? o->geometry() : QRect(0, 0, 0, 0));
		s = s.expandedTo(o->minimumSize());
		++i;
	}
	int w=0, h=0;
	/*Position playerPosition = player->getPosition();
	if(n) switch(playerPosition)
	{
		case North:
		case South:
			w = 12 * spacing() + player->countSuits() * itemRect.width();
			h = itemRect.height();
			break;
		case East:
		case West:
			w = (player->countLongestSuit() - 1) * spacing() + itemRect.width();
			h = itemRect.height() + (player->countSuits() - 1) * 2 * spacing();
			h = std::max(h, 2 * itemRect.height() + 2 * spacing());
			break;
		default: break;
	}*/
	return s + QSize(0, 0);
}

int PlayedCardsLayout::spacing() const
{
	return 16;
}