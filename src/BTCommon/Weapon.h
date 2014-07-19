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

#ifndef WEAPON_H
#define WEAPON_H

#include <QtWidgets>
#include "BTCommon/Rules.h"

class Weapon;
class WeaponBase;

/**
 * \class WeaponHolder
 * WeaponHolder contains info about the thing which contains the given weapon. Simple.
 */
class WeaponHolder
{

public:
	WeaponHolder(const Weapon *weapon = nullptr);
	virtual ~WeaponHolder();

	virtual QString getUnitName() const = 0;
	virtual QString getOwnerName() const = 0;

	void setCurrentWeapon(const Weapon *weapon) const; //!
	const Weapon * getCurrentWeapon() const;

private:
	mutable const Weapon *currentWeapon;
};

/**
 * \class Weapon
 * Weapon represents all types of weapons that can be found in the world of BTech.
 */
class Weapon
{

public:
	Weapon(const WeaponBase *base = nullptr);

	explicit operator QString () const;

	void setBase(const WeaponBase *base);

	QString getName() const;

	BTech::Range distanceToRange(int distance) const;
	int getMinimumRangeModifier(int distance) const;
	int getRangeModifier(BTech::Range range) const;
	int getRangeModifier(int distance) const;

	int getMinRange() const;
	int getMaxRange(BTech::Range range) const;

	int getHeat() const;
	int getDamage() const;
	qreal getTonnage() const;
	int getCriticalSpaces() const;
	int getAmmoShotsPerTon() const;
	int getMissilesNumberShot() const;

	const WeaponHolder * getWeaponHolder() const;
	void setWeaponHolder(const WeaponHolder *weaponHolder);

	void setUsed(bool used);
	bool isUsed() const;

	static const int MAX_AMMO_SHOTS_PER_TON = 10000;

	friend QDataStream & operator << (QDataStream &out, const Weapon &weapon);
	friend QDataStream & operator >> (QDataStream &in, Weapon &weapon);

private:
	const WeaponBase *base;

	const WeaponHolder *weaponHolder;

	bool used;

	void clear();
};

#endif // WEAPON_H
