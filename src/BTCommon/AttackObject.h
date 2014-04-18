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

#ifndef ATTACKOBJECT_H
#define ATTACKOBJECT_H

#include "BTCommon/Position.h"
#include "BTCommon/Weapon.h"

#include <QtWidgets>

class AttackObject
{

public:
	static int getRangeModifier(BTech::ModifierType type, BTech::Range range);
	static int getDirectionModifier(BTech::ModifierType type, Direction direction);
	static int getTerrainModifier(BTech::ModifierType type, const LineOfSight &path);

	AttackObject();

	int getDistance() const;
	void setDistance(int distance);

	Direction getDirection() const;
	void setDirection(Direction direction);

	BTech::CombatAction getActionType() const;
	void setActionType(const BTech::CombatAction actionType);

	int getDamage() const;
	void setDamage(int damage);

	const WeaponHolder * getWeaponHolder() const;
	void setWeaponHolder(const WeaponHolder *weaponHolder);
	const Weapon * getWeapon() const;

	int getModifier(BTech::ModifierType type, BTech::Modifier modifier) const;
	void setModifier(BTech::ModifierType type, BTech::Modifier modifier, int value);
	void addModifier(BTech::ModifierType type, BTech::Modifier modifier, int value);
	QHash <BTech::Modifier, int> getModifiers(BTech::ModifierType type) const;
	int getTotalModifier(BTech::ModifierType type) const;

	void setIneffective();
	bool isEffective() const;

private:
	static int getRangeAttackModifier_BBD(BTech::Range range);
	static int getRangeAttackModifier_ABD(BTech::Range range);
	static int getDirectionArmorPenetrationModifier_BBD(Direction direction);
	static int getDirectionArmorPenetrationModifier_ABD(Direction direction);
	static int getTerrainAttackModifier_BBD(const LineOfSight &path);
	static int getTerrainAttackModifier_ABD(const LineOfSight &path);

	static const QHash <QPair <BTech::GameVersion, BTech::ModifierType>, int (*)(BTech::Range)> rangeModifier;
	static const QHash <QPair <BTech::GameVersion, BTech::ModifierType>, int (*)(Direction)> directionModifier;
	static const QHash <QPair <BTech::GameVersion, BTech::ModifierType>, int (*)(const LineOfSight &)> terrainModifier;

	int distance;
	Direction direction;
	BTech::CombatAction actionType;
	int damage;
	const WeaponHolder *weaponHolder;
	QHash <QPair <BTech::ModifierType, BTech::Modifier>, int> modifiers;
};

/**
 * \class Attackable
 * Provides info about the attack.
 */
class Attackable
{
public:
	virtual ~Attackable();

	void setAttackObject(const AttackObject &attackObject);
	AttackObject getAttackObject() const;
	bool hasAttackObject() const;
	void removeAttackObject();

	void receiveAttack();
	QList <AttackObject> getIncomingAttacks() const;

protected:
	AttackObject attackObject;
	QList <AttackObject> incomingAttacks;
};

#endif // ATTACKOBJECT_H
