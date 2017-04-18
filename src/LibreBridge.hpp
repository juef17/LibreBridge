#ifndef LIBREBRIDGE_HPP
#define LIBREBRIDGE_HPP

// Card values are 2-14, to be interpreted into T,J,Q,K,A with Card::valueToChar
enum Suit {Clubs=1, Diamonds=2, Hearts=3, Spades=4, NoTrump=5};
enum GameType {Local, Network};
enum Position {North=0, East=1, South=2, West=3};
enum BetType {Pass, Normal, Double, Redouble, Invalid};
enum Vulnerability {None=0, NS=1, EW=2, Both=3};

struct options
{
	int AI_playDelay = 500;	// milliseconds
};

#endif