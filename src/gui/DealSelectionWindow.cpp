#include "Common.hpp"
#include "DealSelectionWindow.hpp"
#include "PlayWindow.hpp"
#include "WelcomeWindow.hpp"
#include "SeedValidator.hpp"
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
	seedTextBox->setMaxLength(getSeedTextMaxLength());
	//seedTextBox->setReadOnly(true);
	seedTextBox->setText(QString("%1").arg(getSeed()));
	seedTextBox->setAlignment(Qt::AlignCenter);
	seedValidator = new SeedValidator;
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
	y = (height()-playButton->height()) / 2;
	playButton->move(x, seedTextBox->y() + seedTextBox->height() + 10);
	connect(playButton, SIGNAL (clicked()), this, SLOT (playThisDeal()));
	playButton->setDefault(true);
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
	RANDOMNESS_SIZE tmp = (RANDOMNESS_SIZE)(defaultLocale.RANDOMNESS_QT_SIZE(text, &ok));
	if(ok) // Should always be true, since the QLineEdit has a validator
	{
		options.seed = tmp;
		parent->destroyAllHandWidgets();
		parent->getGame()->findNextDeal();
		seedTextBox->setText(QString("%1").arg(getSeed())); // If constraints weren't met, the deal number may have changed
		parent->createAllHandWidgets();
	}
}

void DealSelectionWindow::closeEvent(QCloseEvent *)
{
	delete seedValidator;
	WelcomeWindow *welcomeWindow = new WelcomeWindow;
	copyWindowGeometry(parent, welcomeWindow);
	parent->close();
	welcomeWindow->show();
}