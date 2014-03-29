#include "Colors.h"

static QString intToThreeCharsFormat(int a)
{
	return QString::number(a / 100)
	     + QString::number(a / 10 % 10)
	     + QString::number(a % 10);
}

QString BTech::colorToString(const QColor &color)
{
	return QObject::tr("#")
	     + intToThreeCharsFormat(color.red())
	     + intToThreeCharsFormat(color.green())
	     + intToThreeCharsFormat(color.blue());
}