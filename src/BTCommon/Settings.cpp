#include "BTCommon/Settings.h"

bool Settings::contains(const QString &key)
{
	return getInstance().contains(key);
}

void Settings::setValue(const QString &key, const QVariant &value)
{
	getInstance().setValue(key, value);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue)
{
	return getInstance().value(key, defaultValue);
}

void Settings::sync()
{
	getInstance().sync();
}

QSettings & Settings::getInstance()
{
	static QSettings instance;
	return instance;
}

Settings::~Settings()
{
	getInstance().sync();
}
