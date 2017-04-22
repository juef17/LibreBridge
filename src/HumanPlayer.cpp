#include <string>
#include <iostream>
#include <vector>
#include "HumanPlayer.hpp"
#include "Misc.hpp"

using namespace std;

HumanPlayer::HumanPlayer()
{
	isHuman = true;
}

Card HumanPlayer::playCard(Suit firstSuit, vector<Card> dummyHand)
{
	Card card;
	string playerInput;
	cout << positionToString(position) << ", please play a card";
	if(playingForDummy(dummyHand)) cout << " for " << positionToString(nextTeammate(position));
	cout << ": ";
	getline (cin, playerInput);
	if(playerInput.length()) card = Card::stringToCard(playerInput);
	else card = playRandomCard(firstSuit, dummyHand);
	return card;
}

void HumanPlayer::bid(Bid& bid, uint8_t lastLevel, Suit lastSuit, bool lastDoubled, bool lastRedoubled, vector<Bid>)
{
	string playerInput;
	cout << positionToString(position) << ", please enter your bid: ";
	getline (cin, playerInput);
	bid.setBid(playerInput, position, lastLevel, lastSuit, lastDoubled, lastRedoubled);
}