#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <limits>
#include <random>
#include "Game.hpp"
#include "HumanPlayer.hpp"
#include "Misc.hpp"

using namespace std;

Game::Game()
{
	bool keepPlaying = false;
	// Generate random vulnerability and dealer
	vulnerability = Vulnerability(randomUint8(0, 3, getSeed()));
	dealer = Position(randomUint8(0, 3, getSeed()));
	if(!areDealConstraintsValid()) options.useDealConstraints = false;
	
	for(uint8_t i=0; i<4; i++) players[i] = Player::getNewPlayer(options.playerTypes[i]);
	if(!options.useGui)
	{
		do
		{
			findNextDeal();
			for(uint8_t i=0; i<4; i++)
			{
				if(players[i]->getIsHuman()) players[i]->printHand(' ');
			}
			contract = bid();
			cout << "Contract is: ";
			contract.print();
			cout << "Deal #" << to_string(getSeed()) << endl;
			if(contract.getLevel()) playCards();
			prepareForNextGame();
		} while(keepPlaying || !options.useGui);
		for(uint8_t i=0; i<4; i++) delete players[i];
	}
}

GameType Game::getGameType()
{
	return gameType;
}

void Game::deal()
{
	vector<Card> deck;
	
	for(uint8_t i = 1; i<5; ++i) for(uint8_t j = 2; j<15; ++j)
	{
		Card card(j, Suit(i));
		deck.push_back(card);
	}
	
	auto engine = default_random_engine{getSeed()};
	shuffle(begin(deck), end(deck), engine);
	
	for(uint8_t i=0; i<4; ++i) 
	{
		players[i]->setPosition(Position(i));
		players[i]->clearHand();
		for(uint8_t j = 0; j<13; ++j)
		{
			players[i]->addCardToHand(deck.back());
			deck.pop_back();
		}
		players[i]->sortHand();
	}
}

void Game::setVulnerability()
{
	vulnerability = Vulnerability((vulnerability + 1) % 4);
}

Contract Game::bid()
{
	uint8_t firstBidsTable[2][6];	// 1st index is team %2, 2nd is suit
	for(uint8_t i = 0; i<2; ++i) for(uint8_t j = 1; j<6; ++j) firstBidsTable[i][j] = 10; // Mark as unset (10)

	bool atLeastOneBidMade = false;
	uint8_t numberOfPass = 0;
	uint8_t lastBidMade = 0; // team % 2 who bet last
	uint8_t lastLevel = 0;
	Suit lastSuit = NoTrump;
	Position playerPos = dealer;
	Position playerWhoBetNormallyLast = dealer;
	bool lastDoubled = false, lastRedoubled = false;
	Contract contract;
	
	while(numberOfPass < 3 || (!atLeastOneBidMade && numberOfPass < 4))
    {
		Bid bid;
		do
		{
			players[playerPos]->bid(bid, lastLevel, lastSuit, lastDoubled, lastRedoubled, playerWhoBetNormallyLast, bidWar);
			if(bid.getBetType() == Invalid) cout << "Invalid bet!\n";
		} while (bid.getBetType() == Invalid);
		if(!players[playerPos]->getIsHuman())
		{
			if(options.AI_playDelay) this_thread::sleep_for(chrono::milliseconds(options.AI_playDelay));
			cout << positionToString(playerPos) << ": " << bid.toString() << "\n";
		}
		bidWar.push_back(bid);
		if(bid.getBetType() == Pass) numberOfPass++;
		else numberOfPass = 0;
		if(bid.getBetType() == Normal)
		{
			playerWhoBetNormallyLast = playerPos;
			lastBidMade = playerPos%2;
			lastSuit = bid.getSuit();
			lastLevel = bid.getLevel();
			atLeastOneBidMade = true;
			if(firstBidsTable[playerPos%2][lastSuit] == 10) firstBidsTable[playerPos%2][lastSuit] = playerPos;
			lastDoubled = false;
			lastRedoubled = false;
		}
		if(bid.getBetType() == Double) lastDoubled = true;
		if(bid.getBetType() == Redouble) lastRedoubled = true;
		playerPos = nextPosition(playerPos);
	}
	if(numberOfPass == 4)
	{
		contract.setContract(0, NoTrump, North, false, false, vulnerability);
		return contract;
	}
	contract.setContract(lastLevel, lastSuit, Position(firstBidsTable[lastBidMade][lastSuit]), lastDoubled, lastRedoubled, vulnerability);
	return contract;
}

