#ifndef AIPLAYER_HPP
#define AIPLAYER_HPP

class AIPlayer: public Player
{
	private:
	protected:
	public:
		virtual Card playCard(Suit firstSuit);
};

#endif