#include "PlayedCardsLayout.hpp"
#include "../Player.hpp"
#include "../Game.hpp"
#include "../Misc.hpp"
#include <iostream>

PlayedCardsLayout::PlayedCardsLayout(std::vector<CardWidget*> *playedCardsWidgets, Game *g, QWidget *parent): QLayout(parent)
{
	cardWidgets = playedCardsWidgets;
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
	
	Card playedCards[4];
	int i = 0;
	Position currentWinner;
	CardWidget *winningCardWidget = Q_NULLPTR;
	for (auto &o : *cardWidgets)
	{
		Card card = o->getCard();
		Position position = game->getPositionFromCard(card, true);
		for(int j = i; j<4; j++) playedCards[j] = card;
		currentWinner = whoWinsTheTrick(playedCards,game->getPositionFromCard(playedCards[0], true), game->getContract());
		if(position == currentWinner) winningCardWidget = o;
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
		o->resetColor();
		i++;
	}
	if(i) winningCardWidget->setEmphasisColor();
}

QSize PlayedCardsLayout::sizeHint() const
{
	return minimumSize();
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
	return s + QSize(0, 0);
}

int PlayedCardsLayout::spacing() const
{
	return 16;
}

void PlayedCardsLayout::clearPlayedCardsWidgets()
{
	for(auto &o : *cardWidgets)
	{
		removeWidget((QWidget*)o);
		delete o;
	}
	cardWidgets->clear();
}