#include <cstdint>
#include <string>
#include <iostream>
#include "LibreBridge.hpp"
#include "Bid.hpp"
#include "Misc.hpp"

using namespace std;

void Bid::setBid(string stringBid, Position pla, uint8_t lastLevel, Suit lastSuit, bool doubled, bool redoubled)
{
	betType = Invalid;
	player = pla;
	if(stringBid == "X")
	{
		if(!lastLevel || doubled || redoubled) return;
		betType = Double;
	}
	else if(stringBid == "XX")
	{
		if(!lastLevel || !doubled || redoubled) return;
		betType = Redouble;
	}
	else if(stringBid == "Pass" || stringBid == "") betType = Pass;
	else
	{
		if(stringBid.length() < 2 || stringBid.length() > 3) return;
		level = stringBid[0];
		if(level < 0 || level > 7) return;
		if(stringBid.length() == 2 && (stringBid[1] == 'C' || stringBid[1] == 'D' || stringBid[1] == 'H' || stringBid[1] == 'S'))
		{
			suit = charToSuit(stringBid[1]);
			if(lastLevel > level) return;
			if(lastLevel == level && lastSuit >= suit) return;
			betType = Normal;
		}
		else if(stringBid.length() == 3 && stringBid[1] == 'N' && stringBid[2] == 'T')
		{
			suit = NoTrump;
			if(lastLevel > level) return;
			if(lastLevel == level && lastSuit == suit) return;
			betType = Normal;
		}
	}
}

void Bid::printBid()
{
	if(betType == Invalid)
	{
		cout << "Bid is invalid.";
		return;
	}
	cout << "Bid is: ";
	if(betType == Pass) cout << "Pass";
	if(betType == Double) cout << "Double";
	if(betType == Redouble) cout << "Redouble";
	if(betType == Normal) cout << "Pass";
	cout << " by " << positionToString(player) << "\n";
}

BetType Bid::getBetType()
{
	return betType;
}

Suit Bid::getSuit()
{
	return Suit(suit);
}

uint8_t Bid::getLevel()
{
	return level;
}