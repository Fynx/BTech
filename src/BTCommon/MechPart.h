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

#ifndef MECH_PART_H
#define MECH_PART_H

#include "BTCommon/Effect.h"
#include "BTCommon/Weapon.h"

class AttackObject;
class MechPartBase;

class MechPart : public WeaponHolder, public EffectProne
{
public:
	static void initMechPart();

	MechPart();
	MechPart(const MechPartBase *mechPartBase);
	~MechPart();

	explicit operator QString() const;

	QString getUnitName() const;
	QString getOwnerName() const;

	friend bool operator == (const MechPart &lhs, const MechPart &rhs);

	BTech::MechPartType getType() const;
	BTech::MechPartSide getSide() const;
	int getMaxArmorValue() const;
	int getMaxInternalValue() const;

	void setBase(const MechPartBase *base);

	void setArmorValue(int value);
	int getArmorValue() const;

	void setInternalValue(int value);
	int getInternalValue() const;

	void addWeapon(Weapon *weapon);
	void removeWeapon(const Weapon *weapon);
	QList <Weapon *> getWeapons();

	void applyDamage(int damage);

	void triggerTurnRecovery();
	void attack(const AttackObject &attack);

	friend QDataStream & operator << (QDataStream &out, const MechPart &mechPart);
	friend QDataStream & operator >> (QDataStream &in, MechPart &mechPart);

private:
	const MechPartBase *base;

	int armorValue;
	int internalValue;
	QList <Weapon *> weapons;

	void clearData();
};

#endif // MECH_PART_H
