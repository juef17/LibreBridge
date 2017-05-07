#ifndef DEALSELECTIONWINDOW_HPP
#define DEALSELECTIONWINDOW_HPP

#include <QDialog>
class QPushButton;
class QResizeEvent;
class QCloseEvent;
class QLineEdit;
class Game;
class PlayWindow;

class DealSelectionWindow : public QDialog 
{
	Q_OBJECT
	public:
		explicit DealSelectionWindow(QWidget *parent = Q_NULLPTR);
		QLineEdit *seedTextBox;
		QPushButton *playButton;
		void closeEvent(QCloseEvent *event);
	signals:
	public slots:
		void playThisDeal();
	protected:
		Game *game;
	private:
		PlayWindow *parent;
};

#endif