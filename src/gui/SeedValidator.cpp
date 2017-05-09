#include "../LibreBridge.hpp"
#include "SeedValidator.hpp"
#include <iostream>

SeedValidator::SeedValidator(QObject *parent) : QValidator(parent)
{
}

QValidator::State SeedValidator::validate(QString &input, int&) const
{
	QLocale defaultLocale;
	bool ok;
	defaultLocale.RANDOMNESS_QT_SIZE(input, &ok);
	if(ok) return Acceptable;
	return Invalid;
}