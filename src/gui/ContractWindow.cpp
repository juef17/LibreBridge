#include "Common.hpp"
#include "BidWindow.hpp"
#include "ContractWindow.hpp"
#include "PlayWindow.hpp"
#include "../Game.hpp"
#include <QPushButton>
#include <QLabel>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QApplication>

ContractWindow::ContractWindow(Contract c, QWidget *parent): QDialog (parent)
{
	contract = c;
	this->parent = (BidWindow*)parent;
	
	// This window
	int x, y;
	setFixedSize(300, 200);
	setTitle(this, "Contract");
	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	x = (screenGeometry.width()-width()) / 2;
	y = (screenGeometry.height()-height()) / 2;
	move(x, y);
	setModal(true);
	
	// OK button
	OKButton = new QPushButton("OK", this);
	OKButton->setFixedSize(40, 30);
	OKButton->move(130, 140);
	connect(OKButton, SIGNAL (clicked()), this, SLOT (OK()));
	
	// playAsDeclarer button
	playAsDeclarerButton = new QPushButton("Play as declarer", this);
	playAsDeclarerButton->setFixedSize(100, 30);
	playAsDeclarerButton->move(15, 85);
	playAsDeclarerButton->setEnabled(false);
	playAsDeclarerButton->setAutoDefault(false);
	connect(playAsDeclarerButton, SIGNAL (clicked()), this, SLOT (playAsDeclarer()));
	
	// changeBid button
	changeBidButton = new QPushButton("Change bid", this);
	changeBidButton->setFixedSize(80, 30);
	changeBidButton->move(15, 140);
	changeBidButton->setEnabled(false);
	changeBidButton->setAutoDefault(false);
	connect(changeBidButton, SIGNAL (clicked()), this, SLOT (changeBid()));
	
	// restartBidding button
	restartBiddingButton = new QPushButton("Restart bidding", this);
	restartBiddingButton->setFixedSize(100, 30);
	restartBiddingButton->move(130, 85);
	restartBiddingButton->setEnabled(false);
	restartBiddingButton->setAutoDefault(false);
	connect(restartBiddingButton, SIGNAL (clicked()), this, SLOT (restartBidding()));

	// contract label
	contractLabel = new QLabel(QString::fromStdString(contract.toString()), this);
	contractLabel->move(15, 15);
}

void ContractWindow::playAsDeclarer()
{/*
	options.seedIncrementStep = -1; // So that if there are constraints, we don't just add back 1 to the seed
	incrementSeed();
	seedTextBox->setText(QString("%1").arg(getSeed())); // Should emit textChanged signal and run updateOnSeedChange()
	options.seedIncrementStep = 1;*/
}

void ContractWindow::changeBid()
{
	//seedTextBox->setText(QString("%1").arg(getSeed()+1)); // Should emit textChanged signal and run updateOnSeedChange()
}

void ContractWindow::restartBidding()
{/*
	showWelcomeWindowWhenDone = false;
	this->close();
	parent->startBidding();*/
}

void ContractWindow::OK()
{
	PlayWindow *playWindow = parent->getPlayWindow();
	playWindow->setCardsAreClickable(true);
	playWindow->getGame()->setContract(contract);
	playWindow->updateContractInfoLabel();
	playWindow->updateCurrentPlayerArrow();
	parent->close();
	this->close();
	//playWindow->activateWindow();
	playWindow->playingProcess();
}

void ContractWindow::closeEvent(QCloseEvent *)
{/*
	delete seedValidator;
	if(showWelcomeWindowWhenDone)
	{
		WelcomeWindow *welcomeWindow = new WelcomeWindow;
		copyWindowGeometry(parent, welcomeWindow);
		parent->close();
		welcomeWindow->show();
	}*/
}