#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "LibreBridge.hpp"
#include "Bid.hpp"
#include "Card.hpp"

class Player
{
	private:
	protected:
		std::string name;
		Position position;
		std::vector<Card> hand;
		bool isHuman;
	public:
		void addCardToHand(Card card);
		void printHand(char c);
		void sortHand();
		void sortHand(Suit suit);
		uint8_t countHonorPoints() const;
		uint8_t countHonorPoints(Suit suit) const;
		uint8_t countVoids() const;
		uint8_t countCards(Suit suit) const;
		uint8_t countSuits() const;
		uint8_t countLongestSuit() const;
		void setPosition(Position p);
		Position getPosition();
		void clearHand();
		void clearCard(Card card);
		virtual Card playCard(Suit firstSuit, std::vector<Card> dummyHand = std::vector<Card>()) = 0; // If firstSuit is NoTrump, this is the first card to be played
		virtual void bid(Bid& bid, uint8_t lastLevel, Suit lastSuit, bool lastDoubled, bool lastRedoubled, std::vector<Bid> bidWar) = 0;
		Card playRandomCard(Suit firstSuit, std::vector<Card> dummyHand = std::vector<Card>());
		bool hasCard(Card c, std::vector<Card> dummyHand = std::vector<Card>()) const;
		bool isValidPlay(Card c, Suit firstSuit, std::vector<Card> dummyHand = std::vector<Card>()) const;
		std::vector<Card> getPlayableCards(Suit firstSuit, std::vector<Card> dummyHand = std::vector<Card>());
		bool getIsHuman();
		virtual ~Player(){}
		bool playingForDummy(std::vector<Card> dummyHand) const;
		std::vector<Card> getHand();
		static Player* getNewPlayer(std::string playerType);
		static void listAvailableAIPlayers();
};

#endif