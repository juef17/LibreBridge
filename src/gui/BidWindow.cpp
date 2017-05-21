#include "BidWindow.hpp"
#include "Common.hpp"
#include "PlayWindow.hpp"
#include "WelcomeWindow.hpp"
#include "../Misc.hpp"
#include <QPushButton>
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>

BidWindow::BidWindow(QWidget *parent): QDialog (parent)
{
	this->parent = (PlayWindow*)parent;
	showWelcomeWindowWhenDone = true;
	
	// This window
	int x, y;
	setFixedSize(370, 200);
	setTitle(this, "Bidding");
	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	x = (screenGeometry.width()-width()) / 2;
	y = (screenGeometry.height()-height()) / 2;
	move(x, y);

	// evalButton
	evalButton = new QPushButton("Evaluate", this);
	evalButton->move(10, 135);
	evalButton->setAutoDefault(false);
	evalButton->setEnabled(false);

	// interpretButton
	interpretButton = new QPushButton("Interpret", this);
	interpretButton->move(96, 135);
	interpretButton->setAutoDefault(false);
	interpretButton->setEnabled(false);

	// hintButton
	hintButton = new QPushButton("Hint", this);
	hintButton->move(53, 165);
	hintButton->setAutoDefault(false);
	hintButton->setEnabled(false);

	// passButton
	passButton = new QPushButton("Pass", this);
	passButton->move(185, 165);
	passButton->setAutoDefault(false);

	// doubleButton
	doubleButton = new QPushButton("Double", this);
	doubleButton->move(275, 165);
	doubleButton->setAutoDefault(false);
	doubleButton->setEnabled(false);
	
	// QString("♥")
	// http://stackoverflow.com/questions/30973781/qt-add-custom-font-from-resource
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