#ifndef BIDWINDOW_HPP
#define BIDWINDOW_HPP

#include <QDialog>
class PlayWindow;
class WelcomeWindow;

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
};

#endif