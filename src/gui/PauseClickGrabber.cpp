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
	/*	Grabs Keypresses, clicks and keyboard input... I think
		2:	MouseButtonPress
		3:	MouseButtonRelease
		4:	MouseButtonDblClick
		6:	KeyPress
		7:	KeyRelease
		51:	ShortcutOverride
	*/
	if(e == 3 || e == 7 || e == 51) parent->resumeFromPause();
	return QObject::eventFilter(obj, event);
}