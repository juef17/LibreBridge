#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include "LibreBridge.hpp"
#include "Bid.hpp"
#include "Card.hpp"
#include "Misc.hpp"
#include "Player.hpp"

using namespace std;

bool Player::hasCard(Card c) const
{
	for (auto &card : hand) if(c == card) return true;
	return false;
}

bool Player::isValidPlay(Card c, Suit firstSuit) const
{
	if(c.getSuit() == firstSuit) return true;
	for (auto &card : hand) if(card.getSuit() == firstSuit) return false;
	return true;
}

Card Player::playRandomCard(Suit firstSuit)
{
	vector<Card> playableCards = getPlayableCards(firstSuit);
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	auto engine = default_random_engine{seed};
	shuffle(begin(playableCards), end(playableCards), engine);
	cout << positionToString(position) << " is playing ";
	playableCards[0].printCard();
	cout << "\n";
	return playableCards[0];
}

vector<Card> Player::getPlayableCards(Suit firstSuit)
{
	vector<Card> playableCards;
	for (auto &card : hand) if(isValidPlay(card, firstSuit)) playableCards.push_back(card);
	return playableCards;
}

void Player::addCardToHand(Card card)
{
	hand.push_back(card);
}

void Player::printHand(char c)
{
	cout << setw(5) << positionToString(position);
	cout << ":" << c;
	for (auto &card : hand)
	{
		card.printCard();
		cout << c;
	}
	if(c == ' ') cout << "\n";
}

void Player::sortHand()
{
	sort(hand.begin(), hand.end());
}

void Player::sortHand(Suit suit)
{
	sort(hand.begin(), hand.end(),
		[suit](const Card& c1, const Card& c2)
		{
			if(c1.getSuit() == suit && c2.getSuit() != suit) return true;
			if(c1.getSuit() != suit && c2.getSuit() == suit) return false;
			if(c1.getSuit() < c2.getSuit()) return true;
			if(c1.getSuit() > c2.getSuit()) return false;
			return (c1.getValue() > c2.getValue());
		}
	);
}

uint8_t Player::countHonorPoints()
{
	uint8_t points = 0;
	for (auto &card : hand)
	{
		int8_t pts = card.getValue() - 10;
		if(pts > 0) points += pts;
	}
	return points;
}

uint8_t Player::countHonorPoints(Suit suit)
{
	uint8_t points = 0;
	for (auto &card : hand)
	{
		if(suit != card.getSuit()) continue;
		int8_t pts = card.getValue() - 10;
		if(pts > 0) points += pts;
	}
	return points;
}

void Player::setPosition(Position p)
{
	position = p;
}

Position Player::getPosition(Position p)
{
	return position;
}

void Player::clearHand()
{
	hand.clear();
}

bool Player::getIsHuman()
{
	return isHuman;
}