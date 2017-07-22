#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QResizeEvent>
#include <QGridLayout>
#include <QLabel>
#include <vector>
#include <iostream>
#include "CardWidget.hpp"
#include "Common.hpp"
#include "PlayWindow.hpp"
#include "DealSelectionWindow.hpp"
#include "BidWindow.hpp"
#include "DealOverWindow.hpp"
#include "CardLayout.hpp"
#include "PlayedCardsLayout.hpp"
#include "PauseClickGrabber.hpp"
#include "../Card.hpp"
#include "../Game.hpp"
#include "../Misc.hpp"

using namespace std;

PlayWindow::PlayWindow(QWidget *parent): QMainWindow(parent)
{
	randomizeSeed();
	game = new Game();
	game->findNextDeal();
	
	// This window
	centralWidget = new QWidget(this);
	setTitle(this);
	setObjectName("playWindow"); // Otherwise all its children inherit the background color
	setStyleSheet("#playWindow {background-color: green;}");
	setCentralWidget(centralWidget);
	cardsAreClickable = false;
	firstSuit = NoTrump;
	tricksMade[0] = 0;
	tricksMade[1] = 0;
	waitForAI = true;
	waitForAutoplaySingles = true;
	isPaused = false;
	
	// Menu
	/*menuBar = new QMenuBar(centralWidget);
	QMenu *fileMenu = new QMenu("File");
	fileMenu->addMenu(new QMenu("menu1_SubMenu"));
	menuBar->addMenu(fileMenu);*/
	
	createAllHandWidgets();
	
	dealInfoLabel = new QLabel("", this);
	gridLayout.addWidget(dealInfoLabel, 0, 2);
	gridLayout.setAlignment(dealInfoLabel, Qt::AlignRight);
	updateDealInfoLabel();
	
	contractInfoLabel = new QLabel("", this);
	gridLayout.addWidget(contractInfoLabel, 2, 2);
	gridLayout.setAlignment(dealInfoLabel, Qt::AlignRight);
	
	tricksInfoLabel = new QLabel("", this);
	gridLayout.addWidget(tricksInfoLabel, 0, 0);
	
	playedCardsLayout = new PlayedCardsLayout(&playedCardsWidgets, game, Q_NULLPTR);
	gridLayout.addLayout(playedCardsLayout, 1, 1);
	
	QPixmap arrowImage("./images/arrow.png");
	for(int i=0; i<4; i++)
	{
		QTransform transformArrow;
		transformArrow.scale(0.5, 0.5);
		arrows[i] = new QLabel;
		QFlags<Qt::AlignmentFlag> flag;
		switch(Position(i))
		{
			case North:
				flag = Qt::AlignHCenter | Qt::AlignTop;
				break;
			case South:
				transformArrow.rotate(180);
				flag = Qt::AlignHCenter | Qt::AlignBottom;
				break;
			case East:
				transformArrow.rotate(90);
				flag = Qt::AlignVCenter | Qt::AlignRight;
				break;
			case West:
				transformArrow.rotate(270);
				flag = Qt::AlignVCenter | Qt::AlignLeft;
				break;
		}
		gridLayout.addWidget(arrows[i], 1, 1, flag);
		arrows[i]->setPixmap(arrowImage.transformed(transformArrow));
		arrows[i]->setVisible(false);
	}
	
	/*QPushButton* test = new QPushButton("test");
	test->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	gridLayout.addWidget(test, 1, 1);*/
		
	gridLayout.setColumnStretch(1, 1);
	gridLayout.setRowStretch(1, 1);
	centralWidget->setLayout(&gridLayout);
	centerWindow(this);
	show();
	
	dealSelectionWindow = new DealSelectionWindow(this);
	dealSelectionWindow->show();
	PauseClickGrabber *g = new PauseClickGrabber(this);
	installEventFilter(g);
	
	dealOverWindow = new DealOverWindow(this);
}

PlayWindow::~PlayWindow()
{
	destroyAllHandWidgets();
}

void PlayWindow::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
}

void PlayWindow::closeEvent(QCloseEvent *event)
{
	QWidget::closeEvent(event);
}

