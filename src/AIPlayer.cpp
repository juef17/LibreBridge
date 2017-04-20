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
#include "AIPlayer.hpp"

using namespace std;

AIPlayer::AIPlayer()
{
	isHuman = false;
}

void AIPlayer::bid(Bid& bid, uint8_t lastLevel, Suit lastSuit, bool lastDoubled, bool lastRedoubled)
{
	BetType betType = Invalid;
	Suit suit = NoTrump;
	uint8_t level = 0;
	makeBid(betType, suit, level);
	bid.setBid(betType, suit, level, position, lastLevel, lastSuit, lastDoubled, lastRedoubled);
}