#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include <QResizeEvent>
#include <QGridLayout>
#include <vector>
#include "CardWidget.hpp"
#include "Common.hpp"
#include "PlayWindow.hpp"
#include "DealSelectionWindow.hpp"
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
	setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
	setObjectName("playWindow"); // Otherwise all its children inherit the background color
	setStyleSheet("#playWindow {background-color: green;}");
	setCentralWidget(centralWidget);
	
	// Menu
	menuBar = new QMenuBar(centralWidget);
	QMenu *fileMenu = new QMenu("File");
	fileMenu->addMenu(new QMenu("menu1_SubMenu"));
	menuBar->addMenu(fileMenu);
	
	createAllHandWidgets();
	centralWidget->setLayout(&gridLayout);
	show();
	
	dealSelectionWindow = new DealSelectionWindow(this);
	dealSelectionWindow->show();
	
	//this->setDisabled(true);
	//dealSelectionWindow->setDisabled(false);
}

PlayWindow::~PlayWindow()
{
	destroyAllHandWidgets();
	for(int i=0; i<4; i++) for(int j=0; j<4; j++)
	{
		delete playersHLayout[i][j];
	}
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
	Player **players = game->getPlayers();
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			playersHLayout[i][j] = new QHBoxLayout;
			if(i%2) EWVLayout[i/2].addLayout(playersHLayout[i][j]);
			else NSHLayout[i/2].addLayout(playersHLayout[i][j]);
		}
		for (auto &card : players[i]->getHand())
		{
			CardWidget* cardWidget = new CardWidget(&card);
			Qt::Alignment align = (i == 1 ? Qt::AlignRight : Qt::AlignLeft);
			playersHLayout[i][card.getSuit()-1]->addWidget(cardWidget, 0, align);
			handsWidgets[i].push_back(cardWidget);
		}
	}
	if(!gridLayout.itemAtPosition(1, 2)) // If the players layouts haven't been added to the gridLayout yet
	{
		gridLayout.addLayout(&(EWVLayout[0]), 1, 2);
		gridLayout.addLayout(&(EWVLayout[1]), 1, 0);
		gridLayout.addLayout(&(NSHLayout[0]), 0, 1);
		gridLayout.addLayout(&(NSHLayout[1]), 2, 1);
	}
}

void PlayWindow::destroyAllHandWidgets()
{
	for(int i=0; i<4; i++)
	{
		for (auto &cardWidget : handsWidgets[i])
		{
			delete cardWidget;
		}
		handsWidgets[i].clear();
	}
}

Game* PlayWindow::getGame() const
{
	return game;
}