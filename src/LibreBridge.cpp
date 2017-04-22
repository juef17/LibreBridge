#include <cstdint>
#include <cstring>
#include <vector>
#include "LibreBridge.hpp"
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