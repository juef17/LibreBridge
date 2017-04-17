#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player
{
	private:
	protected:
		char name[];
		Position position;
		std::vector<Card> hand;
	public:
		void addCardToHand(Card card);
		void printHand(char c);
		void sortHand();
		void sortHand(Suit suit);
		uint8_t countHonorPoints();
		uint8_t countHonorPoints(Suit suit);
		void setPosition(Position p);
		void clearHand();
		virtual Card playCard(Suit firstSuit);
		Card playRandomCard(Suit firstSuit);
		bool hasCard(Card c) const;
		bool isValidPlay(Card c, Suit firstSuit) const;
		std::vector<Card> getPlayableCards(Suit firstSuit);
};

#endif