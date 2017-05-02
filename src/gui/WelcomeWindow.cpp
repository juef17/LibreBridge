#include "WelcomeWindow.hpp"
#include <QPushButton>
#include <QApplication>
#include <QResizeEvent>

WelcomeWindow::WelcomeWindow(QWidget *parent): QWidget(parent)
{
	setMinimumSize(480, 320);
	QPixmap tmpBackground("./images/LibreBridge.png");
	background = tmpBackground.scaled(500, 150, Qt::IgnoreAspectRatio);
	QPalette palette;
	palette.setBrush(QPalette::Background, background);
	this->setPalette(palette);

	m_button = new QPushButton("Hello World", this);
	m_button->setGeometry(10, 10, 80, 30);
	m_button->setFixedSize(150, 50);
	m_button->setToolTip("Panus");
	QFont font("Courier");
	m_button->setFont(font);
	QIcon icon("./images/LibreBridge.ico");
	m_button->setIcon(icon);
	connect(m_button, SIGNAL (clicked()), QApplication::instance(), SLOT (quit()));

	connect(this, SIGNAL (resizeEvent(QResizeEvent)), QApplication::instance(), SLOT (quit()));
}

void WelcomeWindow::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	
	// Resize background image
	QSize size = event->size();
	QPixmap tmpBackground("./images/LibreBridge.png");
	background = tmpBackground.scaled(size.width(), size.height(), Qt::IgnoreAspectRatio);
	QPalette palette;
	palette.setBrush(QPalette::Background, background);
	this->setPalette(palette);
	
	
}