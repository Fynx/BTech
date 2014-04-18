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

#ifndef RULES_H
#define RULES_H

#include <QtWidgets>
#include "BTCommon/Utils.h"

/**
 * \class Rules
 * Contains description of rules of the game. This includes name of the version, phases that are allowed during the game, allowed actions and much more.
 */
class Rules : public QObject
{
Q_OBJECT;

public:
	static void initRules();

	static void setVersion(const BTech::GameVersion version);
	static BTech::GameVersion getVersion();
	static void setDescription(const QString &description);
	static QString getDescription();
	static QList <BTech::GamePhase> getAllowedPhases();

private:
	Rules() = delete;
	Rules(const Rules &) = delete;
	Rules(Rules &&) = delete;
	void operator = (const Rules &) = delete;
	void operator = (Rules &&) = delete;

	static BTech::GameVersion version;
	static QString description;
	static QList <BTech::GamePhase> allowedPhases;
};

#endif // RULES_H
