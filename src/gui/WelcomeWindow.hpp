#ifndef WELCOMEWINDOW_HPP
#define WELCOMEWINDOW_HPP

#include <QWidget>
class QPushButton;
class QPixmap;
class QResizeEvent;
class DealSelectionWindow;

class WelcomeWindow : public QWidget
{
	Q_OBJECT
	public:
		explicit WelcomeWindow(QWidget *parent = 0);
		QPushButton *quitButton;
		QPushButton *playButton;

	signals:
	public slots:
		void startLocalGame();
	protected:
		void resizeEvent(QResizeEvent *event) override;
	private:
		QPixmap background;
		DealSelectionWindow *playWindow;
};

#endif