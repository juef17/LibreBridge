#include <iostream>
#include <chrono>
#include <thread>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QCloseEvent>
#include <QTimer>
#include "BidWindow.hpp"
#include "BidButton.hpp"
#include "Common.hpp"
#include "PlayWindow.hpp"
#include "WelcomeWindow.hpp"
#include "ContractWindow.hpp"
#include "../Misc.hpp"
#include "../Bid.hpp"
#include "../Contract.hpp"
#include "../Game.hpp"

BidWindow::BidWindow(QWidget *parent): QDialog (parent)
{
	this->parent = (PlayWindow*)parent;
	showWelcomeWindowWhenDone = true;
	isDoubleLegal = false;
	isRedoubleLegal = false;
	game = this->parent->getGame();
	currentBidHistoryLabel = game->getDealer();
	playerWhoBetNormallyLast = game->getDealer();
	playerPos = game->getDealer();
	lastDoubled = false;
	lastRedoubled = false;
	lastLevel = 0;
	lastSuit = NoTrump;
	waitForAI = true;
	
	// This window
	setFixedSize(370, 240);
	setTitle(this, "Bidding");
	centerWindow(this);

	// evalButton
	evalButton = new QPushButton("Evaluate", this);
	evalButton->move(10, 175);
	evalButton->setAutoDefault(false);

	// interpretButton
	interpretButton = new QPushButton("Interpret", this);
	interpretButton->move(96, 175);
	interpretButton->setAutoDefault(false);

	// hintButton
	hintButton = new QPushButton("Hint", this);
	hintButton->move(53, 205);
	hintButton->setAutoDefault(false);

	// passButton
	passButton = new QPushButton("Pass", this);
	passButton->move(185, 205);
	passButton->setAutoDefault(false);
	connect(passButton, SIGNAL (clicked()), this, SLOT (bidPass()));

	// doubleButton
	doubleButton = new QPushButton("Double", this);
	doubleButton->move(275, 205);
	doubleButton->setAutoDefault(false);
	connect(doubleButton, SIGNAL (clicked()), this, SLOT (bidDouble()));
	
	// Bid History
	bidHistoryWidget = new QLabel("", this);
	bidHistoryWidget->setFrameStyle(QFrame::Box | QFrame::Sunken);
	bidHistoryWidget->setGeometry(10, 20, 160, 150);
	bidHistoryPositionLabelsBox = new QHBoxLayout;
	bidHistoryPositionLabelsBox->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	bidHistoryPositionLabelsWidget = new QWidget(this);
	bidHistoryPositionLabelsWidget->setLayout(bidHistoryPositionLabelsBox);
	bidHistoryPositionLabelsWidget->move(10, -5);
	for(int i=0; i<4; i++)
	{
		bidHistoryPositionLabels[i] = new QLabel(QString::fromStdString(positionToString(Position(i))));
		bidHistoryPositionLabelsBox->addWidget(bidHistoryPositionLabels[i]);
	}
	bidHistoryDisplayGrid = new QGridLayout();
	bidHistoryDisplayGrid->setAlignment(Qt::AlignCenter);
	bidHistoryWidget->setLayout(bidHistoryDisplayGrid);
	for(int i=0; i<28; i++)
	{
		bidHistoryLabels[i] = new QLabel("");
		bidHistoryLabels[i]->setAlignment(Qt::AlignCenter);
		//setBidHistoryText(bidHistoryLabels[i], b);
		//bidHistoryLabels[i]->setStyleSheet("QLabel { background-color : red; color : blue; }");
		bidHistoryLabels[i]->setFixedWidth((bidHistoryWidget->width()-20) / 4);
		//bidHistoryLabels[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
		bidHistoryDisplayGrid->addWidget(bidHistoryLabels[i], i/4, i%4);
	}
	
	// Bid buttons
	bidButtonsWidget = new QWidget(this);
	bidButtonsWidget->setGeometry(185, 10, 160, 180);
	bidButtonsLayout = new QGridLayout;
	bidButtonsLayout->setAlignment(Qt::AlignCenter);
	bidButtonsWidget->setLayout(bidButtonsLayout);
	for(int i = 0; i<7; i++)
	{
		bidLevels[i] = new QLabel(QString::number(i+1) + QString("."));
		bidLevels[i]->setAlignment(Qt::AlignBottom);
		bidButtonsLayout->addWidget(bidLevels[i], i, 0);
		for(int j = 0; j<5; j++)
		{
			int index = i*5 + j;
			bidButtons[index] = new BidButton(Suit(j+1), i+1, this);
			bidButtonsLayout->addWidget(bidButtons[index], i, j+1);
		}
	}
	
	evalButton->setEnabled(false);
	hintButton->setEnabled(false);
	interpretButton->setEnabled(false);
	
	biddingProcess();
}

void BidWindow::closeEvent(QCloseEvent *)
{
	if(showWelcomeWindowWhenDone && !parent->getCardsAreClickable()) // If they're clickable, a game was just started
	{
		WelcomeWindow *welcomeWindow = new WelcomeWindow;
		copyWindowGeometry(parent, welcomeWindow);
		parent->close();
		welcomeWindow->show();
	}
}

