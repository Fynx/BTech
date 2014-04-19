/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
This file is part of BTech Project.

	BTech Project is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	BTech Project is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with BTech.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SETTINGS_H
#define SETTINGS_H

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
};

#endif
