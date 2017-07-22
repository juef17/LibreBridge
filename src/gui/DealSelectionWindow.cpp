#include "Common.hpp"
#include "DealSelectionWindow.hpp"
#include "PlayWindow.hpp"
#include "WelcomeWindow.hpp"
#include "SeedValidator.hpp"
#include "../Game.hpp"
#include "../Misc.hpp"
#include <QPushButton>
#include <QResizeEvent>
#include <QLineEdit>
#include <QCloseEvent>

DealSelectionWindow::DealSelectionWindow(QWidget *parent): QDialog (parent)
{
	this->parent = (PlayWindow*)parent;
	showWelcomeWindowWhenDone = true;
	
	// This window
	setFixedSize(300, 100);
	setTitle(this, "Deal selection");
	centerWindow(this);
	
	// Deal number
	seedTextBox = new QLineEdit(this);
	seedTextBox->setFixedSize(getSeedTextMaxWidth(), 30);
	int x = (width()-seedTextBox->width()) / 2;
	seedTextBox->move(x, 10);
	seedTextBox->setMaxLength(getSeedTextMaxLength()+10);
	seedTextBox->setText(QString("%1").arg(getSeed()));
	seedTextBox->setAlignment(Qt::AlignCenter);
	seedValidator = new SeedValidator(this);
	seedTextBox->setValidator(seedValidator);
	connect(seedTextBox, SIGNAL (textChanged(QString)), this, SLOT (updateOnSeedChange(QString)));

	// Minus Button
	seedMinusButton = new QPushButton("-", this);
	seedMinusButton->setFixedSize(15, 15);
	seedMinusButton->move(seedTextBox->x() - 2*seedMinusButton->width(), seedTextBox->y() + (seedTextBox->height()-seedMinusButton->height())/2);
	seedMinusButton->setAutoDefault(false);
	connect(seedMinusButton, SIGNAL (clicked()), this, SLOT (minusDeal()));

	// Plus Button
	seedPlusButton = new QPushButton("+", this);
	seedPlusButton->setFixedSize(15, 15);
	seedPlusButton->move(seedTextBox->x() + seedTextBox->width() + seedPlusButton->width(), seedTextBox->y() + (seedTextBox->height()-seedPlusButton->height())/2);
	seedPlusButton->setAutoDefault(false);
	connect(seedPlusButton, SIGNAL (clicked()), this, SLOT (plusDeal()));

	// playButton
	playButton = new QPushButton("Play this deal", this);
	playButton->setFixedSize(150, 30);
	x = (width()-playButton->width()) / 2;
	playButton->move(x, seedTextBox->y() + seedTextBox->height() + 10);
	connect(playButton, SIGNAL (clicked()), this, SLOT (startBidding()));
	playButton->setDefault(true);
}

void DealSelectionWindow::minusDeal()
{
	options.seedIncrementStep = -1; // So that if there are constraints, we don't just add back 1 to the seed
	incrementSeed();
	seedTextBox->setText(QString("%1").arg(getSeed())); // Should emit textChanged signal and run updateOnSeedChange()
	options.seedIncrementStep = 1;
}

void DealSelectionWindow::plusDeal()
{
	seedTextBox->setText(QString("%1").arg(getSeed()+1)); // Should emit textChanged signal and run updateOnSeedChange()
}

void DealSelectionWindow::updateOnSeedChange(const QString &text)
{
	QLocale defaultLocale;
	bool ok;
	RANDOMNESS_SIZE tmp = (RANDOMNESS_SIZE)(defaultLocale.RANDOMNESS_QT_SIZE(text, &ok));
	if(ok) // Should always be true, since the QLineEdit has a validator
	{
		options.seed = tmp;
		parent->destroyAllHandWidgets();
		parent->getGame()->findNextDeal();
		seedTextBox->setText(QString("%1").arg(getSeed())); // If constraints weren't met, the deal number may have changed
		parent->createAllHandWidgets();
		parent->updateDealInfoLabel();
	}
}

void DealSelectionWindow::closeEvent(QCloseEvent *)
{
	delete seedValidator;
	if(showWelcomeWindowWhenDone)
	{
		WelcomeWindow *welcomeWindow = new WelcomeWindow;
		copyWindowGeometry(parent, welcomeWindow);
		parent->close();
		welcomeWindow->show();
	}
}

void DealSelectionWindow::startBidding()
{
	showWelcomeWindowWhenDone = false;
	this->close();
	parent->startBidding();
}