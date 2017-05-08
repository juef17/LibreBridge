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
	
	// This window
	centralWidget = new QWidget(this);
	setTitle(this);
	setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
	setObjectName("playWindow"); // Otherwise all its children inherit the background color
	setStyleSheet("#playWindow {background-color: green;}");
	//this->setCentralWidget(centralWidget);
	
	// Menu
	menuBar = new QMenuBar(centralWidget);
	QMenu *fileMenu = new QMenu("File");
	fileMenu->addMenu(new QMenu("menu1_SubMenu"));
	menuBar->addMenu(fileMenu);
	/*
	gridLayout = new QGridLayout;
	QHBoxLayout *layout = new QHBoxLayout;
	gridLayout->addLayout();
    layout->addWidget(button1);
	centralWidget->setLayout(layout);
	centralWidget->show();
	*/
	
	createAllHandWidgets();
	show();
	
	dealSelectionWindow = new DealSelectionWindow(this);
	dealSelectionWindow->show();
	
	this->setDisabled(true);
	dealSelectionWindow->setDisabled(false);
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
		for (auto &card : players[i]->getHand())
		{
			CardWidget* cardWidget = new CardWidget(&card, this);
			handsWidgets[i].push_back(cardWidget);
		}
	}
}