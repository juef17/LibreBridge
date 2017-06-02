#include <iostream>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <random>
#include "Player.hpp"
#include "Misc.hpp"

#include "HumanPlayer.hpp"
#include "AIPlayers/AI_Random.hpp"

using namespace std;

bool Player::hasCard(Card c, vector<Card> dummyHand) const
{
	for (auto &card : (playingForDummy(dummyHand) ? dummyHand : hand)) if(c == card) return true;
	return false;
}

bool Player::isValidPlay(Card c, Suit firstSuit, vector<Card> dummyHand) const
{
	if(c.getSuit() == firstSuit) return true;
	for (auto &card : (playingForDummy(dummyHand) ? dummyHand : hand)) if(card.getSuit() == firstSuit) return false;
	return true;
}

Card Player::playRandomCard(Suit firstSuit, vector<Card> dummyHand)
{
	vector<Card> playableCards = getPlayableCards(firstSuit, dummyHand);
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	auto engine = default_random_engine{seed};
	shuffle(begin(playableCards), end(playableCards), engine);
	cout << positionToString(position) << " is playing ";
	playableCards[0].printCard();
	if(playingForDummy(dummyHand)) cout << " for " << positionToString(nextTeammate(position));
	cout << "\n";
	return playableCards[0];
}
vector<Card> Player::getPlayableCards(Suit firstSuit, vector<Card> dummyHand)
{
	vector<Card> playableCards;
	for (auto &card : (playingForDummy(dummyHand) ? dummyHand : hand)) if(isValidPlay(card, firstSuit, dummyHand)) playableCards.push_back(card);
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

uint8_t Player::countHonorPoints() const
{
	uint8_t points = 0;
	for (auto &card : hand)
	{
		int8_t pts = card.getValue() - 10;
		if(pts > 0) points += pts;
	}
	return points;
}

uint8_t Player::countHonorPoints(Suit suit) const
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

uint8_t Player::countVoids() const
{
	uint8_t suitCounts[5] = {0};
	for (auto &card : hand) suitCounts[card.getSuit()]++;
	return ((suitCounts[Clubs] == 0) + (suitCounts[Diamonds] == 0) + (suitCounts[Hearts] == 0) + (suitCounts[Spades] == 0));
}

uint8_t Player::countCards(Suit suit) const
{
	uint8_t suitCount = 0;
	for (auto &card : hand) if(card.getSuit() == suit) suitCount++;
	return suitCount;
}

uint8_t Player::countCards() const
{
	return hand.size();
}

uint8_t Player::countSuits() const
{
	return 4 - countVoids();
}

uint8_t Player::countLongestSuit() const
{
	uint8_t suitCounts[5] = {0};
	for (auto &card : hand) suitCounts[card.getSuit()]++;
	return std::max({suitCounts[1], suitCounts[2], suitCounts[3], suitCounts[4]});
}

void Player::setPosition(Position p)
{
	position = p;
}

Position Player::getPosition()
{
	return position;
}

void Player::clearHand()
{
	hand.clear();
}

void Player::clearCard(Card card)
{
	hand.erase(remove(hand.begin(), hand.end(), card), hand.end());
}

bool Player::getIsHuman()
{
	return isHuman;
}

bool Player::playingForDummy(vector<Card> dummyHand) const
{
	return !dummyHand.empty();
}

vector<Card> Player::getHand()
{
	return hand;
}

Player* Player::getNewPlayer(string playerType)
{
	if(playerType == "Human" || playerType == "HumanPlayer") return new HumanPlayer;
	if(playerType == "AI_Random") return new AI_Random;
	return new HumanPlayer;
}

void Player::listAvailableAIPlayers()
{
	options.AI_availableTypes.push_back("AI_Random");
}
