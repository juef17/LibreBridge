#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <filesystem>
#include "LibreBridge.hpp"

std::filesystem::path getConfigDirectory();
void saveSettings();
void loadSettings();

#endif