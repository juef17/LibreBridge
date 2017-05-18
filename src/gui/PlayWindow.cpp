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
#include "CardLayout.hpp"
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
	
	QPushButton* test = new QPushButton("test");
	test->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	gridLayout.addWidget(test, 1, 1);
	// test hand 7924083785712169274
		
	gridLayout.setColumnStretch(1, 1);
	gridLayout.setRowStretch(1, 1);
	centralWidget->setLayout(&gridLayout);
	show();
	
	dealSelectionWindow = new DealSelectionWindow(this);
	dealSelectionWindow->show();
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
	cardLayouts[p] = new CardLayout(player);
	for (auto &card : player->getHand())
	{
		CardWidget* cardWidget = new CardWidget(&card);
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
	delete cardLayouts[p];
}

Game* PlayWindow::getGame() const
{
	return game;
}