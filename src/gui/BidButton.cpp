#include "BidButton.hpp"
#include "BidWindow.hpp"
#include "../Bid.hpp"
#include <iostream>

BidButton::BidButton(Suit suit, int level, QWidget *parent): QPushButton (parent)
{
	this->parent = (BidWindow*)parent;
	this->suit = suit;
	this->level = level;
	
	setImage();
	
	connect(this, SIGNAL (clicked()), this, SLOT (clickBidButton()));
}

void BidButton::clickBidButton()
{
	std::cout << std::flush;
	std::cout << std::to_string(suit) << " " << std::to_string(level) << std::endl;
	std::cout << std::flush;
}

void BidButton::setImage()
{
	switch(suit)
	{
		case Clubs:
			setText("♣");
			break;
		case Diamonds:
			setText("♦");
			break;
		case Hearts:
			setText("♥");
			break;
		case Spades:
			setText("♠");
			break;
		case NoTrump:
		default:
			setText("NT");
	}
	if(suit == Diamonds || suit == Hearts) setStyleSheet("BidButton {color: red;}");
	QFont font = this->font();
	font.setPointSize(12);
	setFont(font);
	
	setAutoDefault(false);
	setFixedSize(24, 24);
}