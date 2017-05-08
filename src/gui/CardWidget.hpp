#ifndef CARDWIDGET_HPP
#define CARDWIDGET_HPP

#include <QPushButton>
class Card;

class CardWidget : public QPushButton
{
	Q_OBJECT
	public:
		explicit CardWidget(Card *card, QWidget *parent = Q_NULLPTR);
	signals:
	public slots:
		void clickCardWidget();
	protected:
	private:
};

#endif