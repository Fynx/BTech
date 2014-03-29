#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QtCore>

class Settings
{

public:
	static bool contains(const QString &key);
	static void setValue(const QString &key, const QVariant &value);
	static QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
	static void sync();

private:
	Settings() = default;
	Settings(const Settings &) = delete;
	~Settings();
	void operator = (const Settings &) = delete;

	static QSettings & getInstance();

	static const QString ApplicationName, OrganizationName;
};

#endif
