/* TODO:
	Lua AI scripting: http://www.rasterbar.com/products/luabind/docs.html
	Qt interface
	Windows / Android ports
*/

#include "LibreBridge.hpp"

using namespace std;

Card::Card(uint8_t v, Suit s)
{
	// TODO: faire les checks de sécurité
	value = v;
	suit = s;
}
Card::Card()
{
}
char Card::valueToChar(uint8_t value)
{
	// TODO: faire les checks de sécurité
	switch(value)
	{
		case 10: return 'T';
		case 11: return 'J';
		case 12: return 'Q';
		case 13: return 'K';
		case 14: return 'A';
		default: return '0' + value;
	}
}
uint8_t Card::charToValue(char c)
{
	// TODO: faire les checks de sécurité
	// TODO: optimiser un peu sti
	for(uint8_t i=0; i<15; i++) if(valueToChar(i) == c) return i;
	return 0;
}
char Card::suitToChar(Suit suit)
{
	// TODO: faire les checks de sécurité
	switch(suit)
	{
		case Clubs: return 'C';
		case Diamonds: return 'D';
		case Hearts: return 'H';
		case Spades: return 'S';
		case NoTrump: return 'N';
		default: return 'X';
	}
}

bool Card::operator == (const Card& card)
{
	return(suit == card.suit && value == card.value);
}
void Card::printCard()
{
	cout << valueToChar(value) << suitToChar(suit);
}
bool Card::operator < (const Card& card)
{
	if(suit < card.suit) return true;
	if(suit > card.suit) return false;
	return (value > card.value);
}
uint8_t Card::getValue() const
{
	return value;
}
Suit Card::getSuit() const
{
	return suit;
}
Card Card::stringToCard(string s)
{
	Card card(0, NoTrump);
	if(s.length() != 2) return card;
	uint8_t value = value = charToValue(s[0]);
	Suit suit = charToSuit(s[1]);
	if(!value) return card;
	if(suit == NoTrump) return card;
	Card goodCard(value, suit);
	return goodCard;
}
bool Player::hasCard(Card c) const
{
	for (auto &card : hand) if(c == card) return true;
	return false;
}
bool Player::isValidPlay(Card c, Suit firstSuit) const
{
	if(c.getSuit() == firstSuit) return true;
	for (auto &card : hand) if(card.getSuit() == firstSuit) return false;
	return true;
}

Card Player::playCard(Suit firstSuit) // If firstSuit is NoTrump, this is the first card to be played
{
	Card card;
	string playerInput;
	do
	{
		cout << positionToString(position) << ", please play a card: ";
		getline (cin, playerInput);
		if(playerInput.length()) card = Card::stringToCard(playerInput);
		else card = playRandomCard(firstSuit);
	} while (!card.getValue() || !hasCard(card) || !isValidPlay(card, firstSuit));
	hand.erase(std::remove(hand.begin(), hand.end(), card), hand.end());
	return card;
}
Card Player::playRandomCard(Suit firstSuit)
{
	vector<Card> playableCards = getPlayableCards(firstSuit);
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	auto engine = default_random_engine{seed};
	shuffle(begin(playableCards), end(playableCards), engine);
	cout << positionToString(position) << " is playing ";
	playableCards[0].printCard();
	cout << "\n";
	return playableCards[0];
}
vector<Card> Player::getPlayableCards(Suit firstSuit)
{
	vector<Card> playableCards;
	for (auto &card : hand) if(isValidPlay(card, firstSuit)) playableCards.push_back(card);
	return playableCards;
}
void Player::addCardToHand(Card card)
{
	hand.push_back(card);
}
void Player::printHand(char c)
{
	cout << setw(5) << positionToString(position);
	cout << ":" << c;
	for (auto &card : hand)
	{
		card.printCard();
		cout << c;
	}
	if(c == ' ') cout << "\n";
}
void Player::sortHand()
{
	sort(hand.begin(), hand.end());
}
void Player::sortHand(Suit suit)
{
	sort(hand.begin(), hand.end(),
		[suit](const Card& c1, const Card& c2)
		{
			if(c1.getSuit() == suit && c2.getSuit() != suit) return true;
			if(c1.getSuit() != suit && c2.getSuit() == suit) return false;
			if(c1.getSuit() < c2.getSuit()) return true;
			if(c1.getSuit() > c2.getSuit()) return false;
			return (c1.getValue() > c2.getValue());
		}
	);
}
uint8_t Player::countHonorPoints()
{
	uint8_t points = 0;
	for (auto &card : hand)
	{
		int8_t pts = card.getValue() - 10;
		if(pts > 0) points += pts;
	}
	return points;
}
uint8_t Player::countHonorPoints(Suit suit)
{
	uint8_t points = 0;
	for (auto &card : hand)
	{
		if(suit != card.getSuit()) continue;
		int8_t pts = card.getValue() - 10;
		if(pts > 0) points += pts;
	}
	return points;
}
void Player::setPosition(Position p)
{
	position = p;
}
void Player::clearHand()
{
	hand.clear();
}

