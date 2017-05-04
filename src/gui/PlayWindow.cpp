#include "PlayWindow.hpp"
#include "../Game.hpp"
#include <QPushButton>
#include <QApplication>
#include <QResizeEvent>
#include <QLineEdit>

PlayWindow::PlayWindow(QWidget *parent): QWidget(parent)
{
	this->setObjectName("playWindow"); // Otherwise all its children inherit the background color
	this->setStyleSheet("#playWindow {background-color: green;}");
	seedTextBox = new QLineEdit(this);
    seedTextBox->setPlaceholderText("Placeholder Text");
	seedTextBox->setFixedSize(300, 30);
	seedTextBox->setGeometry(10, 10, 80, 30);
    seedTextBox->setFocus();
	
	//game = new Game();

	// playButton
	playButton = new QPushButton("Play this deal", this);
	playButton->setGeometry(170, 10, 80, 30);
	playButton->setFixedSize(150, 50);
	connect(playButton, SIGNAL (clicked()), this, SLOT (playThisDeal()));
	
	//this->setDisabled(true);
}

void PlayWindow::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);

}

void PlayWindow::playThisDeal()
{
}