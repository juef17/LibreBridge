#ifndef PLAYWINDOW_HPP
#define PLAYWINDOW_HPP

#include <vector>
#include <QMainWindow>
class QMenuBar;
class QGridLayout;
class QHBoxLayout;
class QPushButton;
class CardWidget;
class DealSelectionWindow;
class Game;
class CardWidget;

class PlayWindow : public QMainWindow
{
	Q_OBJECT
	public:
		explicit PlayWindow(QWidget *parent = Q_NULLPTR);
		void closeEvent(QCloseEvent *event);
		void createAllHandWidgets();
	signals:
	public slots:
	protected:
		void resizeEvent(QResizeEvent *event) override;
	private:
		DealSelectionWindow *dealSelectionWindow;
		QMenuBar *menuBar;
		QMenu *fileMenu;
		QPushButton *testButton;
		QWidget *centralWidget;
		Game *game;
		QGridLayout *gridLayout;
		QHBoxLayout *layout;
		std::vector<CardWidget*> handsWidgets[4];
};

#endif