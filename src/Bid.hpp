#ifndef BID_HPP
#define BID_HPP

class Bid {
	private:
		Position player;
		BetType betType;
		uint8_t	suit : 3,
				level : 3,
				:2;
	protected:
	public:
		void setBid(std::string stringBid, Position pla, uint8_t lastLevel, Suit lastSuit, bool doubled, bool redoubled);
		void printBid();
		BetType getBetType();
		Suit getSuit();
		uint8_t getLevel();
};

#endif