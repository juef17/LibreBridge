#ifndef MISC_HPP
#define MISC_HPP

Position nextPosition(Position p);
Suit charToSuit(char stringSuit);
std::string suitToString(Suit suit);
std::string positionToString(Position position);
void generateScoringChart();
uint8_t randomUint8(int min, int max);

#endif