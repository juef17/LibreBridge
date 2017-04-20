#include <cstdint>
#include <string>
#include <iostream>
#include "LibreBridge.hpp"
#include "Card.hpp"
#include "Misc.hpp"

using namespace std;

Card::Card(uint8_t v, Suit s)
{
	// TODO: faire les checks de sécurité
	value = v;
	suit = s;
}

Card::Card()
{
}

char Card::valueToChar(uint8_t value)
{
	// TODO: faire les checks de sécurité
	switch(value)
	{
		case 10: return 'T';
		case 11: return 'J';
		case 12: return 'Q';
		case 13: return 'K';
		case 14: return 'A';
		default: return '0' + value;
	}
}

uint8_t Card::charToValue(char c)
{
	// TODO: faire les checks de sécurité
	// TODO: optimiser un peu sti
	for(uint8_t i=0; i<15; i++) if(valueToChar(i) == c) return i;
	return 0;
}

char Card::suitToChar(Suit suit)
{
	// TODO: faire les checks de sécurité
	switch(suit)
	{
		case Clubs: return 'C';
		case Diamonds: return 'D';
		case Hearts: return 'H';
		case Spades: return 'S';
		case NoTrump: return 'N';
		default: return 'X';
	}
}

bool Card::operator == (const Card& card)
{
	return(suit == card.suit && value == card.value);
}

void Card::printCard()
{
	cout << valueToChar(value) << suitToChar(suit);
}

bool Card::operator < (const Card& card)
{
	if(suit < card.suit) return true;
	if(suit > card.suit) return false;
	return (value > card.value);
}

uint8_t Card::getValue() const
{
	return value;
}

Suit Card::getSuit() const
{
	return suit;
}

Card Card::stringToCard(string s)
{
	Card card(0, NoTrump);
	if(s.length() != 2) return card;
	uint8_t value = charToValue(s[0]);
	Suit suit = charToSuit(s[1]);
	if(!value) return card;
	if(suit == NoTrump) return card;
	Card goodCard(value, suit);
	return goodCard;
}