#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <cstring>
#include <string>

using namespace std;

// Card values are 2-14, to be interpreted into T,J,Q,K,A with Card::valueToChar
enum Suit {Clubs=1, Diamonds=2, Hearts=3, Spades=4, NoTrump=5};
enum GameType {Local, Network};
enum Position {North=0, East=1, South=2, West=3};
enum BetType {Pass, Normal, Double, Redouble, Invalid};
enum Vulnerability {None=0, NS=1, EW=2, Both=3};

class Bid {
	private:
		Position player;
		BetType betType;
		uint8_t	suit : 3,
				level : 3,
				:2;
	protected:
	public:
		void setBid(string stringBid, Position pla, uint8_t lastLevel, Suit lastSuit, bool doubled, bool redoubled);
		void printBid();
		BetType getBetType();
		Suit getSuit();
		uint8_t getLevel();
};

class Card
{
	private:
		Suit suit;
		uint8_t value;
	protected:
	public:
		Card(uint8_t v, Suit s);
		Card();
		static char valueToChar(uint8_t value);
		static uint8_t charToValue(char c);
		static char suitToChar(Suit suit);
		
		void printCard();
		// TODO: add when g++ supports u8 character literals from c++17, see http://en.cppreference.com/w/cpp/language/character_literal
		// TODO: and convert every string litteral to u8
		/*char cardToUnicode()
		{
			return this->cardToUnicode(this);
		}
		static char cardToUnicode(Card card)
		{
			return this->cardToUnicode(card.value, card.suit);
		}
		static char cardToUnicode(uint8_t value, Suit suit)
		{
			return u8'a';
		}*/
		bool operator == (const Card& card);
		bool operator < (const Card& card);
		uint8_t getValue() const;
		Suit getSuit() const;
		static Card stringToCard(string s);
};

class Player
{
	private:
		char name[];
		Position position;
		vector<Card> hand;
	protected:
	public:
		void addCardToHand(Card card);
		void printHand(char c);
		void sortHand();
		void sortHand(Suit suit);
		uint8_t countHonorPoints();
		uint8_t countHonorPoints(Suit suit);
		void setPosition(Position p);
		void clearHand();
		Card playCard(Suit firstSuit);
		Card playRandomCard(Suit firstSuit);
		bool hasCard(Card c) const;
		bool isValidPlay(Card c, Suit firstSuit) const;
		vector<Card> getPlayableCards(Suit firstSuit);
};

class HumanPlayer: public Player
{
	private:
	protected:
	public:
};

class AIPlayer: public Player
{
	private:
	protected:
	public:
};

class Contract
{
	private:
		uint8_t level; // If 0, all passed
		Suit suit;
		Position declarer;
		bool doubled;
		bool redoubled;
		Vulnerability vulnerability;
	protected:
	public:
		int16_t calculateScore(Position pos, uint8_t tricks);
		static uint16_t calculateIMP(int16_t score);
		void setContract(uint8_t lev, Suit sui, Position pos, bool doubl, bool redoubl, Vulnerability vuln);
		void print();
		Position getDeclarer();
		Suit getSuit() const;
		uint8_t getLevel() const;
		bool isTeamVulnerable(Position p) const;
};

class Game
{
	private:
		GameType gameType;
		// TODO vectors pour contenir les infos des parties précédentes: scores, qui avait le contrat, y'avait tu un meilleur contrat, etc
		uint32_t totalIMP_NS;
		uint32_t totalIMP_AI_NS;
		HumanPlayer players[4];
		Vulnerability vulnerability;
		Contract contract;
		Position dealer;
		vector<Bid> bidWar;
		vector<Card> playedCardsHistory;
		Contract bid();
		void playCards();
		void deal();
		
	protected:
	public:
		GameType getGameType();
		uint8_t getVulnerability();
		void setVulnerability();
		Game();
		void prepareForNextGame();
		Position whoWinsTheTrick(Card playedCards[], Position firstPlayer) const;
};

class LocalGame: public Game
{
	private:
	protected:
	public:
};

class NetworkGame: public Game
{
	private:
		uint32_t totalIMP_EW;
		uint32_t totalIMP_AI_EW;
	protected:
	public:
};

Position nextPosition(Position p);
Suit charToSuit(char stringSuit);
string suitToString(Suit suit);
string positionToString(Position position);
void generateScoringChart();