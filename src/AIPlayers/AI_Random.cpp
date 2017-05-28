//#include <string>
//#include <iostream>
//#include <chrono>
//#include <algorithm>
//#include <iomanip>
#include "AI_Random.hpp"
#include "../Misc.hpp"

using namespace std;

Card AI_Random::chooseCardToPlay(Suit firstSuit, vector<Card> dummyHand)
{
	return playRandomCard(firstSuit, dummyHand);
}

void AI_Random::makeBid(BetType& betType, Suit& suit, uint8_t& level, vector<Bid> bidWar)
{
	betType = Pass;
	// If everybody passed so far, 20% chance to make a random level 1 bet
	for (auto &bid : bidWar) if(bid.getBetType() == Normal) level = bid.getLevel();
	if(!level && randomUint8(0, 4))
	{
		betType = Normal;
		level = 1;
		suit = Suit(randomUint8(1, 5));
	}
}