#ifndef CARDWIDGET_HPP
#define CARDWIDGET_HPP

#include <QPushButton>
#include <QSize>
#include "../Card.hpp"
class PlayWindow;

class CardWidget : public QPushButton
{
	Q_OBJECT
	public:
		explicit CardWidget(Card card, QWidget *parent = Q_NULLPTR);
		Card getCard();
	signals:
	public slots:
		void clickCardWidget();
	protected:
	private:
		Card card;
		PlayWindow *parent;
};

#endif