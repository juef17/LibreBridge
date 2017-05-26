#ifndef BIDWINDOW_HPP
#define BIDWINDOW_HPP

#include <QDialog>
class QLabel;
class QPushButton;
class QGridLayout;
class QHBoxLayout;
class WelcomeWindow;
class PlayWindow;
class BidButton;

class BidWindow : public QDialog 
{
	Q_OBJECT
	public:
		explicit BidWindow(QWidget *parent = Q_NULLPTR);
		void closeEvent(QCloseEvent *event);
	signals:
	public slots:
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
		QWidget *bidHistoryPositionLabelsWidget;
		QLabel *bidHistoryWidget;
		QWidget *bidButtonsWidget;
		QGridLayout *bidButtonsLayout;
		BidButton *bidButtons[35];
		QLabel *bidLevels[7];
};

#endif