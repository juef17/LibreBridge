#ifndef WELCOMEWINDOW_HPP
#define WELCOMEWINDOW_HPP

#include <QWidget>
class QPushButton;
class QPixmap;
class QResizeEvent;

class WelcomeWindow : public QWidget
{
	Q_OBJECT
	public:
		explicit WelcomeWindow(QWidget *parent = 0);
		QPushButton *m_button;

	signals:
	public slots:
	protected:
		void resizeEvent(QResizeEvent *event) override;
	private:
		QPixmap background;
};

#endif