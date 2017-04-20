#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include "../LibreBridge.hpp"
#include "../Bid.hpp"
#include "../Card.hpp"
#include "../Misc.hpp"
#include "../Player.hpp"
#include "../AIPlayer.hpp"
#include "AI_Random.hpp"

using namespace std;

Card AI_Random::playCard(Suit firstSuit)
{
	return playRandomCard(firstSuit);
}

void AI_Random::makeBid(BetType& betType, Suit& suit, uint8_t& level)
{
	betType = Pass;
}