void PlayWindow::createAllHandWidgets()
{
	for(int i=0; i<4; i++) createHandWidgets((Position)i);
}

void PlayWindow::createHandWidgets(Position p)
{
	Player *player = game->getPlayers()[p];
	cardLayouts[p] = new CardLayout(player, &handsWidgets[p], this);
	for (auto &card : player->getHand())
	{
		CardWidget* cardWidget = new CardWidget(card, this);
		handsWidgets[p].push_back(cardWidget);
		cardLayouts[p]->addWidget(cardWidget);
	}
	if(p == North)	gridLayout.addLayout(cardLayouts[p], 0, 1);
	if(p == East)	gridLayout.addLayout(cardLayouts[p], 1, 2);
	if(p == South)	gridLayout.addLayout(cardLayouts[p], 2, 1);
	if(p == West)	gridLayout.addLayout(cardLayouts[p], 1, 0);
}

void PlayWindow::destroyAllHandWidgets()
{
	for(int i=0; i<4; i++) destroyHandWidgets((Position)i);
}

void PlayWindow::destroyHandWidgets(Position p)
{
	for (auto &cardWidget : handsWidgets[p])
	{
		delete cardWidget;
	}
	handsWidgets[p].clear();
	if(cardLayouts[p])
	{
		delete cardLayouts[p];
		cardLayouts[p] = NULL;
	}
}

void PlayWindow::startBidding()
{
	bidWindow = new BidWindow(this);
	bidWindow->show();
	//delete dealSelectionWindow;
}

Game* PlayWindow::getGame() const
{
	return game;
}

bool PlayWindow::getCardsAreClickable() const
{
	return cardsAreClickable;
}

void PlayWindow::setCardsAreClickable(bool c)
{
	cardsAreClickable = c;
}

void PlayWindow::updateDealInfoLabel()
{
	QString dealNumber = QString("Deal: ") + QString::number(getSeed());
	QString dealer = QString("Dealer: ") + QString::fromStdString(positionToString(game->getDealer()));
	QString vulnerability = QString("Vulnerable: ") + QString::fromStdString(vulnerabilityToString(game->getVulnerability()));
	dealInfoLabel->setText(dealNumber + "\n" + dealer + "\n" + vulnerability);
}

void PlayWindow::updateContractInfoLabel()
{
	contractInfoLabel->setText(QString::fromStdString(game->getContract().toString()));
}

void PlayWindow::playCard(CardWidget *c)
{
	Card card = c->getCard();
	Position p = game->getPositionFromCard(card);
	Player *player = game->getPlayers()[p];
	if(game->getPlayedCardsHistory().size() % 4 == 0) firstSuit = NoTrump; // So player can choose card of any suit
	if(game->whoseTurnIsItToPlay() != p) return;
	if(!player->hasCard(card)) return;
	if(!player->isValidPlay(card, firstSuit)) return;
	
	if(game->getPlayedCardsHistory().size() % 4 == 0) firstSuit = card.getSuit();
	cardLayouts[p]->removeCardWidget(c);
	playedCardsWidgets.push_back(c);
	c->setCardImageFaceUp();
	playedCardsLayout->addWidget(c);
	player->clearCard(card);
	game->addCardToPlayHistory(card);
	updateCurrentPlayerArrow();
	if(game->getPlayedCardsHistory().size() % 4 == 0)
	{
		tricksMade[game->whoseTurnIsItToPlay() % 2]++; // The one whose turn it is is necessarily the one who one the trick
		updateTricksInfoLabel();
		isPaused = true;
		if(!options.waitAfterTrick) QTimer::singleShot(options.minimalWait, this, SLOT(resumeFromPause()));
	}
	else playingProcess();
}

void PlayWindow::updateCurrentPlayerArrow()
{
	Position p = game->whoseTurnIsItToPlay();
	int numberOfPlayedCards = game->getPlayedCardsHistory().size();
	for(int i=0; i<4; i++) arrows[i]->setVisible(Position(i) == p && numberOfPlayedCards != 52);
}

