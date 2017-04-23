#include <cstdint>
#include <cstring>
#include "LibreBridge.hpp"
#include "Misc.hpp"
#include "Game.hpp"

using namespace std;

Options options;

int main(int argc, char *argv[])
{
	initializeOptions();
	for(uint8_t i=1; i<argc; i++)
	{
		if(!strcmp(argv[i], "-C"))
		{
			generateScoringChart();
			return 0;
		}
		else if(!strcmp(argv[i], "-PN") && i+1<argc) options.playerTypes[North] = argv[++i];
		else if(!strcmp(argv[i], "-PE") && i+1<argc) options.playerTypes[East] = argv[++i];
		else if(!strcmp(argv[i], "-PS") && i+1<argc) options.playerTypes[South] = argv[++i];
		else if(!strcmp(argv[i], "-PW") && i+1<argc) options.playerTypes[West] = argv[++i];
		else if(!strcmp(argv[i], "-S") && i+1<argc) options.seed = atol(argv[++i]);
	}
	
	Game game;
	return 0;
}