#ifndef BIDBUTTON_HPP
#define BIDBUTTON_HPP

#include <QPushButton>
#include "../LibreBridge.hpp"
class BidWindow;

class BidButton : public QPushButton 
{
	Q_OBJECT
	public:
		explicit BidButton(Suit suit, int level, QWidget *parent = Q_NULLPTR);
	signals:
	public slots:
		void clickBidButton();
	protected:
	private:
		Suit suit;
		int level;
		BidWindow *parent;
};

#endif