#ifndef SEEDVALIDATOR_HPP
#define SEEDVALIDATOR_HPP

#include <QValidator>

class SeedValidator : public QValidator
{
	Q_OBJECT
	public:
		explicit SeedValidator(QObject *parent = Q_NULLPTR);
		QValidator::State validate(QString &input, int&) const;
};

#endif