void BidWindow::biddingProcess()
{
	using namespace std;
	vector<Bid> bidWar = game->getBidWar();
	
	disableAllButtons();
	
	uint8_t firstBidsTable[2][6];	// 1st index is team %2, 2nd is suit
	for(uint8_t i = 0; i<2; ++i) for(uint8_t j = 1; j<6; ++j) firstBidsTable[i][j] = 10; // Mark as unset (10)

	bool atLeastOneBidMade = false;
	uint8_t numberOfPass = 0;
	uint8_t lastBidMade = 0; // team % 2 who bet last
	lastLevel = 0;
	lastSuit = NoTrump;
	isDoubleLegal = false; isRedoubleLegal = false;
	Contract contract;
	playerPos = game->getDealer();
	
	for (auto &bid : bidWar)
    {
		if(bid.getBetType() == Pass) numberOfPass++;
		else numberOfPass = 0;
		if(bid.getBetType() == Normal)
		{
			playerWhoBetNormallyLast = playerPos;
			lastBidMade = playerPos%2;
			lastSuit = bid.getSuit();
			lastLevel = bid.getLevel();
			atLeastOneBidMade = true;
			if(firstBidsTable[playerPos%2][lastSuit] == 10) firstBidsTable[playerPos%2][lastSuit] = playerPos;
			lastDoubled = false;
			lastRedoubled = false;
		}
		if(bid.getBetType() == Double) lastDoubled = true;
		if(bid.getBetType() == Redouble) lastRedoubled = true;
		isDoubleLegal = atLeastOneBidMade && !lastDoubled && !lastRedoubled && (nextPosition(playerPos) % 2 != playerWhoBetNormallyLast % 2);
		isRedoubleLegal = atLeastOneBidMade && lastDoubled && !lastRedoubled && (nextPosition(playerPos) % 2 == playerWhoBetNormallyLast % 2);
		if(isRedoubleLegal) doubleButton->setText("Redouble");
		else doubleButton->setText("Double");
			
		playerPos = nextPosition(playerPos);
	}
	
	if(numberOfPass == 4) // Don't play that deal TODO
	{
		contract.setContract(0, NoTrump, North, false, false, game->getVulnerability());
		contractWindow = new ContractWindow(contract, this);
		contractWindow->show();
	}
	else if(atLeastOneBidMade && numberOfPass == 3) // Set the contract
	{
		contract.setContract(lastLevel, lastSuit, Position(firstBidsTable[lastBidMade][lastSuit]), lastDoubled, lastRedoubled, game->getVulnerability());
		contractWindow = new ContractWindow(contract, this);
		contractWindow->show();
	}
	else // Continue playing
	{
		Player **players = game->getPlayers();
		if(players[playerPos]->getIsHuman())
		{
			enableButtons();
		}
		else 
		{
			if(options.AI_bidDelay && waitForAI && atLeastOneBidMade)
			{
				waitForAI = false;
				QTimer::singleShot(options.AI_bidDelay, this, SLOT(biddingProcess()));
				return;
			}
			waitForAI = true;
			Bid bid;
			do
			{
				players[playerPos]->bid(bid, lastLevel, lastSuit, lastDoubled, lastRedoubled, playerWhoBetNormallyLast, bidWar);
			} while (bid.getBetType() == Invalid);
			game->addBid(bid);
			setBidHistoryText(bidHistoryLabels[currentBidHistoryLabel++], bid);
			biddingProcess();
		}
	}
}

void BidWindow::disableAllButtons()
{
	evalButton->setEnabled(false);
	hintButton->setEnabled(false);
	interpretButton->setEnabled(false);
	passButton->setEnabled(false);
	doubleButton->setEnabled(false);
	for(int i = 0; i<35; i++)
	{
		bidButtons[i]->setEnabled(false);
		bidButtons[i]->setText(" ");
	}
}

void BidWindow::enableButtons()
{
	//evalButton->setEnabled(true);
	//hintButton->setEnabled(true);
	//interpretButton->setEnabled(true);
	passButton->setEnabled(true);
	doubleButton->setEnabled(isDoubleLegal || isRedoubleLegal);
	for(int i = std::min(0, lastLevel-1); i<7; i++)
	{
		for(int j = 0; j<5; j++)
		{
			if(i < lastLevel-1 || (i == lastLevel-1 && j+1 <= lastSuit)) continue;
			int index = i*5 + j;
			bidButtons[index]->setEnabled(true);
			bidButtons[index]->setImage();
		}
	}
}

void BidWindow::bidClick(BetType betType, uint8_t level, Suit suit)
{
	Bid bid;
	bid.setBid(betType, suit, level, playerPos, lastLevel, lastSuit, lastDoubled, lastRedoubled, playerWhoBetNormallyLast);
	if(betType != Invalid)
	{
		game->addBid(bid);
		setBidHistoryText(bidHistoryLabels[currentBidHistoryLabel], bid);
		currentBidHistoryLabel = (currentBidHistoryLabel + 1) % 28;
		biddingProcess();
	}
}

void BidWindow::bidPass()
{
	bidClick(Pass, 0, NoTrump);
}

void BidWindow::bidDouble()
{
	if(isDoubleLegal) bidClick(Double, 0, NoTrump);
	else if(isRedoubleLegal) bidClick(Redouble, 0, NoTrump);
}

PlayWindow* BidWindow::getPlayWindow() const
{
	return parent;
}

void BidWindow::reset()
{
	showWelcomeWindowWhenDone = true;
	isDoubleLegal = false;
	isRedoubleLegal = false;
	currentBidHistoryLabel = game->getDealer();
	playerWhoBetNormallyLast = game->getDealer();
	playerPos = game->getDealer();
	lastDoubled = false;
	lastRedoubled = false;
	lastLevel = 0;
	lastSuit = NoTrump;
	waitForAI = true;
	enableButtons();
	for(int i=0; i<28; i++) bidHistoryLabels[i]->setText("");
	biddingProcess();
}