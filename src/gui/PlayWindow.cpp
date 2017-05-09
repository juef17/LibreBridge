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

using namespace std;

PlayWindow::PlayWindow(QWidget *parent): QMainWindow(parent)
{
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
	
	gridLayout = new QGridLayout;
	
	createAllHandWidgets();
	centralWidget->setLayout(gridLayout);
	show();
	
	dealSelectionWindow = new DealSelectionWindow(this);
	dealSelectionWindow->show();
	
	//this->setDisabled(true);
	//dealSelectionWindow->setDisabled(false);
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
		int j=0;
		for (auto &card : players[i]->getHand())
		{
			CardWidget* cardWidget = new CardWidget(&card);
			gridLayout->addWidget(cardWidget, i, j++);
			handsWidgets[i].push_back(cardWidget);
		}
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