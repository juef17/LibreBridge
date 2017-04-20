#ifndef AI_RANDOM_HPP
#define AI_RANDOM_HPP

class AI_Random: public AIPlayer
{
	private:
	protected:
	public:
		Card playCard(Suit firstSuit);
		void makeBid(BetType& betType, Suit& suit, uint8_t& level, std::vector<Bid> bidWar);
};

#endif