void PlayWindow::playingProcess()
{
	updateTricksInfoLabel();
	int numberOfPlayedCards = game->getPlayedCardsHistory().size();
	Position player = game->whoseTurnIsItToPlay();
	Position dummyPosition = nextTeammate(game->getContract().getDeclarer());
	bool isDummy = (player == dummyPosition);
	Suit firstSuit = (numberOfPlayedCards%4 == 0 ? NoTrump : game->getPlayedCardsHistory().at((numberOfPlayedCards-1) - (numberOfPlayedCards-1)%4).getSuit());
	Position actualPlayer = (isDummy ? game->getContract().getDeclarer() : player);
	
	if(numberOfPlayedCards == 52)
	{
		Player **players = game->getPlayers();
		destroyAllHandWidgets();
		for(int i=0; i<4; i++)
		{
			for(auto &card : recreateHand(Position(i), game->getPlayedCardsHistory(), game->getContract())) players[i]->addCardToHand(card);
			players[i]->sortHand(game->getContract().getSuit());
		}
		createAllHandWidgets();
		dealOverWindow->show();
		return;
	}
	
	if(game->getPlayers()[actualPlayer]->getIsHuman())
	{
		if(options.autoplaySingles && game->getPlayers()[player]->getPlayableCards(firstSuit).size() == 1)
		{
			// Autoplay single
			if(options.minimalWait && waitForAutoplaySingles && numberOfPlayedCards)
			{
				waitForAutoplaySingles = false;
				QTimer::singleShot(options.minimalWait, this, SLOT(playingProcess()));
				return;
			}
			waitForAutoplaySingles = true;
			Card card = game->getPlayers()[player]->getPlayableCards(firstSuit).front();
			playCard(getCardWidgetFromCard(card)); // Validity has already been checked since it's the only playable card
			cardsAreClickable = false;
			return;
		}
		else
		{
			cardsAreClickable = true;
			return;
		}
	}
	else cardsAreClickable = false;
	
	if(options.AI_playDelay && waitForAI && numberOfPlayedCards)
	{
		waitForAI = false;
		QTimer::singleShot(options.AI_playDelay, this, SLOT(playingProcess()));
		return;
	}
	waitForAI = true;
	
	Card playedCard;
	do
	{
		playedCard = (isDummy ? game->getPlayers()[actualPlayer]->playCard(firstSuit, game->getPlayers()[dummyPosition]->getHand()) : game->getPlayers()[actualPlayer]->playCard(firstSuit));
	} while(!game->getPlayers()[player]->hasCard(playedCard) || !game->getPlayers()[player]->isValidPlay(playedCard, firstSuit));
	playCard(getCardWidgetFromCard(playedCard));
}

CardWidget* PlayWindow::getCardWidgetFromCard(Card c) const
{
	Position p = game->getPositionFromCard(c);
	for (auto &o : handsWidgets[p]) if(o->getCard() == c) return o;
	if(DEBUG_COUT) cout << flush << "PlayWindow::getCardWidgetFromCard ERROR" << endl << flush;
	return Q_NULLPTR;
}

void PlayWindow::resumeFromPause()
{
	if(isPaused)
	{
		playedCardsLayout->clearPlayedCardsWidgets();
		isPaused = false;
		updateCurrentPlayerArrow();
		playingProcess();
	}
}

bool PlayWindow::getIsPaused() const
{
	return isPaused;
}

void PlayWindow::updateTricksInfoLabel() const
{
	tricksInfoLabel->setText(QString::fromStdString("NS tricks: " + to_string(tricksMade[0]) + "\nEW tricks: " + to_string(tricksMade[1])));
}

void PlayWindow::startNewGame(RANDOMNESS_SIZE currentDeal)
{
	destroyAllHandWidgets();
	setSeed(currentDeal);
	game->prepareForNextGame();
	game->findNextDeal();
	
	cardsAreClickable = false;
	firstSuit = NoTrump;
	tricksMade[0] = 0;
	tricksMade[1] = 0;
	waitForAI = true;
	waitForAutoplaySingles = true;
	isPaused = false;
	
	createAllHandWidgets();
	updateDealInfoLabel();
	contractInfoLabel->setText("");
	updateTricksInfoLabel();
	for(int i=0; i<4; i++) arrows[i]->setVisible(false);
	
	bidWindow->reset();
	bidWindow->show();
	
}