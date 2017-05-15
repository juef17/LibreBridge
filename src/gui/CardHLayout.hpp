#ifndef CARDHLAYOUT_HPP
#define CARDHLAYOUT_HPP

#include <QtWidgets>
#include <QList>

class CardHLayout : public QLayout
{
	public:
		CardHLayout(QWidget *parent, int dist): QLayout(parent) {}
		~CardHLayout();

		void addItem(QLayoutItem *item);
		QSize sizeHint() const;
		QSize minimumSize() const;
		QLayoutItem *itemAt(int) const;
		QLayoutItem *takeAt(int);
		void setGeometry(const QRect &rect);

	private:
		QList<QLayoutItem*> list;
};

#endif
