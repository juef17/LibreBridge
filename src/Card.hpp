#ifndef CARD_HPP
#define CARD_HPP

#include <cstdint>
#include <string>
#include "LibreBridge.hpp"

class Card
{
	private:
		Suit suit;
		uint8_t value;
	protected:
	public:
		Card(uint8_t v, Suit s);
		Card();
		static char valueToChar(uint8_t value);
		static uint8_t charToValue(char c);
		static char suitToChar(Suit suit);
		
		void printCard();
		// TODO: add when g++ supports u8 character literals from c++17, see http://en.cppreference.com/w/cpp/language/character_literal
		// TODO: and convert every string litteral to u8
		/*char cardToUnicode()
		{
			return this->cardToUnicode(this);
		}
		static char cardToUnicode(Card card)
		{
			return this->cardToUnicode(card.value, card.suit);
		}
		static char cardToUnicode(uint8_t value, Suit suit)
		{
			return u8'a';
		}*/
		bool operator == (const Card& card);
		bool operator < (const Card& card);
		uint8_t getValue() const;
		Suit getSuit() const;
		static Card stringToCard(std::string s);
		std::string getImagePath();
};

#endif