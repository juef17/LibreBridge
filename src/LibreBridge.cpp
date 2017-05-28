#include <cstdint>
#include <cstring>
#include <cmath>
#include <iostream>
#include <QApplication>
#include "LibreBridge.hpp"
#include "Misc.hpp"
#include "Game.hpp"
#include "gui/WelcomeWindow.hpp"

using namespace std;

Options options;

int main(int argc, char *argv[])
{
	initializeOptions();
	options.useGui = (argc <= 1);
	
	if(options.useGui)
	{
		if(!DEBUG_COUT) cout.setstate(ios_base::failbit); // ignore console output
		QApplication libreBridgeGUI(argc, argv);
		
		WelcomeWindow welcomeWindow;
		
		return libreBridgeGUI.exec();
	}
	
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
		else
		{
			//TODO display command line help
			cout << "\nLibreBridge version " << to_string(VERSION_MAJOR) << "." << to_string(VERSION_MINOR) << "." << to_string(VERSION_BUILD) << "\n\n";
			return 0;
		}
	}
	
	Game game;
	return 0;
}