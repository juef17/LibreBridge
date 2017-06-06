#include "PauseClickGrabber.hpp"
#include "PlayWindow.hpp"
#include <iostream>

PauseClickGrabber::PauseClickGrabber(QWidget *parent): QObject(parent)
{
	this->parent = (PlayWindow*)parent;
}

bool PauseClickGrabber::eventFilter(QObject *obj, QEvent *event)
{
	int e = event->type();
	if((e >= 2 && e <= 4) || e == 6 || e == 7 || e == 51)
	{
		parent->resumeFromPause();
		std::cout << std::flush << "PauseClickGrabber::eventFilter" << std::endl << std::flush;
	}
	// Grabs Keypresses, clicks and keyboard input... I think
	return QObject::eventFilter(obj, event);
}