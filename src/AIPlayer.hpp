#ifndef AIPLAYER_HPP
#define AIPLAYER_HPP

class AIPlayer: public Player
{
	private:
	protected:
	public:
		AIPlayer();
		virtual Card playCard(Suit firstSuit, std::vector<Card> dummyHand = std::vector<Card>()) = 0;
		virtual void makeBid(BetType& betType, Suit& suit, uint8_t& level, std::vector<Bid> bidWar) = 0;
		void bid(Bid& bid, uint8_t lastLevel, Suit lastSuit, bool lastDoubled, bool lastRedoubled, std::vector<Bid> bidWar);
		virtual ~AIPlayer(){}
};

#endif