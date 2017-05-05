#include "Common.hpp"
#include "DealSelectionWindow.hpp"
#include "../Game.hpp"
#include <QPushButton>
#include <QApplication>
#include <QResizeEvent>
#include <QLineEdit>

DealSelectionWindow::DealSelectionWindow(QWidget *parent): QWidget(parent)
{
	Game game;
	
	// This window
	setTitle(this);
	setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
	setObjectName("playWindow"); // Otherwise all its children inherit the background color
	setStyleSheet("#playWindow {background-color: green;}");
	
	// Deal number
	seedTextBox = new QLineEdit(this);
	seedTextBox->setFixedSize(getSeedTextMaxWidth(), 30);
	seedTextBox->setGeometry(10, 10, 80, 30);
    seedTextBox->setFocus();
	seedTextBox->setReadOnly(true);
	seedTextBox->setText(QString("%1").arg(game.getSeed()));
	seedTextBox->setAlignment(Qt::AlignCenter);

	// playButton
	playButton = new QPushButton("Play this deal", this);
	playButton->setGeometry(180, 10, 80, 30);
	playButton->setFixedSize(150, 50);
	connect(playButton, SIGNAL (clicked()), this, SLOT (playThisDeal()));
	
	//this->setDisabled(true);
}

void DealSelectionWindow::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);

}

void DealSelectionWindow::playThisDeal()
{
}