#ifndef CARDLAYOUT_HPP
#define CARDLAYOUT_HPP

#include <QtWidgets>
#include <QList>
#include "CardWidget.hpp"
class Player;
class PlayWindow;

class CardLayout : public QLayout
{
	public:
		CardLayout(Player *p, std::vector<CardWidget*> *h, PlayWindow *playWindow, QWidget *parent = Q_NULLPTR);
		~CardLayout();

		void addItem(QLayoutItem *item);
		int count() const;
		QSize sizeHint() const;
		QSize minimumSize() const;
		QLayoutItem *itemAt(int) const;
		QLayoutItem *takeAt(int);
		void setGeometry(const QRect &rect);
		int spacing() const;
		void removeCardWidget(CardWidget *c);

	private:
		QList<QLayoutItem*> list;
		Player *player;
		std::vector<CardWidget*> *handWidgets;
		PlayWindow *playWindow;
};

#endif
