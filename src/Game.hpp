#ifndef GAME_HPP
#define GAME_HPP

#include <cstdint>
#include <vector>
#include "LibreBridge.hpp"
#include "Bid.hpp"
#include "Card.hpp"
#include "Contract.hpp"
#include "Player.hpp"

class Game
{
	private:
		GameType gameType;
		uint32_t totalIMP[2];
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
		Game();
		void prepareForNextGame();
		Position whoWinsTheTrick(Card playedCards[], Position firstPlayer) const;
		bool isAllAI() const;
		bool areConstraintsRespected() const;
		Player** getPlayers();
		void findNextDeal();
		std::vector<Bid> getBidWar() const;
		Position getDealer() const;
		void addBid(Bid bid);
		Vulnerability getVulnerability() const;
		void updateVulnerabilityAndDealer();
		void setContract(Contract c);
		Contract getContract() const;
		Position getPositionFromCard(Card c, bool mayHaveBeenPlayed = false) const;
		std::vector<Card> recreateHand(Position p) const;
		void addCardToPlayHistory(Card c);
		Position whoseTurnIsItToPlay() const;
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
	protected:
	public:
};

#endif