int16_t Contract::calculateScore(Position pos, uint8_t tricks)
{
	if((int(pos) % 2) != int(declarer) % 2) tricks = 13-tricks;
	int16_t score = 0;
	int8_t underOrOverTricks = tricks - 6 - level;
	int8_t i = 0;
	bool vulnerable = isTeamVulnerable(declarer);
	if(underOrOverTricks < 0) // Contract was defeated
	{
		if(vulnerable)
		{
			if(!doubled) while(i-- > underOrOverTricks) score += 100;
			else if(!redoubled)
			{
				while(i > underOrOverTricks && i > -1)
				{
					score += 200;
					i--;
				}
				while(i-- > underOrOverTricks) score += 300;
			}
			else
			{
				while(i > underOrOverTricks && i > -1)
				{
					score += 400;
					i--;
				}
				while(i-- > underOrOverTricks) score += 600;
			}
		}
		else
		{
			if(!doubled) while(i-- > underOrOverTricks) score += 50;
			else if(!redoubled)
			{
				while(i > underOrOverTricks && i > -1)
				{
					score += 100;
					i--;
				}
				while(i > underOrOverTricks && i > -3)
				{
					score += 200;
					i--;
				}
				while(i-- > underOrOverTricks) score += 300;
			}
			else
			{
				while(i > underOrOverTricks && i > -1)
				{
					score += 200;
					i--;
				}
				while(i > underOrOverTricks && i > -3)
				{
					score += 400;
					i--;
				}
				while(i-- > underOrOverTricks) score += 600;
			}
		}
	}
	else // Contract was made
	{
		bool game = false;
		//Contract points
		while(i<level)
		{
			switch(suit)
			{
				case NoTrump:
				{
					score += (i==0 ? 40 : 30);
					break;
				}
				case Hearts:
				case Spades:
				{
					score += 30;
					break;
				}
				case Clubs:
				case Diamonds:
				{
					score += 20;
					break;
				}
				default: break;
			}
			i++;
		}
		if(doubled) score *= 2;
		if(redoubled) score *= 2;
		if(score >= 100) game = true;
		
		// Overtrick points
		while(i<underOrOverTricks+level)
		{
			if(!doubled)
			{
				switch(suit)
				{
					case NoTrump:
					case Hearts:
					case Spades:
					{
						score += 30;
						break;
					}
					case Clubs:
					case Diamonds:
					{
						score += 20;
						break;
					}
					default: break;
				}
			}
			else if(!redoubled) score += (vulnerable ? 200 : 100);
			else score += (vulnerable ? 400 : 200);
			i++;
		}
		
		// Slam bonus
		if(tricks == 13 && level == 7) score += (vulnerable ? 1500 : 1000);
		if(tricks >= 12 && level == 6) score += (vulnerable ? 750 : 500);
		
		// Doubled and redoubled bonus
		if(doubled) score += 50;
		if(redoubled) score += 50;
		
		// Game or part game bonus
		if(game) score += (vulnerable ? 500 : 300);
		else score += 50;
	}
	if(underOrOverTricks < 0 && (int(pos) % 2) == int(declarer) % 2) score *= -1; // This team didn't make the contract
	if(underOrOverTricks >= 0 && (int(pos) % 2) != int(declarer) % 2) score *= -1; // The other team made their contract
	return score;
}
uint16_t Contract::calculateIMP(int16_t score)
{
	if(score < 11) return 0;
	if(score < 41) return 1;
	if(score < 81) return 2;
	if(score < 121) return 3;
	if(score < 161) return 4;
	if(score < 211) return 5;
	if(score < 261) return 6;
	if(score < 311) return 7;
	if(score < 361) return 8;
	if(score < 421) return 9;
	if(score < 491) return 10;
	if(score < 591) return 11;
	if(score < 741) return 12;
	if(score < 891) return 13;
	if(score < 1091) return 14;
	if(score < 1291) return 15;
	if(score < 1491) return 16;
	if(score < 1741) return 17;
	if(score < 1991) return 18;
	if(score < 2241) return 19;
	if(score < 2491) return 20;
	if(score < 2991) return 21;
	if(score < 3491) return 22;
	if(score < 3991) return 23;
	return 24;
}
void Contract::setContract(uint8_t lev, Suit sui, Position pos, bool doubl, bool redoubl, Vulnerability vuln)
{
	level = lev;
	suit = sui;
	declarer = pos;
	doubled = doubl;
	redoubled = redoubl;
	vulnerability = vuln;
}
void Contract::print()
{
	if(!level) cout << "PASS";
	else
	{
		cout << to_string(level) << " " << suitToString(suit);
		if(redoubled) cout << " redoubled";
		else if(doubled) cout << " doubled";
		cout << " by " << positionToString(declarer) << " (" << (isTeamVulnerable(declarer) ? "" : "not ") << "vulnerable)";
	}
	cout << "\n";
}

