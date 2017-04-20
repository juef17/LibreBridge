#ifndef HUMANPLAYER_HPP
#define HUMANPLAYER_HPP

class HumanPlayer: public Player
{
	private:
	protected:
	public:
		HumanPlayer();
		Card playCard(Suit firstSuit);
		void bid(Bid& bid, uint8_t lastLevel, Suit lastSuit, bool lastDoubled, bool lastRedoubled);
};

#endif