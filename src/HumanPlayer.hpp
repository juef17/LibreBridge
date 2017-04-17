#ifndef HUMANPLAYER_HPP
#define HUMANPLAYER_HPP

class HumanPlayer: public Player
{
	private:
	protected:
	public:
		Card playCard(Suit firstSuit);
};

#endif