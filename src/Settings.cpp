#include <iostream>
#include <filesystem>
#include <cstdlib>
#include "external/toml.hpp" // A single-file third-party header
#include <fstream>
#include "Misc.hpp"
#include "Settings.hpp"

std::filesystem::path getConfigDirectory()
{
	std::filesystem::path configPath;

#if defined(_WIN32)
	// Windows: C:\Users\<User>\AppData\Roaming
	const char* appData = std::getenv("APPDATA");
	if(appData)	configPath = std::filesystem::path(appData);
	else		configPath = std::filesystem::path(std::getenv("USERPROFILE")) / "AppData" / "Roaming";

#elif defined(__APPLE__)
	// macOS: /Users/<User>/Library/Application Support
	const char* home = std::getenv("HOME");
	if(home)	configPath = std::filesystem::path(home) / "Library" / "Application Support";
    // else... I don't know, I've never touched an Apple device
	
#else
	// Linux/BSD/Other
	const char* xdgConfig = std::getenv("XDG_CONFIG_HOME");
	if(xdgConfig && *xdgConfig != '\0')    configPath = std::filesystem::path(xdgConfig);
	else
	{
		const char* home = std::getenv("HOME");
		if(home)	configPath = std::filesystem::path(home) / ".config";
		else		configPath = std::filesystem::current_path(); // No home? 🤷 Let's just dump this here then
	}
#endif

	configPath /= "librebridge";
	std::filesystem::create_directories(configPath);

	return configPath; // Something like "/home/user/.config/librebridge/"
}

void saveSettings()
{
	std::filesystem::path settingsFile = getConfigDirectory() / "settings.toml";
	toml::table settings;

    settings.insert("AI", toml::table{
        {"playDelay", options.AI_playDelay},
        {"bidDelay", options.AI_bidDelay},
        {"letGamesRun", options.AI_letGamesRun}
    });

    toml::array playerTypesArr;
    for (int i = 0; i < 4; i++)
        playerTypesArr.push_back(options.playerTypes[i]);
    settings.insert("playerTypes", playerTypesArr);

    settings.insert("seedIncrementStep", options.seedIncrementStep);
    settings.insert("useDealConstraints", options.useDealConstraints);

    toml::array pHonorMin, pHonorMax, pVoidsMin, pVoidsMax, pLongMin, pLongMax;
    for (int i = 0; i < 4; i++)
    {
        pHonorMin.push_back(options.constraints.playerHonorPointsMin[i]);
        pHonorMax.push_back(options.constraints.playerHonorPointsMax[i]);
        pVoidsMin.push_back(options.constraints.playerVoidsMin[i]);
        pVoidsMax.push_back(options.constraints.playerVoidsMax[i]);
        pLongMin.push_back(options.constraints.playerLongestSuitCountMin[i]);
        pLongMax.push_back(options.constraints.playerLongestSuitCountMax[i]);
    }

    toml::array tHonorMin, tHonorMax, tVoidsMin, tVoidsMax, tLongMin, tLongMax;
    for (int i = 0; i < 2; i++)
    {
        tHonorMin.push_back(options.constraints.teamHonorPointsMin[i]);
        tHonorMax.push_back(options.constraints.teamHonorPointsMax[i]);
        tVoidsMin.push_back(options.constraints.teamVoidsMin[i]);
        tVoidsMax.push_back(options.constraints.teamVoidsMax[i]);
    }

    settings.insert("constraints", toml::table{
        {"playerHonorPointsMin", pHonorMin},
        {"playerHonorPointsMax", pHonorMax},
        {"playerVoidsMin", pVoidsMin},
        {"playerVoidsMax", pVoidsMax},
        {"playerLongestSuitCountMin", pLongMin},
        {"playerLongestSuitCountMax", pLongMax},
        {"teamHonorPointsMin", tHonorMin},
        {"teamHonorPointsMax", tHonorMax},
        {"teamVoidsMin", tVoidsMin},
        {"teamVoidsMax", tVoidsMax}
    });

    settings.insert("autoplaySingles", options.autoplaySingles);
    settings.insert("minimalWait", options.minimalWait);
    settings.insert("waitAfterTrick", options.waitAfterTrick);

    settings.insert("theme", toml::table{
        {"cards", options.theme_cards},
        {"emphasisColor", options.theme_emphasisColor},
        {"flashingDelay", options.theme_flashingDelay}
    });

	std::ofstream ofs(settingsFile);
	ofs << settings;
}

void loadSettings()
{
	std::filesystem::path settingsFile = getConfigDirectory() / "settings.toml";
	
	if(!std::filesystem::exists(settingsFile))
    {
        // File doesn't exist yet, load hardcoded defaults
		//return;
        
        // Create the file, we'll set sensible default values below
        std::ofstream ofs(settingsFile);
        ofs.close();
    }

	auto config = toml::parse_file(settingsFile.string());
	
	// Sets sensible default values if the settings file has nothing
    
	options.AI_playDelay = config["AI"]["playDelay"].value_or(150);
    options.AI_bidDelay = config["AI"]["bidDelay"].value_or(150);
    options.AI_letGamesRun = config["AI"]["letGamesRun"].value_or(false);
    
    for(int i=0; i<4; i++) options.playerTypes[i] = config["playerTypes"][i].value_or("AI_Random");
    options.playerTypes[South] = config["playerTypes"][South].value_or("HumanPlayer");
    
    randomizeSeed();
	options.seedIncrementStep = config["seedIncrementStep"].value_or(1);
    
    options.useDealConstraints = config["useDealConstraints"].value_or(false);
    for(int i=0; i<4; i++)
    {
        options.constraints.playerHonorPointsMin[i] = config["constraints"]["playerHonorPointsMin"][i].value_or(0);
        options.constraints.playerHonorPointsMax[i] = config["constraints"]["playerHonorPointsMax"][i].value_or(40);
        options.constraints.playerVoidsMin[i] = config["constraints"]["playerVoidsMin"][i].value_or(0);
        options.constraints.playerVoidsMax[i] = config["constraints"]["playerVoidsMax"][i].value_or(3);
        options.constraints.playerLongestSuitCountMin[i] = config["constraints"]["playerLongestSuitCountMin"][i].value_or(4);
        options.constraints.playerLongestSuitCountMax[i] = config["constraints"]["playerLongestSuitCountMax"][i].value_or(13);
    }
    for(int i=0; i<2; i++)
    {
        options.constraints.teamHonorPointsMin[i] = config["constraints"]["teamHonorPointsMin"][i].value_or(0);
        options.constraints.teamHonorPointsMax[i] = config["constraints"]["teamHonorPointsMax"][i].value_or(40);
        options.constraints.teamVoidsMin[i] = config["constraints"]["teamVoidsMin"][i].value_or(0);
        options.constraints.teamVoidsMax[i] = config["constraints"]["teamVoidsMax"][i].value_or(6);
    }
    
    options.autoplaySingles = config["autoplaySingles"].value_or(true);
    options.minimalWait = config["minimalWait"].value_or(0);
    options.waitAfterTrick = config["waitAfterTrick"].value_or(false);
    
    options.theme_cards = config["theme"]["cards"].value_or("default");
    options.theme_emphasisColor = config["theme"]["emphasisColor"].value_or("#ffff80");
    options.theme_flashingDelay = config["theme"]["flashingDelay"].value_or(100);
}