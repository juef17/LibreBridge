#ifndef LIBREBRIDGE_HPP
#define LIBREBRIDGE_HPP

#include <string>
#include <vector>

// Card values are 2-14, to be interpreted into T,J,Q,K,A with Card::valueToChar
#define RANDOMNESS_SIZE unsigned long long
#define RANDOMNESS_QT_SIZE toULongLong
enum Suit {Clubs=1, Diamonds=2, Hearts=3, Spades=4, NoTrump=5};
enum GameType {Local, Network};
enum Position {North=0, East=1, South=2, West=3};
enum BetType {Pass, Normal, Double, Redouble, Invalid};
enum Vulnerability {None=0, NS=1, EW=2, Both=3};

struct DealConstraints
{
	uint8_t playerHonorPointsMin[4];	// Allowed number of honor points
	uint8_t playerHonorPointsMax[4];
	uint8_t teamHonorPointsMin[2];
	uint8_t teamHonorPointsMax[2];
	
	uint8_t playerVoidsMin[4];	// Allowed number of voids
	uint8_t playerVoidsMax[4];
	uint8_t teamVoidsMin[2];
	uint8_t teamVoidsMax[2];
	
	uint8_t playerLongestSuitCountMin[4];	// Number of cards for the longest suit in this player's hand
	uint8_t playerLongestSuitCountMax[4];
	
	// TODO add:
	// players: nbSingletons, nbDoubletons, nbAces, nbKings, nb5+Suit, nbCardsLongestSuit, atLeast1PlayerWithXpts
	// teams: 	nbSingletons, nbDoubletons, nbAces, nbKings, nb7+Fits, nb8+Fits, atLeast1TeamWithXpts
};

struct Options
{
	int AI_playDelay;							// Milliseconds before each AI play
	int AI_bidDelay;							// Milliseconds before each AI bid
	bool AI_letGamesRun;						// If true, no user input between AI-only games (then overrides AI_playDelay and AI_bidDelay)
	
	std::string playerTypes[4];					// Contains "Human" (or "HumanPlayer") or the name of the AI to use
	std::vector<std::string> AI_availableTypes;	// Contains the names of the usable AI players
	
	RANDOMNESS_SIZE seed;		// Seed as passed by command line argument (or user input), to use in Game(). If not, generated from chrono
	int seedIncrementStep;		// Usually +1, unless we're going backwards then it's -1
	
	bool useDealConstraints;	// Use DealConstraints to make deals not 100% random
	DealConstraints constraints;// Constraints to use for deals
	
	bool autoplaySingles;		// If there's a single possible card to play, play it automatically. GUI only.
	int minimalWait;			// Wait time for autoplaySingles
	bool waitAfterTrick;		// Wait for input after a trick
	
	bool useGui;						// False if we're running in text-only
	std::string theme_cards;			// Set of cards we're using
	std::string theme_emphasisColor;	// Color to replace white for hints / winning card, etc
	int theme_flashingDelay = 100;		// How many milliseconds between any color changing on the cards
} extern options;

#endif