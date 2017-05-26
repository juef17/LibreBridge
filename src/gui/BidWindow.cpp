#include "BidWindow.hpp"
#include "BidButton.hpp"
#include "Common.hpp"
#include "PlayWindow.hpp"
#include "WelcomeWindow.hpp"
#include "../Misc.hpp"
#include "../Bid.hpp"
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>

BidWindow::BidWindow(QWidget *parent): QDialog (parent)
{
	this->parent = (PlayWindow*)parent;
	showWelcomeWindowWhenDone = true;
	
	// This window
	int x, y;
	setFixedSize(370, 240);
	setTitle(this, "Bidding");
	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	x = (screenGeometry.width()-width()) / 2;
	y = (screenGeometry.height()-height()) / 2;
	move(x, y);

	// evalButton
	evalButton = new QPushButton("Evaluate", this);
	evalButton->move(10, 175);
	evalButton->setAutoDefault(false);
	evalButton->setEnabled(false);

	// interpretButton
	interpretButton = new QPushButton("Interpret", this);
	interpretButton->move(96, 175);
	interpretButton->setAutoDefault(false);
	interpretButton->setEnabled(false);

	// hintButton
	hintButton = new QPushButton("Hint", this);
	hintButton->move(53, 205);
	hintButton->setAutoDefault(false);
	hintButton->setEnabled(false);

	// passButton
	passButton = new QPushButton("Pass", this);
	passButton->move(185, 205);
	passButton->setAutoDefault(false);

	// doubleButton
	doubleButton = new QPushButton("Double", this);
	doubleButton->move(275, 205);
	doubleButton->setAutoDefault(false);
	doubleButton->setEnabled(false);
	
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
			bidButtons[index] = new BidButton(Suit(j+1), i+1);
			bidButtonsLayout->addWidget(bidButtons[index], i, j+1);
		}
	}
}

void BidWindow::closeEvent(QCloseEvent *)
{
	if(showWelcomeWindowWhenDone)
	{
		WelcomeWindow *welcomeWindow = new WelcomeWindow;
		copyWindowGeometry(parent, welcomeWindow);
		parent->close();
		welcomeWindow->show();
	}
}