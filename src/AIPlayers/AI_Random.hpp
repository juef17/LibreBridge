#ifndef AI_RANDOM_HPP
#define AI_RANDOM_HPP

#include <vector>
#include <cstdint>
#include "../LibreBridge.hpp"
#include "../Bid.hpp"
#include "../Card.hpp"
#include "../AIPlayer.hpp"

class AI_Random: public AIPlayer
{
	private:
	protected:
	public:
		Card playCard(Suit firstSuit, std::vector<Card> dummyHand = std::vector<Card>());
		void makeBid(BetType& betType, Suit& suit, uint8_t& level, std::vector<Bid> bidWar);
		~AI_Random(){}
};

#endif