#ifndef CONTRACT_HPP
#define CONTRACT_HPP

class Contract
{
	private:
		uint8_t level; // If 0, all passed
		Suit suit;
		Position declarer;
		bool doubled;
		bool redoubled;
		Vulnerability vulnerability;
	protected:
	public:
		int16_t calculateScore(Position pos, uint8_t tricks);
		static uint8_t calculateIMP(int16_t score);
		void setContract(uint8_t lev, Suit sui, Position pos, bool doubl, bool redoubl, Vulnerability vuln);
		void print();
		Position getDeclarer();
		Suit getSuit() const;
		uint8_t getLevel() const;
		bool isTeamVulnerable(Position p) const;
};

#endif