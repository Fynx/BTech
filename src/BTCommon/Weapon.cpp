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

#include "BTCommon/Bases/WeaponBase.h"
#include "BTCommon/Models/WeaponModel.h"
#include "BTCommon/Weapon.h"

/**
 * \class WeaponHolder
 */

WeaponHolder::WeaponHolder(const Weapon *weapon)
	: currentWeapon(weapon)
{}

WeaponHolder::~WeaponHolder()
{}

void WeaponHolder::setCurrentWeapon(const Weapon *weapon) const
{
	this->currentWeapon = weapon;
}

const Weapon * WeaponHolder::getCurrentWeapon() const
{
	return currentWeapon;
}

/**
 * \class Weapon
 */

Weapon::Weapon(const WeaponBase *base)
	: weaponHolder(nullptr)
{
	setBase(base);
	clear();
}

Weapon::operator QString () const
{
	return getName();
}

void Weapon::setBase(const WeaponBase *base)
{
	this->base = base;

	if (base == nullptr)
		return;
}

QString Weapon::getName() const
{
	return base->getName();
}

int Weapon::getRangeModifier(BTech::Range range) const
{
	return base->getRangeModifier(range);
}

int Weapon::getRangeModifier(int distance) const
{
	return base->getRangeModifier(distance);
}

BTech::Range Weapon::distanceToRange(int distance) const
{
	return base->distanceToRange(distance);
}

int Weapon::getMinRange() const
{
	return base->getMinRange();
}

int Weapon::getMaxRange(BTech::Range range) const
{
	return base->getMaxRange(range);
}

int Weapon::getHeat() const
{
	return base->getHeat();
}

int Weapon::getDamage() const
{
	return base->getDamage();
}

qreal Weapon::getTonnage() const
{
	return base->getTonnage();
}

int Weapon::getCriticalSpaces() const
{
	return base->getCriticalSpaces();
}

int Weapon::getAmmoShotsPerTon() const
{
	return base->getAmmoPerTon();
}

int Weapon::getMissilesNumberShot() const
{
	return base->getMissilesPerShot();
}

const WeaponHolder * Weapon::getWeaponHolder() const
{
	return weaponHolder;
}

void Weapon::setWeaponHolder(const WeaponHolder *weaponHolder)
{
	this->weaponHolder = weaponHolder;
}

void Weapon::setUsed(bool used)
{
	this->used = used;
}

bool Weapon::isUsed() const
{
	return used;
}

QDataStream & operator << (QDataStream &out, const Weapon &weapon)
{
	out << weapon.base->getUid();
	return out;
}

QDataStream & operator >> (QDataStream &in, Weapon &weapon)
{
	UID uid;
	in >> uid;
	weapon.setBase(WeaponModel::getWeapon(uid));

	return in;
}

void Weapon::clear()
{
	setUsed(false);
}
