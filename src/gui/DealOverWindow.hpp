#ifndef DEALOVERWINDOW_HPP
#define DEALOVERWINDOW_HPP

#include <QDialog>
class QPushButton;
class QCloseEvent;
class PlayWindow;

class DealOverWindow : public QDialog 
{
	Q_OBJECT
	public:
		explicit DealOverWindow(QWidget *parent = Q_NULLPTR);
		void closeEvent(QCloseEvent *event);
	signals:
	public slots:
		void score();
		void playAgain();
		void reviewBidding();
		void replay();
		void save();
		void print();
		void nextDeal();
		void backToMenu();
	protected:
	private:
		PlayWindow *playWindow;
		bool showWelcomeWindowWhenDone;
		QPushButton *scoreButton;
		QPushButton *playAgainButton;
		QPushButton *reviewBiddingButton;
		QPushButton *replayButton;
		QPushButton *saveButton;
		QPushButton *printButton;
		QPushButton *nextDealButton;
		QPushButton *backToMenuButton;
};

#endif