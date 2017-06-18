#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QIcon>
#include <QSize>
#include <QString>
#include <QTimer>
#include <iostream>
#include "CardWidget.hpp"
#include "PlayWindow.hpp"

CardWidget::CardWidget(Card c, QWidget *parent) : QPushButton("", parent)
{
	this->parent = (PlayWindow*)parent;
	card = c;
	isFlashing = false;
	isEmphasized = false;
	isFaceUp = false;
	
	setCardImageFaceUp();
	setStyleSheet("QPushButton {background-color: black;}");
	QSize size = cardImage.rect().size();
	setIconSize(size);
	resize(size);
	setMinimumSize(size);
	setFlat(true);
	
	connect(this, SIGNAL (clicked()), this, SLOT (clickCardWidget()));
}

Card CardWidget::getCard()
{
	return card;
}

void CardWidget::clickCardWidget()
{
	if(parent->getIsPaused())
	{
		parent->resumeFromPause();
		return; // We just wanted to unpause, not play the card
	}
	if(!parent->getCardsAreClickable()) return;
	stopFlashing();
	resetColor();
	parent->playCard(this);
}

void CardWidget::setEmphasisColor()
{
	QPixmap emphasisedImage = cardImage;
	QPixmap emphasisedColorFilledImage = cardImage;
	QPixmap maskedImage = cardImage;
	emphasisedColorFilledImage.fill("#ffff80");
	QPainter painter(&emphasisedImage);
	painter.drawPixmap(0, 0, emphasisedColorFilledImage);
	QBitmap mask = maskedImage.createMaskFromColor("#F0F0F0", Qt::MaskInColor);
	maskedImage.setMask(mask);
	painter.drawPixmap(0, 0, maskedImage);
	setIcon(QIcon(emphasisedImage));
	isEmphasized = true;
}

void CardWidget::startFlashing()
{
	isFlashing = true;
	keepFlashing();
}

void CardWidget::keepFlashing()
{
	if(!options.theme_flashingDelay || !isFlashing)
	{
		resetColor();
		isFlashing = false;
	}
	else
	{
		if(isEmphasized) resetColor();
		else setEmphasisColor();
		QTimer::singleShot(options.theme_flashingDelay, this, SLOT(keepFlashing()));
	}
}

void CardWidget::resetColor()
{
	setIcon(cardImage);
	isEmphasized = false;
}

void CardWidget::stopFlashing()
{
	isFlashing = false;
	resetColor();
}

void CardWidget::setCardImageFaceUp(bool faceUp)
{
	if(isFaceUp != faceUp)
	{
		cardImage = QPixmap(QString::fromStdString(getImagePath(faceUp)));
		setIcon(QIcon(cardImage));
		isFaceUp = faceUp;
	}
}

std::string CardWidget::getImagePath(bool faceUp) const
{
	if(faceUp) return "./images/cards/" + options.theme_cards + "/" + std::to_string(card.getImageNumber()) + ".png";
	else return "./images/cards/" + options.theme_cards + "/flipside.png";
}
