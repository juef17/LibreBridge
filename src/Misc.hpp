#ifndef MISC_HPP
#define MISC_HPP

#include <cstdint>
#include <string>
#include "LibreBridge.hpp"


Position nextPosition(Position p);
Position nextTeammate(Position p);
Suit charToSuit(char stringSuit);
std::string suitToString(Suit suit);
std::string positionToString(Position position);
void generateScoringChart();
uint8_t randomUint8(uint8_t min, uint8_t max, RANDOMNESS_SIZE seed = 0);
void initializeOptions();
bool areDealConstraintsValid();
std::string intToTeamString(uint8_t i);
void setSeed(RANDOMNESS_SIZE s);
void incrementSeed();
RANDOMNESS_SIZE getSeed();

#endif