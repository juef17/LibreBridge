#include <iostream>
#include "Common.hpp"
#include "DealSelectionWindow.hpp"
#include "PlayWindow.hpp"
#include "WelcomeWindow.hpp"
#include "../Game.hpp"
#include "../Misc.hpp"
#include <QPushButton>
#include <QApplication>
#include <QResizeEvent>
#include <QLineEdit>
#include <QCloseEvent>
#include <QDesktopWidget>

DealSelectionWindow::DealSelectionWindow(QWidget *parent): QDialog (parent)
{
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
	//seedTextBox->setReadOnly(true);
	seedTextBox->setText(QString("%1").arg(getSeed()));
	seedTextBox->setAlignment(Qt::AlignCenter);
	connect(seedTextBox, SIGNAL (textChanged(QString)), this, SLOT (updateOnSeedChange(QString)));

	// Minus Button
	seedMinusButton = new QPushButton("-", this);
	seedMinusButton->setFixedSize(15, 15);
	seedMinusButton->move(seedTextBox->x() - 2*seedMinusButton->width(), seedTextBox->y() + (seedTextBox->height()-seedMinusButton->height())/2);
	connect(seedMinusButton, SIGNAL (clicked()), this, SLOT (minusDeal()));

	// Plus Button
	seedPlusButton = new QPushButton("+", this);
	seedPlusButton->setFixedSize(15, 15);
	seedPlusButton->move(seedTextBox->x() + seedTextBox->width() + seedPlusButton->width(), seedTextBox->y() + (seedTextBox->height()-seedPlusButton->height())/2);
	connect(seedPlusButton, SIGNAL (clicked()), this, SLOT (plusDeal()));

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

void DealSelectionWindow::minusDeal()
{
	seedTextBox->setText(QString("%1").arg(getSeed()-1)); // Should emit textChanged signal and run updateOnSeedChange()
}

void DealSelectionWindow::plusDeal()
{
	seedTextBox->setText(QString("%1").arg(getSeed()+1)); // Should emit textChanged signal and run updateOnSeedChange()
}

void DealSelectionWindow::updateOnSeedChange(const QString &text)
{
	QLocale defaultLocale;
	bool ok;
	RANDOMNESS_SIZE tmp = (RANDOMNESS_SIZE)(defaultLocale.toULongLong(text, &ok));
	// TODO:	http://doc.qt.io/qt-5/qlineedit.html#setValidator              http://doc.qt.io/qt-4.8/qintvalidator.html
	//			faire un #define pour les choses du genre toULongLong
	if(ok)
	{
		options.seed = tmp;
		
		std::cout << std::endl << "yay" << std::endl;
		parent->destroyAllHandWidgets();
		parent->getGame()->findNextDeal();
		seedTextBox->setText(QString("%1").arg(getSeed())); // If constraints weren't met, the deal number may have changed
		parent->createAllHandWidgets();
		//centralWidget->setLayout(gridLayout);
	}
}

void DealSelectionWindow::closeEvent(QCloseEvent *)
{
	WelcomeWindow *welcomeWindow = new WelcomeWindow;
	copyWindowGeometry(parent, welcomeWindow);
	welcomeWindow->show();
	parent->close();
}