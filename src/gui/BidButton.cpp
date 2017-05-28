#include "BidButton.hpp"
#include "BidWindow.hpp"
#include "../Bid.hpp"
#include "../Misc.hpp"
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
	parent->bidClick(Normal, level, suit);
}

void BidButton::setImage()
{
	setText(QString::fromStdString(suitToUnicodeString(suit)));
	if(suit == Diamonds || suit == Hearts) setStyleSheet("BidButton {color: red;}");
	QFont font = this->font();
	font.setPointSize(12);
	setFont(font);
	
	setAutoDefault(false);
	setFixedSize(24, 24);
}