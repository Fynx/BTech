/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
Copyright (C) 2014 by Krzysztof Adamek <squadack [at] students [dot] mimuw [dot] edu [dot] pl>
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

#ifndef MECH_H
#define MECH_H

#include <QtWidgets>
#include "BTCommon/Uid.h"
#include "BTCommon/Utils.h"

class MechBase;
class MechPart;
class MechPartBase;
class Weapon;

/**
 * \class Mech
 */
class Mech
{
public:
	Mech();
	Mech(UID uid);
	Mech(const MechBase *base);
	Mech(const Mech &) = delete;
	virtual ~Mech();

	explicit operator QString() const;

	void setBase(const MechBase *base);

	QString getName() const;

	int getTonnage() const;

	int getMaxMovePoints() const;
	int getMaxRunPoints() const;
	int getMaxJumpPoints() const;

	void setArmorValue(int value);
	int getArmorValue() const;

	void setBaseFireRange(int range);
	int getBaseFireRange() const;

	void setHeatSinksNumber(int number);
	int getHeatSinksNumber() const;

	BTech::Range distanceToRange(int distance) const;
	int getRangeDamage(BTech::Range range) const;
	int getDamageValue(int distance) const;

	void removeMechPart(const MechPartBase *mechPart);
	QList <const MechPart *> getMechParts() const;
	QList <const MechPart *> getMechParts(BTech::MechPartType type) const;
	QList <const Weapon *> getWeapons() const;

	friend QDataStream & operator << (QDataStream &out, const Mech &mech);
	friend QDataStream & operator >> (QDataStream &in, Mech &mech);

protected:
	QList <MechPart *> findMechParts(BTech::MechPartType type);
	MechPart * findMechPart(BTech::MechPartType type, BTech::MechPartSide side);

	const MechBase *base;

	int armorValue;
	int baseFireRange;
	int heatSinksNumber;

	QList <MechPart *> parts;

	void clearData();
};

#endif // MECH_H
