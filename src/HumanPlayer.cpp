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
#include "HumanPlayer.hpp"

using namespace std;

HumanPlayer::HumanPlayer()
{
	isHuman = true;
}

Card HumanPlayer::playCard(Suit firstSuit)
{
	Card card;
	string playerInput;
	do
	{
		cout << positionToString(position) << ", please play a card: ";
		getline (cin, playerInput);
		if(playerInput.length()) card = Card::stringToCard(playerInput);
		else card = playRandomCard(firstSuit);
	} while (!card.getValue() || !hasCard(card) || !isValidPlay(card, firstSuit));
	return card;
}

void HumanPlayer::bid(Bid& bid, uint8_t lastLevel, Suit lastSuit, bool lastDoubled, bool lastRedoubled, vector<Bid>)
{
	string playerInput;
	cout << positionToString(position) << ", please enter your bid: ";
	getline (cin, playerInput);
	bid.setBid(playerInput, position, lastLevel, lastSuit, lastDoubled, lastRedoubled);
}