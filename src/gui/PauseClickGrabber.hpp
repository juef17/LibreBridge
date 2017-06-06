#ifndef PAUSECLICKGRABBER_HPP
#define PAUSECLICKGRABBER_HPP

#include <QObject>
#include <QEvent>
#include <QtWidgets>
class PlayWindow;

class PauseClickGrabber : public QObject
{
	Q_OBJECT

	public:
		explicit PauseClickGrabber(QWidget *parent = Q_NULLPTR);

	protected:
		bool eventFilter(QObject *obj, QEvent *event);
	
	private:
		PlayWindow *parent;
};

#endif