#ifndef GAME_HPP
#define GAME_HPP

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