void Game::prepareForNextGame()
{
	incrementSeed();
	bool wasBiddingDone = !bidWar.empty();
	dealer = nextPosition(dealer);
	setVulnerability();
	for(uint8_t i = 0; i<4; ++i) players[i]->clearHand();
	bidWar.clear();
	playedCardsHistory.clear();
	if(options.useGui) return;
	if(!options.AI_letGamesRun && isAllAI())
	{
		if(wasBiddingDone)
		{
			cout << "Press 'Enter' to continue: ";
			cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
		}
	}
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

void Game::playCards()
{
	Position player = nextPosition(contract.getDeclarer());
	Position dummyPosition = nextTeammate(contract.getDeclarer());
	Card playedCards[4];
	Position whoWonTheTrick;
	uint8_t tricksMade[2] = {0}; // %2 for team
	bool isDummy;

	for(uint8_t i=0; i<4; ++i)
	{
		isDummy = (players[i]->getPosition() == dummyPosition);
		players[i]->sortHand(contract.getSuit());
		if(players[i]->getIsHuman() || isDummy) players[i]->printHand(' ');
	}
	for(uint8_t i=0; i<13; ++i)
	{
		Position firstPlayer = player;
		for(uint8_t j = 0; j<4; ++j)
		{
			Suit firstSuit = (j == 0 ? NoTrump : playedCards[0].getSuit());
			isDummy = (player == dummyPosition);
			Position actualPlayer = (isDummy ? contract.getDeclarer() : player);
			if(options.AI_playDelay && !players[actualPlayer]->getIsHuman()) this_thread::sleep_for(chrono::milliseconds(options.AI_playDelay));
			do
			{
				playedCards[j] = (isDummy ? players[actualPlayer]->playCard(firstSuit, players[dummyPosition]->getHand()) : players[actualPlayer]->playCard(firstSuit));
			} while(!players[player]->hasCard(playedCards[j]) || !players[player]->isValidPlay(playedCards[j], firstSuit));
			player = nextPosition(player);
			playedCardsHistory.push_back(playedCards[j]);
			players[(j+firstPlayer)%4]->clearCard(playedCards[j]);
		}
		whoWonTheTrick = whoWinsTheTrick(playedCards, firstPlayer);
		player = whoWonTheTrick;
		cout << positionToString(player) << " won the trick!\n";
		tricksMade[whoWonTheTrick%2]++;
	}
	for(uint8_t i=0; i<2; ++i)
	{
		int16_t pts = contract.calculateScore(Position(i), tricksMade[i]);
		uint8_t imp = Contract::calculateIMP(pts);
		totalIMP[i] += imp;
		cout << intToTeamString(i) << " has made " << to_string(tricksMade[i]) << " tricks and " << to_string(pts) << " points (" << to_string(imp) << " IMP)!\n";
	}
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

bool Game::isAllAI() const
{
	for(uint8_t i=0; i<4; ++i) if(players[i]->getIsHuman()) return false;
	return true;
}

bool Game::areConstraintsRespected() const
{
	if(!options.useDealConstraints) return true; // If we don't use them, no need to respect them
	
	uint8_t teamHP[2] = {0};
	uint8_t teamV[2] = {0};
	
	for(uint8_t i=0; i<4; i++)
	{
		uint8_t hp = players[i]->countHonorPoints();
		uint8_t v = players[i]->countVoids();
		uint8_t longestSuitCount = players[i]->countLongestSuit();
		if(options.constraints.playerHonorPointsMin[i] > hp) return false;
		if(options.constraints.playerHonorPointsMax[i] < hp) return false;
		if(options.constraints.playerVoidsMin[i] > v) return false;
		if(options.constraints.playerVoidsMax[i] < v) return false;
		if(options.constraints.playerLongestSuitCountMin[i] > longestSuitCount || options.constraints.playerLongestSuitCountMax[i] < longestSuitCount) return false;
		teamHP[i%2] += hp;
		teamV[i%2] += v;
	}
	
	for(uint8_t i=0; i<2; i++)
	{
		if(options.constraints.teamHonorPointsMin[i] > teamHP[i]) return false;
		if(options.constraints.teamHonorPointsMax[i] < teamHP[i]) return false;
		if(options.constraints.teamVoidsMin[i] > teamV[i]) return false;
		if(options.constraints.teamVoidsMax[i] < teamV[i]) return false;
	}
	
	return true;
}

Player** Game::getPlayers()
{
	return players;
}

void Game::findNextDeal()
{
	bool constraintsOK;
	do
	{
		deal();
		constraintsOK = areConstraintsRespected();
		if(!constraintsOK) incrementSeed();
	} while(!constraintsOK);
}

vector<Bid> Game::getBidWar() const
{
	return bidWar;
}

Position Game::getDealer() const
{
	return dealer;
}

void Game::addBid(Bid bid)
{
	bidWar.push_back(bid);
}


Vulnerability Game::getVulnerability() const
{
	return vulnerability;
}