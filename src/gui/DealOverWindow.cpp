#include "Common.hpp"
#include "DealOverWindow.hpp"
#include "PlayWindow.hpp"
#include "WelcomeWindow.hpp"
#include "../Game.hpp"
#include "../Misc.hpp"
#include <QPushButton>
#include <QCloseEvent>
#include <iostream>

DealOverWindow::DealOverWindow(QWidget *parent): QDialog (parent)
{
	this->playWindow = (PlayWindow*)parent;
	showWelcomeWindowWhenDone = true;
	
	// This window
	setFixedSize(150, 237);
	setTitle(this, "Deal over");
	centerWindow(this);
	setModal(true);
	
	// score button
	scoreButton = new QPushButton("Score this deal", this);
	scoreButton->setFixedSize(130, 24);
	scoreButton->move(10, 5);
	connect(scoreButton, SIGNAL (clicked()), this, SLOT (score()));
	
	// playAgain button
	playAgainButton = new QPushButton("Play deal again", this);
	playAgainButton->setFixedSize(130, 24);
	playAgainButton->move(10, 34);
	playAgainButton->setAutoDefault(false);
	connect(playAgainButton, SIGNAL (clicked()), this, SLOT (playAgain()));
	
	// reviewBidding button
	reviewBiddingButton = new QPushButton("Review bidding", this);
	reviewBiddingButton->setFixedSize(130, 24);
	reviewBiddingButton->move(10, 63);
	reviewBiddingButton->setEnabled(false);
	reviewBiddingButton->setAutoDefault(false);
	connect(reviewBiddingButton, SIGNAL (clicked()), this, SLOT (reviewBidding()));
	
	// replay button
	replayButton = new QPushButton("View hand replay", this);
	replayButton->setFixedSize(130, 24);
	replayButton->move(10, 92);
	replayButton->setEnabled(false);
	replayButton->setAutoDefault(false);
	connect(replayButton, SIGNAL (clicked()), this, SLOT (replay()));
	
	// save button
	saveButton = new QPushButton("Save deal number", this);
	saveButton->setFixedSize(130, 24);
	saveButton->move(10, 121);
	saveButton->setEnabled(false);
	saveButton->setAutoDefault(false);
	connect(saveButton, SIGNAL (clicked()), this, SLOT (save()));
	
	// print button
	printButton = new QPushButton("Print deal", this);
	printButton->setFixedSize(130, 24);
	printButton->move(10, 150);
	printButton->setEnabled(false);
	printButton->setAutoDefault(false);
	connect(printButton, SIGNAL (clicked()), this, SLOT (print()));
	
	// nextDeal button
	nextDealButton = new QPushButton("Play next deal", this);
	nextDealButton->setFixedSize(130, 24);
	nextDealButton->move(10, 179);
	nextDealButton->setEnabled(false);
	nextDealButton->setAutoDefault(false);
	connect(nextDealButton, SIGNAL (clicked()), this, SLOT (nextDeal()));
	
	// backToMenu button
	backToMenuButton = new QPushButton("Go to main menu", this);
	backToMenuButton->setFixedSize(130, 24);
	backToMenuButton->move(10, 208);
	backToMenuButton->setAutoDefault(false);
	connect(backToMenuButton, SIGNAL (clicked()), this, SLOT (backToMenu()));

}

void DealOverWindow::score()
{

}

void DealOverWindow::playAgain()
{
	showWelcomeWindowWhenDone = false;
	close();
	playWindow->startNewGame(getSeed()-1);
}

void DealOverWindow::reviewBidding()
{
	
}

void DealOverWindow::replay()
{
	
}

void DealOverWindow::save()
{
	
}

void DealOverWindow::print()
{
	
}

void DealOverWindow::nextDeal()
{
	
}

void DealOverWindow::backToMenu()
{
	close();
}

void DealOverWindow::closeEvent(QCloseEvent *)
{
	if(showWelcomeWindowWhenDone)
	{
		WelcomeWindow *welcomeWindow = new WelcomeWindow;
		copyWindowGeometry(playWindow, welcomeWindow);
		playWindow->close();
		welcomeWindow->show();
	}
}