#include "WelcomeWindow.hpp"
#include <QPushButton>
#include <QApplication>

WelcomeWindow::WelcomeWindow(QWidget *parent):

QWidget(parent)
{
	setFixedSize(300, 150);
	m_button = new QPushButton("Hello World", this);
	m_button->setGeometry(10, 10, 80, 30);
	m_button->setFixedSize(150, 50);
	m_button->setToolTip("Panus");
	QFont font("Courier");
	m_button->setFont(font);
	QIcon icon("./images/LibreBridge.ico");
	m_button->setIcon(icon);
	connect(m_button, SIGNAL (clicked()), QApplication::instance(), SLOT (quit()));
}