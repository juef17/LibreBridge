#ifndef GAME_HPP
#define GAME_HPP

class Game
{
	private:
		GameType gameType;
		uint32_t totalIMP_NS;
		uint32_t totalIMP_AI_NS;
		Player* players[4];
		Vulnerability vulnerability;
		Contract contract;
		Position dealer;
		std::vector<Bid> bidWar;
		std::vector<Card> playedCardsHistory;
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

#endif