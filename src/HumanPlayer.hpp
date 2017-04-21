#ifndef HUMANPLAYER_HPP
#define HUMANPLAYER_HPP

class HumanPlayer: public Player
{
	private:
	protected:
	public:
		HumanPlayer();
		Card playCard(Suit firstSuit, std::vector<Card> dummyHand = std::vector<Card>());
		void bid(Bid& bid, uint8_t lastLevel, Suit lastSuit, bool lastDoubled, bool lastRedoubled, std::vector<Bid>);
		~HumanPlayer(){}
};

#endif