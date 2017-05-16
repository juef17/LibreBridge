#ifndef CARDVLAYOUT_HPP
#define CARDVLAYOUT_HPP

#include <QtWidgets>
#include <QList>

class CardVLayout : public QLayout
{
	public:
		CardVLayout(QWidget *parent = Q_NULLPTR) : QLayout(parent) {}
		~CardVLayout();

		void addItem(QLayoutItem *item);
		int count() const;
		QSize sizeHint() const;
		QSize minimumSize() const;
		QLayoutItem *itemAt(int) const;
		QLayoutItem *takeAt(int);
		void setGeometry(const QRect &rect);
		int spacing() const;

	private:
		QList<QLayoutItem*> list;
		Qt::Alignment alignment;
};

#endif
