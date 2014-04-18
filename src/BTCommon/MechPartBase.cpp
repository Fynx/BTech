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

#include "BTCommon/MechPartBase.h"
#include "BTCommon/CommonStrings.h"

MechPartBase::MechPartBase(BTech::MechPartType type, BTech::MechPartSide side)
	: MechPartBase(type, side, 1, 1, QList <const WeaponBase *> ())
{}

MechPartBase::MechPartBase(BTech::MechPartType type,
                           BTech::MechPartSide side,
                           int maxArmorValue,
                           int maxInternalValue,
                           const QList <const WeaponBase *> &weapons)
{
	load(type, side, maxArmorValue, maxInternalValue, weapons);
}

MechPartBase::MechPartBase(const MechPartBase &mechPart)
{
	*this = mechPart;
}

MechPartBase & MechPartBase::operator = (const MechPartBase &mechPart)
{
	load(mechPart.type, mechPart.side, mechPart.maxArmorValue, mechPart.maxInternalValue, mechPart.weapons);
	return *this;
}

MechPartBase::operator QString() const
{
	if (getSide() == BTech::MechPartSide::General)
		return BTech::mechPartTypeStringChange[getType()];
	return BTech::mechPartSideStringChange[getSide()] + " " + BTech::mechPartTypeStringChange[getType()];
}

bool MechPartBase::operator == (const MechPartBase &mechPart) const
{
	return getType() == mechPart.getType() && getSide() == mechPart.getSide();
}

void MechPartBase::setType(BTech::MechPartType type)
{
	qWarning() << "Warning: Setting type of mech part. Use constructor with adequate parameters instead.";
	this->type = type;
}

BTech::MechPartType MechPartBase::getType() const
{
	return type;
}

void MechPartBase::setSide(BTech::MechPartSide side)
{
	qWarning() << "Warning: Setting side of mech part. Use constructor with adequate parameters instead.";
	this->side = side;
}

BTech::MechPartSide MechPartBase::getSide() const
{
	return side;
}

void MechPartBase::setMaxArmorValue(int value)
{
	this->maxArmorValue = value;
}

int MechPartBase::getMaxArmorValue() const
{
	return maxArmorValue;
}

void MechPartBase::setMaxInternalValue(int value)
{
	this->maxInternalValue = value;
}

int MechPartBase::getMaxInternalValue() const
{
	return maxInternalValue;
}

void MechPartBase::addWeapon(const WeaponBase *weapon)
{
	weapons.append(weapon);
}

const QList <const WeaponBase *> & MechPartBase::getWeapons() const
{
	return weapons;
}

void MechPartBase::removeWeapon(int index)
{
	weapons.removeAt(index);
}

QDataStream & operator << (QDataStream &out, const MechPartBase &mechPart)
{
	out << mechPart.type << mechPart.side << mechPart.maxArmorValue << mechPart.maxInternalValue;
	out << static_cast<UID>(mechPart.weapons.count());
	for (const WeaponBase *weapon : mechPart.weapons)
		out << weapon->getUid();
	return out;
}

QDataStream & operator >> (QDataStream &in, MechPartBase &mechPart)
{
	mechPart.clearData();
	in >> mechPart.type >> mechPart.side >> mechPart.maxArmorValue >> mechPart.maxInternalValue;

	UID weaponCount, weaponUid;
	in >> weaponCount;
	for (UID i = 0; i < weaponCount; ++i) {
		in >> weaponUid;
		mechPart.weapons.append(WeaponModel::getWeapon(weaponUid));
	}
	return in;
}

void MechPartBase::load(const MechPartBase &mechPartBase)
{
	return load(mechPartBase.type,
	            mechPartBase.side,
	            mechPartBase.maxArmorValue,
	            mechPartBase.maxInternalValue,
	            mechPartBase.weapons);
}

void MechPartBase::load(BTech::MechPartType type,
                        BTech::MechPartSide side,
                        int maxArmorValue,
                        int maxInternalValue,
                        const QList <const WeaponBase *> &weapons)
{
	clearData();

	this->type = type;
	this->side = side;
	this->maxArmorValue = maxArmorValue;
	this->maxInternalValue = maxInternalValue;

	for (const WeaponBase *weapon : weapons)
		this->weapons.append(weapon);
}

void MechPartBase::clearData()
{
	qDeleteAll(weapons);
	weapons.clear();
}
