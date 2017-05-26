#include "AIPlayer.hpp"

using namespace std;

AIPlayer::AIPlayer()
{
	isHuman = false;
}

void AIPlayer::bid(Bid& bid, uint8_t lastLevel, Suit lastSuit, bool lastDoubled, bool lastRedoubled, Position playerWhoBetNormallyLast, vector<Bid> bidWar)
{
	BetType betType = Invalid;
	Suit suit = NoTrump;
	uint8_t level = 0;
	makeBid(betType, suit, level, bidWar);
	bid.setBid(betType, suit, level, position, lastLevel, lastSuit, lastDoubled, lastRedoubled, playerWhoBetNormallyLast);
}