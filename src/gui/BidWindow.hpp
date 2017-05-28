#ifndef BIDWINDOW_HPP
#define BIDWINDOW_HPP

#include <QDialog>
#include "../LibreBridge.hpp"
class QLabel;
class QPushButton;
class QGridLayout;
class QHBoxLayout;
class WelcomeWindow;
class PlayWindow;
class BidButton;
class Game;

class BidWindow : public QDialog 
{
	Q_OBJECT
	public:
		explicit BidWindow(QWidget *parent = Q_NULLPTR);
		void closeEvent(QCloseEvent *event);
		void biddingProcess();
		void disableAllButtons();
		void enableButtons();
		void bidClick(BetType betType, uint8_t level, Suit suit);
	signals:
	public slots:
		void bidPass();
		void bidDouble();
	protected:
	private:
		PlayWindow *parent;
		bool showWelcomeWindowWhenDone;
		QPushButton *evalButton;
		QPushButton *hintButton;
		QPushButton *interpretButton;
		QPushButton *passButton;
		QPushButton *doubleButton;
		QGridLayout *bidHistoryDisplayGrid;
		QHBoxLayout *bidHistoryPositionLabelsBox;
		QLabel *bidHistoryPositionLabels[4];
		QLabel *bidHistoryLabels[28];
		int currentBidHistoryLabel;
		QWidget *bidHistoryPositionLabelsWidget;
		QLabel *bidHistoryWidget;
		QWidget *bidButtonsWidget;
		QGridLayout *bidButtonsLayout;
		BidButton *bidButtons[35];
		QLabel *bidLevels[7];
		bool isDoubleLegal;
		bool isRedoubleLegal;
		uint8_t lastLevel;
		Suit lastSuit;
		Position playerWhoBetNormallyLast;
		Position playerPos;
		bool lastDoubled;
		bool lastRedoubled;
		Game* game;
};

#endif