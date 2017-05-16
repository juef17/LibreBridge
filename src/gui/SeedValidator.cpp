#include <iostream>
#include "../LibreBridge.hpp"
#include "SeedValidator.hpp"
#include "DealSelectionWindow.hpp"

SeedValidator::SeedValidator(DealSelectionWindow* d, QObject *parent) : QValidator(parent)
{
	dealSelectionWindow = d;
}

QValidator::State SeedValidator::validate(QString &input, int&) const
{
	if(input.right(1) == QString("+")) dealSelectionWindow->plusDeal();
	else if(input.right(1) == QString("-")) dealSelectionWindow->minusDeal();
	QLocale defaultLocale;
	bool ok;
	defaultLocale.RANDOMNESS_QT_SIZE(input, &ok);
	if(ok) return Acceptable;
	return Invalid;
}