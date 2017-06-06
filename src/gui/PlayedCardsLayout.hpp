#ifndef PLAYEDCARDSLAYOUT_HPP
#define PLAYEDCARDSLAYOUT_HPP

#include <QtWidgets>
#include <QList>
#include "CardWidget.hpp"
class Player;
class Game;

class PlayedCardsLayout : public QLayout
{
	public:
		PlayedCardsLayout(std::vector<CardWidget*> *playedCardsWidgets, Game *g, QWidget *parent = Q_NULLPTR);
		~PlayedCardsLayout();

		void addItem(QLayoutItem *item);
		int count() const;
		QSize sizeHint() const;
		QSize minimumSize() const;
		QLayoutItem *itemAt(int) const;
		QLayoutItem *takeAt(int);
		void setGeometry(const QRect &rect);
		int spacing() const;
		void clearPlayedCardsWidgets();

	private:
		QList<QLayoutItem*> list;
		std::vector<CardWidget*> *cardWidgets;
		Game *game;
};

#endif
