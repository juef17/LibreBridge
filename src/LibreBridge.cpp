#include <cstdint>
#include <cstring>
#include "LibreBridge.hpp"
#include "Misc.hpp"
#include "Game.hpp"

using namespace std;

Options options;

int main(int argc, char *argv[])
{
	options.AI_playDelay = 0;
	options.AI_letGamesRun = false;
	for(uint8_t i=0; i<4; i++) options.playerTypes[i] = "AI_Random";

	for(uint8_t i=1; i<argc; i++)
	{
		if(!strcmp(argv[i], "-C"))
		{
			generateScoringChart();
			return 0;
		}
		else if(!strcmp(argv[i], "-PN") && i+1<argc) options.playerTypes[North] = argv[i++];
		else if(!strcmp(argv[i], "-PE") && i+1<argc) options.playerTypes[East] = argv[i++];
		else if(!strcmp(argv[i], "-PS") && i+1<argc) options.playerTypes[South] = argv[i++];
		else if(!strcmp(argv[i], "-PW") && i+1<argc) options.playerTypes[West] = argv[i++];
	}
	
	Game game;
	return 0;
}