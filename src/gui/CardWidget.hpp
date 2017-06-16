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
		void startFlashing();
		void stopFlashing();
		void resetColor();
		void setEmphasisColor();
		void setCardImageFaceUp(bool faceUp = true);
	signals:
	public slots:
		void clickCardWidget();
		void keepFlashing();
	protected:
	private:
		Card card;
		PlayWindow *parent;
		QPixmap cardImage;
		bool isFlashing;
		bool isEmphasized;
		bool isFaceUp;
		std::string getImagePath(bool faceUp = true) const;
};

#endif