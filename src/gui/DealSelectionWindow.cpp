#include "Common.hpp"
#include "DealSelectionWindow.hpp"
#include "PlayWindow.hpp"
#include "WelcomeWindow.hpp"
#include "../Game.hpp"
#include <QPushButton>
#include <QApplication>
#include <QResizeEvent>
#include <QLineEdit>
#include <QStyle>
#include <QDesktopWidget>
#include <QCloseEvent>

DealSelectionWindow::DealSelectionWindow(QWidget *parent): QDialog (parent)
{
	Game game;
	int x, y;
	this->parent = (PlayWindow*)parent;
	
	// This window
	setFixedSize(300, 100);
	setTitle(this, "Deal selection");
	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	x = (screenGeometry.width()-width()) / 2;
	y = (screenGeometry.height()-height()) / 2;
	move(x, y);
	
	// Deal number
	seedTextBox = new QLineEdit(this);
	seedTextBox->setFixedSize(getSeedTextMaxWidth(), 30);
	x = (width()-seedTextBox->width()) / 2;
	seedTextBox->move(x, 10);
    seedTextBox->setFocus();
	seedTextBox->setReadOnly(true);
	seedTextBox->setText(QString("%1").arg(game.getSeed()));
	seedTextBox->setAlignment(Qt::AlignCenter);

	// playButton
	playButton = new QPushButton("Play this deal", this);
	playButton->setFixedSize(150, 30);
	x = (width()-playButton->width()) / 2;
	y = (height()-playButton->height()) / 2;
	playButton->move(x, seedTextBox->y() + seedTextBox->height() + 10);
	connect(playButton, SIGNAL (clicked()), this, SLOT (playThisDeal()));
	playButton->setFocus();
}

void DealSelectionWindow::playThisDeal()
{
}

void DealSelectionWindow::closeEvent(QCloseEvent *)
{
	WelcomeWindow *welcomeWindow = new WelcomeWindow;
	copyWindowGeometry(parent, welcomeWindow);
	welcomeWindow->show();
	parent->close();
}