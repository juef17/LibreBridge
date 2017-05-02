#ifndef WELCOMEWINDOW_HPP
#define WELCOMEWINDOW_HPP

#include <QWidget>
#include <QPushButton>

class WelcomeWindow : public QWidget
{
	Q_OBJECT
	public:
		explicit WelcomeWindow(QWidget *parent = 0);
		QPushButton *m_button;

	signals:
	public slots:
};

#endif