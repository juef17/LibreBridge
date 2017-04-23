#include <iostream>
#include <chrono>
#include <algorithm>
#include "Misc.hpp"
#include "Card.hpp"
#include "Contract.hpp"

using namespace std;

Position nextPosition(Position p)
{
	return Position((p+1)%4);
}

Position nextTeammate(Position p)
{
	return Position((p+2)%4);
}

Suit charToSuit(char stringSuit)
{
	switch(stringSuit)
	{
		case 'C': return Clubs;
		case 'D': return Diamonds;
		case 'H': return Hearts;
		case 'S': return Spades;
		default: return NoTrump;
	}
}

string positionToString(Position position)
{
	switch(position)
	{
		case North: return "North";
		case East: return "East";
		case South: return "South";
		case West: return "West";
		default: return "ERROR";
	}
}

string suitToString(Suit suit)
{
	switch(suit)
	{
		case Clubs: return "Clubs";
		case Diamonds: return "Diamonds";
		case Hearts: return "Hearts";
		case Spades: return "Spades";
		case NoTrump: return "No Trump";
		default: return "ERROR";
	}
}

void generateScoringChart()
{
	cout << "<html><body><table border=1>";
		cout << "<tr>";
			cout << "<th>Result</th>";
			cout << "<th>NV</th>";
			cout << "<th>NVX</th>";
			cout << "<th>NVXX</th>";
			cout << "<th>V</th>";
			cout << "<th>VX</th>";
			cout << "<th>VXX</th>";
		cout << "</tr>";
		for(uint8_t level=1; level<8; ++level)
		{
			for(uint8_t suit = 1; suit<6; ++suit)
			{
				if(suit == 1 || suit == 3) ++suit; // No need for C and D, or H and S
				for(uint8_t made = level; made<8; ++made)
				{
					uint8_t vulnerable = 0;
					bool doubled;
					bool redoubled;
					cout << "<tr>";
						cout << "<td>" << to_string(level) << Card::suitToChar(Suit(suit)) << " made " << to_string(made) << "</td>";
						do
						{
							do
							{
								do
								{
									Contract c;
									c.setContract(level, Suit(suit), North, doubled, redoubled, Vulnerability(vulnerable));
									if(!(redoubled && !doubled)) cout << "<td>" << to_string(c.calculateScore(North, made+6)) << "</td>";
									redoubled = !redoubled;
								} while(redoubled);
								doubled = !doubled;
							} while(doubled);
							vulnerable = Vulnerability(1-vulnerable);
						} while(vulnerable);
					cout << "</tr>";
				}
			}
		}
		cout << "<tr>";
			cout << "<th>Down</th>";
			cout << "<th>NV</th>";
			cout << "<th>NVX</th>";
			cout << "<th>NVXX</th>";
			cout << "<th>V</th>";
			cout << "<th>VX</th>";
			cout << "<th>VXX</th>";
		cout << "</tr>";
		for(uint8_t down=1; down<14; ++down)
		{
			uint8_t vulnerable = 0;
			bool doubled;
			bool redoubled;
			cout << "<tr>";
				cout << "<td>" << to_string(down) << "</td>";
				do
				{
					do
					{
						do
						{
							Contract c;
							c.setContract(7, Clubs, North, doubled, redoubled, Vulnerability(vulnerable));
							if(!(redoubled && !doubled)) cout << "<td>" << to_string(c.calculateScore(East, down)) << "</td>";
							redoubled = !redoubled;
						} while(redoubled);
						doubled = !doubled;
					} while(doubled);
					vulnerable = Vulnerability(1-vulnerable);
				} while(vulnerable);
			cout << "</tr>";
		}
	cout << "</table></body></html>";
}

uint8_t randomUint8(uint8_t min, uint8_t max, RANDOMNESS_SIZE seed)
{
	seed = (seed ? seed : chrono::system_clock::now().time_since_epoch().count());
	default_random_engine generator(seed);
	uniform_int_distribution<uint8_t> distribution(min,max);
	return distribution(generator);
}