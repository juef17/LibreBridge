#ifndef SEEDVALIDATOR_HPP
#define SEEDVALIDATOR_HPP

#include <QValidator>
class DealSelectionWindow;

class SeedValidator : public QValidator
{
	public:
		explicit SeedValidator(DealSelectionWindow* d, QObject *parent = Q_NULLPTR);
		QValidator::State validate(QString &input, int&) const;
	private:
		DealSelectionWindow *dealSelectionWindow;
};

#endif