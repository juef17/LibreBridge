#include "CardLayout.hpp"

CardLayout::CardLayout(Player *p, QWidget *parent): QLayout(parent)
{
	player = p;
}

int CardLayout::count() const
{
	return list.size();
}

QLayoutItem *CardLayout::itemAt(int idx) const
{
	return list.value(idx);
}

QLayoutItem *CardLayout::takeAt(int idx)
{
	return idx >= 0 && idx < list.size() ? list.takeAt(idx) : 0;
}

void CardLayout::addItem(QLayoutItem *item)
{
	list.append(item);
}

CardLayout::~CardLayout()
{
	QLayoutItem *item;
	while((item = takeAt(0))) delete item;
}

void CardLayout::setGeometry(const QRect &r)
{
	QLayout::setGeometry(r);
	
	int listSize = list.size();
	if(!listSize) return;

	int i = 0;
	while(i < listSize)
	{
		QLayoutItem *o = list.at(i);
		QRect itemRect = o->geometry();
		int x;
		//if(alignment == Qt::AlignRight)
		{
			x = r.x() + r.width() - (listSize-i-1) * spacing() - itemRect.width();
		}
		//else
		{
			x = r.x() + i * spacing();
		}
		QRect geom(x, r.y(), itemRect.width(), itemRect.height());
		o->setGeometry(geom);
		++i;
	}
}

QSize CardLayout::sizeHint() const
{
	QSize s(0, 0);
	int n = list.count();
	int i = 0;
	while(i < n)
	{
		QLayoutItem *o = list.at(i);
		s = s.expandedTo(o->sizeHint());
		++i;
	}
	return s + (n-1)*QSize(spacing(), 0);
}

QSize CardLayout::minimumSize() const
{
	QSize s(0, 0);
	int n = list.count();
	int i = 0;
	while(i < n)
	{
		QLayoutItem *o = list.at(i);
		s = s.expandedTo(o->minimumSize());
		++i;
	}
	return s + (n-1)*QSize(spacing(), 0);
}

int CardLayout::spacing() const
{
	return 16;
}