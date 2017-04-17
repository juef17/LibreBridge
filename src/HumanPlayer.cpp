#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include "LibreBridge.hpp"
#include "Card.hpp"
#include "Misc.hpp"
#include "Player.hpp"
#include "HumanPlayer.hpp"

using namespace std;

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
	hand.erase(std::remove(hand.begin(), hand.end(), card), hand.end());
	return card;
}