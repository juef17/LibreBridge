#ifndef CONTRACTWINDOW_HPP
#define CONTRACTWINDOW_HPP

#include <QDialog>
#include "../Contract.hpp"
class QPushButton;
class QCloseEvent;
class QLabel;
class BidWindow;

class ContractWindow : public QDialog 
{
	Q_OBJECT
	public:
		explicit ContractWindow(Contract c, QWidget *parent = Q_NULLPTR);
		void closeEvent(QCloseEvent *event);
	signals:
	public slots:
		void playAsDeclarer();
		void restartBidding();
		void changeBid();
		void OK();
	protected:
	private:
		BidWindow *parent;
		QPushButton *playAsDeclarerButton;
		QPushButton *restartBiddingButton;
		QPushButton *changeBidButton;
		QPushButton *OKButton;
		QLabel *contractLabel;
		Contract contract;
};

#endif