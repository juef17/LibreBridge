#include <string>
#include <iostream>
#include "Contract.hpp"
#include "Misc.hpp"

using namespace std;

int16_t Contract::calculateScore(Position pos, uint8_t tricks)
{
	if((int(pos) % 2) != int(declarer) % 2) tricks = 13-tricks;
	int16_t score = 0;
	int8_t underOrOverTricks = tricks - 6 - level;
	int8_t i = 0;
	bool vulnerable = isTeamVulnerable(declarer);
	if(underOrOverTricks < 0) // Contract was defeated
	{
		if(vulnerable)
		{
			if(!doubled) while(i-- > underOrOverTricks) score += 100;
			else if(!redoubled)
			{
				while(i > underOrOverTricks && i > -1)
				{
					score += 200;
					i--;
				}
				while(i-- > underOrOverTricks) score += 300;
			}
			else
			{
				while(i > underOrOverTricks && i > -1)
				{
					score += 400;
					i--;
				}
				while(i-- > underOrOverTricks) score += 600;
			}
		}
		else
		{
			if(!doubled) while(i-- > underOrOverTricks) score += 50;
			else if(!redoubled)
			{
				while(i > underOrOverTricks && i > -1)
				{
					score += 100;
					i--;
				}
				while(i > underOrOverTricks && i > -3)
				{
					score += 200;
					i--;
				}
				while(i-- > underOrOverTricks) score += 300;
			}
			else
			{
				while(i > underOrOverTricks && i > -1)
				{
					score += 200;
					i--;
				}
				while(i > underOrOverTricks && i > -3)
				{
					score += 400;
					i--;
				}
				while(i-- > underOrOverTricks) score += 600;
			}
		}
	}
	else // Contract was made
	{
		bool game = false;
		//Contract points
		while(i<level)
		{
			switch(suit)
			{
				case NoTrump:
				{
					score += (i==0 ? 40 : 30);
					break;
				}
				case Hearts:
				case Spades:
				{
					score += 30;
					break;
				}
				case Clubs:
				case Diamonds:
				{
					score += 20;
					break;
				}
				default: break;
			}
			i++;
		}
		if(doubled) score *= 2;
		if(redoubled) score *= 2;
		if(score >= 100) game = true;
		
		// Overtrick points
		while(i<underOrOverTricks+level)
		{
			if(!doubled)
			{
				switch(suit)
				{
					case NoTrump:
					case Hearts:
					case Spades:
					{
						score += 30;
						break;
					}
					case Clubs:
					case Diamonds:
					{
						score += 20;
						break;
					}
					default: break;
				}
			}
			else if(!redoubled) score += (vulnerable ? 200 : 100);
			else score += (vulnerable ? 400 : 200);
			i++;
		}
		
		// Slam bonus
		if(tricks == 13 && level == 7) score += (vulnerable ? 1500 : 1000);
		if(tricks >= 12 && level == 6) score += (vulnerable ? 750 : 500);
		
		// Doubled and redoubled bonus
		if(doubled) score += 50;
		if(redoubled) score += 50;
		
		// Game or part game bonus
		if(game) score += (vulnerable ? 500 : 300);
		else score += 50;
	}
	if(underOrOverTricks < 0 && (int(pos) % 2) == int(declarer) % 2) score *= -1; // This team didn't make the contract
	if(underOrOverTricks >= 0 && (int(pos) % 2) != int(declarer) % 2) score *= -1; // The other team made their contract
	return score;
}

uint8_t Contract::calculateIMP(int16_t score)
{
	if(score < 11) return 0;
	if(score < 41) return 1;
	if(score < 81) return 2;
	if(score < 121) return 3;
	if(score < 161) return 4;
	if(score < 211) return 5;
	if(score < 261) return 6;
	if(score < 311) return 7;
	if(score < 361) return 8;
	if(score < 421) return 9;
	if(score < 491) return 10;
	if(score < 591) return 11;
	if(score < 741) return 12;
	if(score < 891) return 13;
	if(score < 1091) return 14;
	if(score < 1291) return 15;
	if(score < 1491) return 16;
	if(score < 1741) return 17;
	if(score < 1991) return 18;
	if(score < 2241) return 19;
	if(score < 2491) return 20;
	if(score < 2991) return 21;
	if(score < 3491) return 22;
	if(score < 3991) return 23;
	return 24;
}

void Contract::setContract(uint8_t lev, Suit sui, Position pos, bool doubl, bool redoubl, Vulnerability vuln)
{
	level = lev;
	suit = sui;
	declarer = pos;
	doubled = doubl;
	redoubled = redoubl;
	vulnerability = vuln;
}

void Contract::print()
{
	if(!level) cout << "PASS";
	else
	{
		cout << to_string(level) << " " << suitToString(suit);
		if(redoubled) cout << " redoubled";
		else if(doubled) cout << " doubled";
		cout << " by " << positionToString(declarer) << " (" << (isTeamVulnerable(declarer) ? "" : "not ") << "vulnerable)";
	}
	cout << "\n";
}

string Contract::toString() const
{
	if(!level) return "All passed.";
	return	"Contract is: " + to_string(level) + " " + suitToString(suit) + (redoubled ? " redoubled" : (doubled ? " doubled" : "")) + "\n\n"
			+ " by " + positionToString(declarer) + " (" + (isTeamVulnerable(declarer) ? "" : "not ") + "vulnerable)";
}