GameType Game::getGameType()
{
	return gameType;
}
void Game::deal()
{
	vector<Card> deck;
	
	for(int i = 1; i<5; ++i) for(int j = 2; j<15; ++j)
	{
		Card card(j, Suit(i));
		deck.push_back(card);
	}
	
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	auto engine = default_random_engine{seed};
	shuffle(begin(deck), end(deck), engine);
	
	for(int i=0; i<4; ++i) 
	{
		players[i].setPosition(Position(i));
		players[i].clearHand();
		for(int j = 0; j<13; ++j)
		{
			players[i].addCardToHand(deck.back());
			deck.pop_back();
		}
		players[i].sortHand();
		players[i].printHand(' ');
	}
	
	//TODO remove
	//cout << to_string(players[0].countHonorPoints()) << "\n";
	//cout << to_string(players[0].countHonorPoints(Clubs)) << "\n";*/
}
uint8_t Game::getVulnerability()
{
	return vulnerability;
}
void Game::setVulnerability()
{
	vulnerability = Vulnerability((vulnerability + 1) % 4);
}
Contract Game::bid()
{
	uint8_t firstBidsTable[2][5];	// 1st index is team %2, 2nd is suit
	for(uint8_t i = 0; i<2; ++i) for(uint8_t j = 1; j<6; ++j) firstBidsTable[i][j] = 10; // Mark as unset

	bool atLeastOneBidMade = false;
	uint8_t numberOfPass = 0;
	uint8_t lastBidMade; // team % 2 who bet last
	uint8_t lastLevel = 0;
	Suit lastSuit;
	Position player = dealer;
	bool doubled = false, redoubled = false;
	Contract contract;
	
	while(numberOfPass < 3 || (!atLeastOneBidMade && numberOfPass < 4))
	{
		string playerInput;
		Bid bid;
		do
		{
			cout << positionToString(player) << ", please enter your bid: ";
			getline (cin, playerInput);
			bid.setBid(playerInput, player, lastLevel, lastSuit, doubled, redoubled);
			if(bid.getBetType() != Invalid) bidWar.push_back(bid);
			else cout << "Invalid bet!\n";
		} while (bid.getBetType() == Invalid);
		if(bid.getBetType() == Pass) numberOfPass++;
		else numberOfPass = 0;
		if(bid.getBetType() == Normal)
		{
			lastBidMade = player%2;
			lastSuit = bid.getSuit();
			lastLevel = bid.getLevel();
			atLeastOneBidMade = true;
			if(firstBidsTable[player%2][lastSuit] == 10) firstBidsTable[player%2][lastSuit] = player;
			doubled = false;
			redoubled = false;
		}
		if(bid.getBetType() == Double) doubled = true;
		if(bid.getBetType() == Redouble) redoubled = true;
		player = nextPosition(player);
	}
	if(numberOfPass == 4)
	{
		contract.setContract(0, NoTrump, North, false, false, vulnerability);
		return contract;
	}
	contract.setContract(lastLevel, lastSuit, Position(firstBidsTable[lastBidMade][lastSuit]), doubled, redoubled, vulnerability);
	return contract;
}
void Game::prepareForNextGame()
{
	dealer = nextPosition(dealer);
	setVulnerability();
	for(uint8_t i = 0; i<4; ++i) players[i].clearHand();
	bidWar.clear();
}

