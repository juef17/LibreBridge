#include "AIPlayer.hpp"
#include <iostream>

using namespace std;

AIPlayer::AIPlayer()
{
	isHuman = false;
}

Card AIPlayer::playCard(Suit firstSuit, vector<Card> dummyHand)
{
	Card c = chooseCardToPlay(firstSuit, dummyHand);
	if(!hasCard(c, dummyHand) || !isValidPlay(c, firstSuit, dummyHand))
	{
		if(DEBUG_COUT) cout << flush << "INVALID PLAY BY AI" << endl << flush;
		c = playRandomCard(firstSuit, dummyHand);
	}
	return c;
}

void AIPlayer::bid(Bid& bid, uint8_t lastLevel, Suit lastSuit, bool lastDoubled, bool lastRedoubled, Position playerWhoBetNormallyLast, vector<Bid> bidWar)
{
	BetType betType = Invalid;
	Suit suit = NoTrump;
	uint8_t level = 0;
	makeBid(betType, suit, level, bidWar);
	if(betType == Invalid)
	{
		if(DEBUG_COUT) cout << flush << "INVALID BET BY AI" << endl << flush;
		suit = NoTrump;
		level = 0;
		betType = Pass;
	}
	bid.setBid(betType, suit, level, position, lastLevel, lastSuit, lastDoubled, lastRedoubled, playerWhoBetNormallyLast);
}