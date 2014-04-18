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

#ifndef PLAYER_H
#define PLAYER_H

#include <QtWidgets>
#include "BTCommon/CommonStrings.h"
#include "BTCommon/MechEntity.h"
#include "BTCommon/Utils.h"

/**
 * \class Player
 * Contains a description of a given player, including name, statistics, optionally AI description (if computer player).
 */
class Player : public QObject
{
Q_OBJECT;

public:
	Player(const QString name = BTech::Strings::PlayerComputer, const QString description = QString());
	~Player();

	QList <MechEntity *> getMechs() const;
	void setName(const QString &name);
	QString getName() const;
	void setDescription(const QString &description);
	QString getDescription() const;
	void setColor(const QColor &color);
	QColor getColor() const;

	void addMech(MechEntity *mech);
	void removeMech(const MechEntity *mech);
	bool hasMech(const MechEntity *mech) const;

	friend QDataStream & operator << (QDataStream &out, const Player &player);
	friend QDataStream & operator >> (QDataStream &in, Player &player);

private:
	QList <MechEntity *> mechs;

	QString name;
	QString description;
	QColor color;
};

#endif // PLAYER_H