Position Contract::getDeclarer()
{
	return declarer;
}
Suit Contract::getSuit() const
{
	return suit;
}
uint8_t Contract::getLevel() const
{
	return level;
}
bool Contract::isTeamVulnerable(Position p) const
{
	switch(vulnerability)
	{
		case None: return false;
		case Both: return true;
		case NS: return (p == North || p == South);
		case EW: return (p == East || p == West);
		default: return false;
	}
}

Game::Game()
{
	// Generate random vulnerability and dealer
	default_random_engine generator;
	uniform_int_distribution<int> distribution(0,3);
	vulnerability = Vulnerability(distribution(generator));
	dealer = Position(distribution(generator));
	do
	{
		deal();
		contract = bid();
		cout << "Contract is: ";
		contract.print();
		if(!contract.getLevel()) continue;
		playCards();
		prepareForNextGame();
	} while(true);
}
void Game::playCards()
{
	Position player = nextPosition(contract.getDeclarer());
	Card playedCards[4];
	Position whoWonTheTrick;
	uint8_t tricksMade[2] = {0}; // %2 for team

	for(uint8_t i=0; i<4; ++i)
	{
		players[i].sortHand(contract.getSuit());
		players[i].printHand(' ');
	}
	for(uint8_t i=0; i<13; ++i)
	{
		playedCards[0] = players[player].playCard(NoTrump);
		Position firstPlayer = player;
		for(uint8_t j = 1; j<4; ++j)
		{
			player = nextPosition(player);
			playedCards[j] = players[player].playCard(playedCards[0].getSuit());
		}
		for(uint8_t j = 0; j<4; ++j) playedCardsHistory.push_back(playedCards[j]);
		whoWonTheTrick = whoWinsTheTrick(playedCards, firstPlayer);
		player = whoWonTheTrick;
		cout << positionToString(player) << " won the trick!\n";
		tricksMade[whoWonTheTrick%2]++;
	}
	cout << "NS has made " << to_string(tricksMade[0]) << " tricks and " << contract.calculateScore(North, tricksMade[0]) << " points (" << Contract::calculateIMP(contract.calculateScore(North, tricksMade[0])) << " IMP)!\n";
	cout << "EW has made " << to_string(tricksMade[1]) << " tricks and " << contract.calculateScore(East, tricksMade[1]) << " points (" << Contract::calculateIMP(contract.calculateScore(East, tricksMade[1])) << " IMP)!\n";
	cout << "----------------- New game -----------------\n";
}
Position Game::whoWinsTheTrick(Card playedCards[], Position firstPlayer) const
{
	uint8_t winnerIndex = 0;
	for(uint8_t i=1; i<4; ++i)
	{
		if(	(playedCards[i].getSuit() == playedCards[winnerIndex].getSuit() && playedCards[i].getValue() > playedCards[winnerIndex].getValue())
		||	(playedCards[i].getSuit() == contract.getSuit() && playedCards[winnerIndex].getSuit() != contract.getSuit())	)
		{
			winnerIndex = i;
		}
	}
	return Position((uint8_t(firstPlayer)+winnerIndex)%4);
}

void Bid::setBid(string stringBid, Position pla, uint8_t lastLevel, Suit lastSuit, bool doubled, bool redoubled)
{
	betType = Invalid;
	player = pla;
	if(stringBid == "X")
	{
		if(!lastLevel || doubled || redoubled) return;
		betType = Double;
	}
	else if(stringBid == "XX")
	{
		if(!lastLevel || !doubled || redoubled) return;
		betType = Redouble;
	}
	else if(stringBid == "Pass" || stringBid == "") betType = Pass;
	else
	{
		if(stringBid.length() < 2 || stringBid.length() > 3) return;
		level = stringBid[0];
		if(level < 0 || level > 7) return;
		if(stringBid.length() == 2 && (stringBid[1] == 'C' || stringBid[1] == 'D' || stringBid[1] == 'H' || stringBid[1] == 'S'))
		{
			suit = charToSuit(stringBid[1]);
			if(lastLevel > level) return;
			if(lastLevel == level && lastSuit >= suit) return;
			betType = Normal;
		}
		else if(stringBid.length() == 3 && stringBid[1] == 'N' && stringBid[2] == 'T')
		{
			suit = NoTrump;
			if(lastLevel > level) return;
			if(lastLevel == level && lastSuit == suit) return;
			betType = Normal;
		}
	}
}

