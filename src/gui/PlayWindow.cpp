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
#include "CardHLayout.hpp"
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
	
	// Menu
	/*menuBar = new QMenuBar(centralWidget);
	QMenu *fileMenu = new QMenu("File");
	fileMenu->addMenu(new QMenu("menu1_SubMenu"));
	menuBar->addMenu(fileMenu);*/
	
	createAllHandWidgets();
	
	/*QPushButton* test = new QPushButton("test");
	test->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	gridLayout.addWidget(test, 1, 1);*/
	// test hand 7924083785712169274
		
	gridLayout.setColumnStretch(1, 1);
	gridLayout.setRowStretch(1, 1);
	gridLayout.addLayout(&(EWVLayout[0]), 1, 2);
	gridLayout.addLayout(&(EWVLayout[1]), 1, 0);
	gridLayout.addLayout(&(NSHLayout[0]), 0, 1);
	gridLayout.addLayout(&(NSHLayout[1]), 2, 1);
	centralWidget->setLayout(&gridLayout);
	show();
	
	dealSelectionWindow = new DealSelectionWindow(this);
	dealSelectionWindow->show();
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
	for(int i=0; i<4; i++) createHandWidgets((Position)i);
}

void PlayWindow::createHandWidgets(Position p)
{
	Player **players = game->getPlayers();
	for(int i=0; i<4; i++)
	{
		Qt::Alignment align = (p == 1 ? Qt::AlignRight : Qt::AlignLeft);
		playersHLayout[p][i] = new CardHLayout(Q_NULLPTR, align);
		if(p%2) EWVLayout[p/2].addLayout(playersHLayout[p][i]);
		else NSHLayout[p/2].addLayout(playersHLayout[p][i]);
	}
	for (auto &card : players[p]->getHand())
	{
		CardWidget* cardWidget = new CardWidget(&card);
		playersHLayout[p][card.getSuit()-1]->addWidget(cardWidget);
		handsWidgets[p].push_back(cardWidget);
	}
	for(int i=0; i<4; i++) if(!playersHLayout[p][i]->count()) delete playersHLayout[p][i];
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
	QLayout *playerLayout;
	if(p%2) playerLayout = &(EWVLayout[p/2]);
	else playerLayout = &(NSHLayout[p/2]);
	QLayoutItem *item;
	while((item = playerLayout->takeAt(0))) delete item;
}

Game* PlayWindow::getGame() const
{
	return game;
}