#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include "LibreBridge.hpp"
#include "Bid.hpp"
#include "Card.hpp"
#include "Contract.hpp"
#include "Player.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include "Misc.hpp"
#include "Game.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	if(argc == 1) Game game;
	else if(argc > 1)
	{
		for(uint8_t i=1; i<argc; i++)
		{
			if(!strcmp(argv[i], "-C")) generateScoringChart();
		}
	}
	return 0;
}