void Bid::printBid()
{
	if(betType == Invalid)
	{
		cout << "Bid is invalid.";
		return;
	}
	cout << "Bid is: ";
	if(betType == Pass) cout << "Pass";
	if(betType == Double) cout << "Double";
	if(betType == Redouble) cout << "Redouble";
	if(betType == Normal) cout << "Pass";
	cout << " by " << positionToString(player) << "\n";
}
BetType Bid::getBetType()
{
	return betType;
}

Suit Bid::getSuit()
{
	return Suit(suit);
}

uint8_t Bid::getLevel()
{
	return level;
}

Position nextPosition(Position p)
{
	return Position((p+1)%4);
}

Suit charToSuit(char stringSuit)
{
	switch(stringSuit)
	{
		case 'C': return Clubs;
		case 'D': return Diamonds;
		case 'H': return Hearts;
		case 'S': return Spades;
		default: return NoTrump;
	}
}

string positionToString(Position position)
{
	switch(position)
	{
		case North: return "North";
		case East: return "East";
		case South: return "South";
		case West: return "West";
		default: return "ERROR";
	}
}

string suitToString(Suit suit)
{
	switch(suit)
	{
		case Clubs: return "Clubs";
		case Diamonds: return "Diamonds";
		case Hearts: return "Hearts";
		case Spades: return "Spades";
		case NoTrump: return "No Trump";
		default: return "ERROR";
	}
}

int main(int argc, char *argv[])
{
	if(argc == 1) Game game;
	else if(argc > 1)
	{
		for(uint8_t i=1; i<argc; i++)
		{
			if(!strcmp(argv[i], "-C"))
			{
				generateScoringChart();
			}
		}
	}

	return 0;
}

void generateScoringChart()
{
	cout << "<html><body><table border=1>";
		cout << "<tr>";
			cout << "<th>Result</th>";
			cout << "<th>NV</th>";
			cout << "<th>NVX</th>";
			cout << "<th>NVXX</th>";
			cout << "<th>V</th>";
			cout << "<th>VX</th>";
			cout << "<th>VXX</th>";
		cout << "</tr>";
		for(uint8_t level=1; level<8; ++level)
		{
			for(uint8_t suit = 1; suit<6; ++suit)
			{
				if(suit == 1 || suit == 3) ++suit; // No need for C and D, or H and S
				for(uint8_t made = level; made<8; ++made)
				{
					uint8_t vulnerable = 0;
					bool doubled;
					bool redoubled;
					cout << "<tr>";
						cout << "<td>" << to_string(level) << Card::suitToChar(Suit(suit)) << " made " << to_string(made) << "</td>";
						do
						{
							do
							{
								do
								{
									Contract c;
									c.setContract(level, Suit(suit), North, doubled, redoubled, Vulnerability(vulnerable));
									if(!(redoubled && !doubled)) cout << "<td>" << to_string(c.calculateScore(North, made+6)) << "</td>";
									redoubled = !redoubled;
								} while(redoubled);
								doubled = !doubled;
							} while(doubled);
							vulnerable = Vulnerability(1-vulnerable);
						} while(vulnerable);
					cout << "</tr>";
				}
			}
		}
		cout << "<tr>";
			cout << "<th>Down</th>";
			cout << "<th>NV</th>";
			cout << "<th>NVX</th>";
			cout << "<th>NVXX</th>";
			cout << "<th>V</th>";
			cout << "<th>VX</th>";
			cout << "<th>VXX</th>";
		cout << "</tr>";
		for(uint8_t down=1; down<14; ++down)
		{
			uint8_t vulnerable = 0;
			bool doubled;
			bool redoubled;
			cout << "<tr>";
				cout << "<td>" << to_string(down) << "</td>";
				do
				{
					do
					{
						do
						{
							Contract c;
							c.setContract(7, Clubs, North, doubled, redoubled, Vulnerability(vulnerable));
							if(!(redoubled && !doubled)) cout << "<td>" << to_string(c.calculateScore(East, down)) << "</td>";
							redoubled = !redoubled;
						} while(redoubled);
						doubled = !doubled;
					} while(doubled);
					vulnerable = Vulnerability(1-vulnerable);
				} while(vulnerable);
			cout << "</tr>";
		}
	cout << "</table></body></html>";
}
