#include <cstdint>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include "LibreBridge.hpp"
#include "Bid.hpp"
#include "Card.hpp"
#include "Contract.hpp"
#include "Player.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include "Game.hpp"
#include "Misc.hpp"

using namespace std;

GameType Game::getGameType()
{
	return gameType;
}

void Game::deal()
{
	vector<Card> deck;
	
	for(int i = 1; i<5; ++i) for(int j = 2; j<15; ++j)
	{
		Card card(j, Suit(i));
		deck.push_back(card);
	}
	
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	auto engine = default_random_engine{seed};
	shuffle(begin(deck), end(deck), engine);
	
	for(int i=0; i<4; ++i) 
	{
		players[i].setPosition(Position(i));
		players[i].clearHand();
		for(int j = 0; j<13; ++j)
		{
			players[i].addCardToHand(deck.back());
			deck.pop_back();
        }
		players[i].sortHand();
		players[i].printHand(' ');
	}
	
	//TODO remove
	//cout << to_string(players[0].countHonorPoints()) << "\n";
	//cout << to_string(players[0].countHonorPoints(Clubs)) << "\n";*/
}

uint8_t Game::getVulnerability()
{
	return vulnerability;
}

void Game::setVulnerability()
{
	vulnerability = Vulnerability((vulnerability + 1) % 4);
}

Contract Game::bid()
{
	uint8_t firstBidsTable[2][5];	// 1st index is team %2, 2nd is suit
	for(uint8_t i = 0; i<2; ++i) for(uint8_t j = 1; j<6; ++j) firstBidsTable[i][j] = 10; // Mark as unset

	bool atLeastOneBidMade = false;
	uint8_t numberOfPass = 0;
	uint8_t lastBidMade; // team % 2 who bet last
	uint8_t lastLevel = 0;
	Suit lastSuit;
	Position player = dealer;
	bool doubled = false, redoubled = false;
	Contract contract;
	
	while(numberOfPass < 3 || (!atLeastOneBidMade && numberOfPass < 4))
    {
		string playerInput;
        Bid bid;
		do
		{
			cout << positionToString(player) << ", please enter your bid: ";
			getline (cin, playerInput);
			bid.setBid(playerInput, player, lastLevel, lastSuit, doubled, redoubled);
			if(bid.getBetType() != Invalid) bidWar.push_back(bid);
			else cout << "Invalid bet!\n";
		} while (bid.getBetType() == Invalid);
		if(bid.getBetType() == Pass) numberOfPass++;
		else numberOfPass = 0;
		if(bid.getBetType() == Normal)
		{
			lastBidMade = player%2;
			lastSuit = bid.getSuit();
			lastLevel = bid.getLevel();
			atLeastOneBidMade = true;
			if(firstBidsTable[player%2][lastSuit] == 10) firstBidsTable[player%2][lastSuit] = player;
			doubled = false;
			redoubled = false;
		}
		if(bid.getBetType() == Double) doubled = true;
		if(bid.getBetType() == Redouble) redoubled = true;
		player = nextPosition(player);
	}
	if(numberOfPass == 4)
	{
		contract.setContract(0, NoTrump, North, false, false, vulnerability);
		return contract;
	}
    contract.setContract(lastLevel, lastSuit, Position(firstBidsTable[lastBidMade][lastSuit]), doubled, redoubled, vulnerability);
	return contract;
}

void Game::prepareForNextGame()
{
	dealer = nextPosition(dealer);
	setVulnerability();
	for(uint8_t i = 0; i<4; ++i) players[i].clearHand();
	bidWar.clear();
}

Position Contract::getDeclarer()
{
	return declarer;
}

Suit Contract::getSuit() const
{
	return suit;
}

uint8_t Contract::getLevel() const
{
	return level;
}

bool Contract::isTeamVulnerable(Position p) const
{
	switch(vulnerability)
	{
		case None: return false;
		case Both: return true;
		case NS: return (p == North || p == South);
		case EW: return (p == East || p == West);
		default: return false;
	}
}

Game::Game()
{
	// Generate random vulnerability and dealer
	default_random_engine generator;
	uniform_int_distribution<int> distribution(0,3);
	vulnerability = Vulnerability(distribution(generator));
	dealer = Position(distribution(generator));
	do
	{
		deal();
		contract = bid();
		cout << "Contract is: ";
		contract.print();
		if(!contract.getLevel()) continue;
		playCards();
		prepareForNextGame();
	} while(true);
}

void Game::playCards()
{
	Position player = nextPosition(contract.getDeclarer());
	Card playedCards[4];
	Position whoWonTheTrick;
	uint8_t tricksMade[2] = {0}; // %2 for team

	for(uint8_t i=0; i<4; ++i)
	{
		players[i].sortHand(contract.getSuit());
		players[i].printHand(' ');
	}
	for(uint8_t i=0; i<13; ++i)
	{
		playedCards[0] = players[player].playCard(NoTrump);
		Position firstPlayer = player;
		for(uint8_t j = 1; j<4; ++j)
		{
			player = nextPosition(player);
			playedCards[j] = players[player].playCard(playedCards[0].getSuit());
		}
		for(uint8_t j = 0; j<4; ++j) playedCardsHistory.push_back(playedCards[j]);
		whoWonTheTrick = whoWinsTheTrick(playedCards, firstPlayer);
		player = whoWonTheTrick;
		cout << positionToString(player) << " won the trick!\n";
		tricksMade[whoWonTheTrick%2]++;
	}
	cout << "NS has made " << to_string(tricksMade[0]) << " tricks and " << contract.calculateScore(North, tricksMade[0]) << " points (" << Contract::calculateIMP(contract.calculateScore(North, tricksMade[0])) << " IMP)!\n";
	cout << "EW has made " << to_string(tricksMade[1]) << " tricks and " << contract.calculateScore(East, tricksMade[1]) << " points (" << Contract::calculateIMP(contract.calculateScore(East, tricksMade[1])) << " IMP)!\n";
	cout << "----------------- New game -----------------\n";
}

Position Game::whoWinsTheTrick(Card playedCards[], Position firstPlayer) const
{
	uint8_t winnerIndex = 0;
	for(uint8_t i=1; i<4; ++i)
	{
		if(	(playedCards[i].getSuit() == playedCards[winnerIndex].getSuit() && playedCards[i].getValue() > playedCards[winnerIndex].getValue())
		||	(playedCards[i].getSuit() == contract.getSuit() && playedCards[winnerIndex].getSuit() != contract.getSuit())	)
		{
			winnerIndex = i;
		}
	}
	return Position((uint8_t(firstPlayer)+winnerIndex)%4);
}
