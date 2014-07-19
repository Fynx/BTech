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

#ifndef MECH_PART_BASE_H
#define MECH_PART_BASE_H

#include <QtWidgets>
#include "BTCommon/Utils.h"

class MechModel;
class EditMechPartsWidget;
class MechPartBase;
class WeaponBase;

class MechPartBase
{
	friend class MechModel;
	friend class EditMechPartsWidget;
public:

	MechPartBase(BTech::MechPartType type = BTech::MechPartType::Head, BTech::MechPartSide side = BTech::MechPartSide::General);
	MechPartBase(BTech::MechPartType type,
		     BTech::MechPartSide side,
		     int maxArmorValue,
		     int maxInternalValue,
		     const QList <const WeaponBase *> &weapons);
	MechPartBase(const MechPartBase &mechPart);

	MechPartBase & operator = (const MechPartBase &mechPart);

	explicit operator QString() const;

	bool operator == (const MechPartBase &mechPart) const;

	void setType(BTech::MechPartType type);
	BTech::MechPartType getType() const;
	void setSide(BTech::MechPartSide side);
	BTech::MechPartSide getSide() const;
	void setMaxArmorValue(int maxArmorValue);
	int getMaxArmorValue() const;
	void setMaxInternalValue(int value);
	int getMaxInternalValue() const;

	void addWeapon(const WeaponBase *weapon);
	const QList <const WeaponBase *> & getWeapons() const;
	void removeWeapon(int index);

	friend QDataStream & operator << (QDataStream &out, const MechPartBase &mechPart);
	friend QDataStream & operator >> (QDataStream &in, MechPartBase &mechPart);

	static const int MINIMAL_ARMOR_VALUE    = 1;
	static const int MINIMAL_INTERNAL_VALUE = 1;

private:
	void load(const MechPartBase &mechPartBase);
	void load(BTech::MechPartType type,
	          BTech::MechPartSide side,
	          int maxArmorValue,
	          int maxInternalValue,
	          const QList <const WeaponBase *> &weapons);

	BTech::MechPartType type;
	BTech::MechPartSide side;
	int maxArmorValue;
	int maxInternalValue;
	QList <const WeaponBase *> weapons;

	void clearData();
};

#endif
