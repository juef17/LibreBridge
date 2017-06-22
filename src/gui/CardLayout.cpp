#include "CardLayout.hpp"
#include "PlayWindow.hpp"
#include "../Player.hpp"
#include "PlayWindow.hpp"
#include "../Game.hpp"
#include "../Misc.hpp"
#include <iostream>

CardLayout::CardLayout(Player *p, std::vector<CardWidget*> *h, PlayWindow *playWindow, QWidget *parent): QLayout(parent)
{
	handWidgets = h;
	player = p;
	this->playWindow = playWindow;
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
	int itemRectWidth = itemRect.width();
	int itemRectHeight = itemRect.height();
	int rWidth = r.width();
	int rHeight = r.height();
	int spacing = this->spacing();
	Position playerPosition = player->getPosition();
	Contract contract = playWindow->getGame()->getContract();
	Position dummyPosition = contract.getIsSet() ? nextTeammate(contract.getDeclarer()) : North;
	bool showCardsFaceUp = player->getIsHuman() || (contract.getIsSet() && player->getPosition() == dummyPosition) || playWindow->getGame()->getPlayedCardsHistory().size() == 52;
	int i = 0;
	int suitOrder[4] = {-1, -1, -1, -1}; // Suit-1 for index, gives order for that suit, -1 = none added yet
	int cardCountX[4] = {-1, -1, -1, -1}; // which "level" of card are we at horizontally, -1 = none added yet
	int currentOrder = 0; // which "level" of suit are we at vertically or horizontally
	
	switch(playerPosition)
	{
		case North:
		case South:
			if(showCardsFaceUp) xStart += (rWidth - (listSize - 1) * spacing - (player->countSuits()) * itemRectWidth) / 2;
			else xStart += (rWidth - (listSize - 1) * spacing - itemRectWidth) / 2;
			break;
		case East:
			xStart += rWidth - itemRectWidth;
		case West:
			if(showCardsFaceUp) yStart += (rHeight - itemRectHeight - (player->countSuits() - 1) * 2 * spacing) / 2;
			else yStart += (rHeight - itemRectHeight - (listSize - 1) * spacing/2) / 2;
			break;
		default: break;
	}
	for (auto &o : *handWidgets)
	{
		int x = 0;
		int y = 0;
		if(showCardsFaceUp)
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

			// For x
			switch(playerPosition)
			{
				case North:
				case South:
					x = i * spacing + std::max(0, currentOrder-1) * itemRectWidth;
					break;
				case East:
					x = -(player->countCards(suit) - cardCountX[suitMinus1] - 1) * spacing;
					break;
				case West:
					x = cardCountX[suitMinus1] * spacing;
					break;
				default: break;
			}

			// For y
			switch(playerPosition)
			{
				case East:
				case West:
					y = suitOrder[suitMinus1] * 2 * spacing;
					break;
				default: break;
			}
		}
		else
		{
			switch(playerPosition)
			{
				case North:
				case South:
					x = i * spacing;
					break;
				case East:
				case West:
					y = i * spacing / 2;
					break;
				default: break;
			}
		}
		
		QRect geom(xStart + x, yStart + y, itemRectWidth, itemRectHeight);
		o->setGeometry(geom);
		o->setCardImageFaceUp(showCardsFaceUp);
		i++;
	}
}

QSize CardLayout::sizeHint() const
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
	return s + (n-1)*QSize(spacing, 0);*/
}

QSize CardLayout::minimumSize() const
{
	QSize s(0, 0);
	int n = list.count();
	int i = 0;
	QRect itemRect;
	while(i < n)
	{
		QLayoutItem *o = list.at(i);
		if(!i) itemRect = (o ? o->geometry() : QRect(0, 0, 0, 0));
		//s = s.expandedTo(o->minimumSize());
		++i;
	}
	int itemRectWidth = itemRect.width();
	int itemRectHeight = itemRect.height();
	int spacing = this->spacing();
	int w=0, h=0;
	Position playerPosition = player->getPosition();
	Position dummyPosition = nextTeammate(playWindow->getGame()->getContract().getDeclarer());
	bool showCardsFaceUp = player->getIsHuman() || player->getPosition() == dummyPosition || playWindow->getGame()->getPlayedCardsHistory().size() == 52;
	if(n)
	{
		if(showCardsFaceUp) switch(playerPosition)
		{
			case North:
			case South:
				w = 12 * spacing + player->countSuits() * itemRectWidth;
				h = itemRectHeight;
				break;
			case East:
			case West:
				w = (player->countLongestSuit() - 1) * spacing + itemRectWidth;
				h = itemRectHeight + (player->countSuits() - 1) * 2 * spacing;
				break;
			default: break;
		}
		else switch(playerPosition)
		{
			case North:
			case South:
				w = 12 * spacing + itemRectWidth;
				h = itemRectHeight;
				break;
			case East:
			case West:
				w = itemRectWidth;
				h = itemRectHeight + 6 * spacing;
				break;
			default: break;
		}
	}
	return s + QSize(std::max(w, s.width()), std::max(h, s.height()));
}

int CardLayout::spacing() const
{
	return 16;
}

void CardLayout::removeCardWidget(CardWidget *c)
{
	removeWidget(c);
	handWidgets->erase(std::remove(handWidgets->begin(), handWidgets->end(), c), handWidgets->end());
}