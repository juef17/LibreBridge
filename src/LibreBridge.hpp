#ifndef LIBREBRIDGE_HPP
#define LIBREBRIDGE_HPP

#include <string>

// Card values are 2-14, to be interpreted into T,J,Q,K,A with Card::valueToChar
#define RANDOMNESS_SIZE unsigned long
enum Suit {Clubs=1, Diamonds=2, Hearts=3, Spades=4, NoTrump=5};
enum GameType {Local, Network};
enum Position {North=0, East=1, South=2, West=3};
enum BetType {Pass, Normal, Double, Redouble, Invalid};
enum Vulnerability {None=0, NS=1, EW=2, Both=3};

struct Options
{
	int AI_playDelay;			// Milliseconds before each AI bid or play
	bool AI_letGamesRun;		// If true, no user input between AI-only games
	std::string playerTypes[4];	// Contains "Human" or the name of the AI to use
	RANDOMNESS_SIZE seed;		// Seed as passed by command line argument, to use in Game(). 0 = generate from time
} extern options;

#endif