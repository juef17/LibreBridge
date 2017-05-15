#include "CardHLayout.hpp"

CardHLayout::CardHLayout(QWidget *parent, Qt::Alignment alignment): QLayout(parent)
{
	this->alignment = alignment;
}

int CardHLayout::count() const
{
	return list.size();
}

QLayoutItem *CardHLayout::itemAt(int idx) const
{
	return list.value(idx);
}

QLayoutItem *CardHLayout::takeAt(int idx)
{
	return idx >= 0 && idx < list.size() ? list.takeAt(idx) : 0;
}

void CardHLayout::addItem(QLayoutItem *item)
{
	list.append(item);
}

CardHLayout::~CardHLayout()
{
	QLayoutItem *item;
	while((item = takeAt(0))) delete item;
}

void CardHLayout::setGeometry(const QRect &r)
{
	QLayout::setGeometry(r);
	if(!list.size()) return;

	int w = r.width();
	int i = 0;
	int listSize = list.size();
	while(i < listSize)
	{
		QLayoutItem *o = list.at(i);
		int x = (alignment == Qt::AlignRight ? -(listSize-i) : i) * spacing();
		QRect geom(r.x() + x, r.y(), w, r.height());
		o->setGeometry(geom);
		++i;
	}
}

QSize CardHLayout::sizeHint() const
{
	QSize s(0,0);
	int n = list.count();
	if (n > 0) s = QSize(10, 70);
	int i = 0;
	while(i < n)
	{
		QLayoutItem *o = list.at(i);
		s = s.expandedTo(o->sizeHint());
		++i;
	}
	return s + n*QSize(spacing(), spacing());
}

QSize CardHLayout::minimumSize() const
{
	QSize s(0,0);
	int n = list.count();
	int i = 0;
	while(i < n)
	{
		QLayoutItem *o = list.at(i);
		s = s.expandedTo(o->minimumSize());
		++i;
	}
	return s + n*QSize(spacing(), spacing());
}

int CardHLayout::spacing() const
{
	return 16;
}