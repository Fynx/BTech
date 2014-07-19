/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
Copyright (C) 2014 by Bartosz Szreder <szreder [at] mimuw [dot] edu [dot] pl>
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

#ifndef MECH_BASE_H
#define MECH_BASE_H

#include <QtWidgets>
#include "BTCommon/Uid.h"
#include "BTCommon/Utils.h"

class MechPartBase;

class MechBase
{

public:
	MechBase();
	MechBase(UID uid);
	MechBase(const QString &name, UID uid);
	MechBase(const QString &name,
	         int tonnage,
	         int armorValue,
	         int maxMPS,
	         int maxJumpPoints,
	         int baseFireRange,
	         int heatSinksNumber,
	         const QList <MechPartBase *> &parts,
	         int contactRangeDamage,
	         int shortRangeDamage,
	         int mediumRangeDamage,
	         int longRangeDamage);
	MechBase(const MechBase &mech);
	~MechBase();

	void load(const MechBase &mech);
	void load(const QString &name,
	          int tonnage,
	          int armorValue,
	          int maxMPS,
	          int maxJumpPoints,
	          int baseFireRange,
	          int heatSinksNumber,
	          const QList <MechPartBase *> &parts,
	          int contactRangeDamage,
	          int shortRangeDamage,
	          int mediumRangeDamage,
	          int longRangeDamage);

	explicit operator QString() const;

	bool operator == (const MechBase &mech) const;

	void setName(const QString &name);
	QString getName() const;

	void setTonnage(int tonnage);
	int getTonnage() const;
	void setArmorValue(int value);
	int getArmorValue() const;
	void setMaxMovePoints(int maxMovePoints);
	int getMaxMovePoints() const;
	void setMaxJumpPoints(int maxJumpPoints);
	int getMaxJumpPoints() const;
	void setBaseFireRange(int range);
	int getBaseFireRange() const;
	void setHeatSinksNumber(int number);
	int getHeatSinksNumber() const;

	BTech::Range distanceToRange(int distance) const;
	void setRangeDamage(BTech::Range range, int damage);
	int getRangeDamage(BTech::Range range) const;
	int getDamageValue(int distance) const;

	MechPartBase * getMechPart(BTech::MechPartType type, BTech::MechPartSide side);
	MechPartBase * getMechPart(QPair <BTech::MechPartType, BTech::MechPartSide> typeAndSide);
	const QList <MechPartBase *> & getMechParts() const;

	friend QDataStream & operator << (QDataStream &out, const MechBase &mech);
	friend QDataStream & operator >> (QDataStream &in, MechBase &mech);

	void setUid(UID uid);
	UID getUid() const;

private:
	void clearData();

	UID uid;
	QString name;
	int tonnage;
	int armorValue;
	int maxMovePoints;
	int maxJumpPoints;
	int baseFireRange;
	int heatSinksNumber;
	QList <MechPartBase *> parts;
	QHash <BTech::Range, int> rangeDamage;

	static const int DEFAULT_TONNAGE             = 10;
	static const int DEFAULT_ARMOR_VALUE         = 1;
	static const int DEFAULT_MAX_MOVE_POINTS     = 1;
	static const int DEFAULT_MAX_JUMP_POINTS     = 1;
	static const int DEFAULT_BASE_FIRE_RANGE     = 1;
	static const int DEFAULT_HEAT_SINKS_NUMBER   = 1;
	static const int DEFAULT_RANGE_DAMAGE        = 1;
	static const int DEFAULT_OUT_OF_RANGE_DAMAGE = 0;
};


#endif
