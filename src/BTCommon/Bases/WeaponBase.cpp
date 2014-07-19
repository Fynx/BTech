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

#include "BTCommon/EnumHashFunctions.h"
#include "BTCommon/Bases/WeaponBase.h"

/**
 * \class WeaponBase
 */

WeaponBase::WeaponBase()
	: uid(EmptyUid)
{
	maxRange[BTech::Range::Contact] = 1;
	load(QString(), BTech::WeaponType::Energy,
	     0, 0, 0, 0, 0, 0, 0, 0,
	     BTech::DEFAULT_AMMO_PER_TON, BTech::DEFAULT_MISSILES_PER_SHOT);
}

WeaponBase::WeaponBase(const QString &name, UID uid)
	: WeaponBase()
{
	setName(name);
	this->uid = uid;
}

WeaponBase::WeaponBase(const WeaponBase &weapon)
{
	load(weapon);
}

WeaponBase::WeaponBase(const QString &name,
                       BTech::WeaponType weaponType,
                       int minRange,
                       int maxShortRange,
                       int maxMediumRange,
                       int maxLongRange,
                       int heat,
                       int damage,
                       int tonnage,
                       int criticalSpaces,
                       int ammoShotsPerTon,
                       int missilesNumberShot)
{
	load(name, weaponType,
	     minRange, maxShortRange, maxMediumRange, maxLongRange,
	     heat, damage, tonnage, criticalSpaces, ammoShotsPerTon, missilesNumberShot);
}

WeaponBase::operator QString () const
{
	return getName();
}

QString WeaponBase::getName() const
{
	return name;
}

void WeaponBase::setName(const QString &name)
{
	this->name = name;
}

BTech::WeaponType WeaponBase::getWeaponType() const
{
	return type;
}

void WeaponBase::setWeaponType(BTech::WeaponType weaponType)
{
	this->type = weaponType;
}

int WeaponBase::getRangeModifier(int distance) const
{
	BTech::Range range = distanceToRange(distance);
	int minimumDistanceModifier = qMax(0, getMinRange() - distance);
	return getRangeModifier(range) + minimumDistanceModifier;
}

int WeaponBase::getRangeModifier(BTech::Range range) const
{
	return rangeModifier[range];
}

BTech::Range WeaponBase::distanceToRange(int distance) const
{
	for (BTech::Range range : BTech::attackRanges)
		if (distance <= maxRange[range])
			return range;
	return BTech::Range::OutOfRange;
}

int WeaponBase::getMinRange() const
{
	return minRange;
}

void WeaponBase::setMinRange(int minRange)
{
	this->minRange = minRange;
}

int WeaponBase::getMaxRange(BTech::Range range) const
{
	return maxRange[range];
}

void WeaponBase::setMaxRange(BTech::Range range, int maxRange)
{
	this->maxRange[range] = maxRange;
}

int WeaponBase::getHeat() const
{
	return heat;
}

void WeaponBase::setHeat(int heat)
{
	this->heat = heat;
}

int WeaponBase::getDamage() const
{
	return damage;
}

void WeaponBase::setDamage(int damage)
{
	this->damage = damage;
}

int WeaponBase::getTonnage() const
{
	return tonnage;
}

void WeaponBase::setTonnage(int tonnage)
{
	this->tonnage = tonnage;
}

int WeaponBase::getCriticalSpaces() const
{
	return criticalSpaces;
}

void WeaponBase::setCriticalSpaces(int criticalSpaces)
{
	this->criticalSpaces = criticalSpaces;
}

int WeaponBase::getAmmoPerTon() const
{
	return ammoPerTon;
}

void WeaponBase::setAmmoPerTon(int ammoPerTon)
{
	this->ammoPerTon = ammoPerTon;
}

int WeaponBase::getMissilesPerShot() const
{
	return missilesPerShot;
}

void WeaponBase::setMissilesPerShot(int missilesPerShot)
{
	this->missilesPerShot = missilesPerShot;
}

QDataStream & operator << (QDataStream &out, const WeaponBase &weapon)
{
	out << weapon.name << weapon.uid << weapon.type;

	for (BTech::Range range : BTech::shootRanges)
		out << weapon.maxRange[range];
	out << weapon.minRange;

	out << weapon.heat << weapon.damage << weapon.tonnage << weapon.criticalSpaces << weapon.ammoPerTon << weapon.missilesPerShot;

	return out;
}

QDataStream & operator >> (QDataStream &in, WeaponBase &weapon)
{
	in >> weapon.name >> weapon.uid >> weapon.type;

	for (BTech::Range range : BTech::shootRanges)
		in >> weapon.maxRange[range];
	in >> weapon.minRange;

	in >> weapon.heat >> weapon.damage >> weapon.tonnage >> weapon.criticalSpaces >> weapon.ammoPerTon >> weapon.missilesPerShot;

	return in;
}

QDebug & operator << (QDebug &out, const WeaponBase &weapon)
{
	out << weapon.name << "(" << weapon.uid << ")"
	    << "\ntype:                      " << BTech::weaponTypeStringChange[weapon.type]
	    << "\nstats:"
	    << "\n\tminimal range:           " << weapon.minRange
	    << "\n\tmaximal ranges:          "
	    << weapon.maxRange[BTech::Range::Short]
	    << weapon.maxRange[BTech::Range::Medium]
	    << weapon.maxRange[BTech::Range::Long]
	    << "\n\theat:                    " << weapon.heat
	    << "\n\tdamage:                  " << weapon.damage
	    << "\n\ttonnage:                 " << weapon.tonnage
	    << "\n\tcritical spaces:         " << weapon.criticalSpaces
	    << "\n\tammo shots per ton:      " << weapon.ammoPerTon
	    << "\n\tmissiles number per shot:" << weapon.missilesPerShot;

	return out;
}

UID WeaponBase::getUid() const
{
	return uid;
}

/*
 * BattleDroids (Advanced)
 * Range Modifier, page 9
 */
const QHash <BTech::Range, int> WeaponBase::rangeModifier {
	{ BTech::Range::Contact, 0 },
	{ BTech::Range::Contact, 0 },
	{ BTech::Range::Contact, 2 },
	{ BTech::Range::Contact, 4 },
	{ BTech::Range::Contact, BTech::INF_ATTACK_MODIFIER }
};

void WeaponBase::load(const QString &name,
                      BTech::WeaponType weaponType,
                      int minRange,
                      int maxShortRange,
                      int maxMediumRange,
                      int maxLongRange,
                      int heat,
                      int damage,
                      int tonnage,
                      int criticalSpaces,
                      int ammoPerTon,
                      int missilesPerShot)
{
	this->name = name;
	this->type = weaponType;
	this->minRange = minRange;
	this->maxRange[BTech::Range::Short] = maxShortRange;
	this->maxRange[BTech::Range::Medium] = maxMediumRange;
	this->maxRange[BTech::Range::Long] = maxLongRange;
	this->heat = heat;
	this->damage = damage;
	this->tonnage = tonnage;
	this->criticalSpaces = criticalSpaces;
	this->ammoPerTon = ammoPerTon;
	this->missilesPerShot = missilesPerShot;
}

void WeaponBase::load(const WeaponBase &weapon)
{
	return load(weapon.name,
		    weapon.type,
	            weapon.minRange,
	            weapon.maxRange[BTech::Range::Short],
	            weapon.maxRange[BTech::Range::Medium],
	            weapon.maxRange[BTech::Range::Long],
	            weapon.heat,
	            weapon.damage,
	            weapon.tonnage,
	            weapon.criticalSpaces,
	            weapon.ammoPerTon,
	            weapon.missilesPerShot);
}

/**
 * \namespace BTech
 */

const QHash <BTech::Modifier, QString> BTech::modifierStringChange {
	{BTech::Modifier::Base,             BTech::Strings::ModifierBase},
	{BTech::Modifier::Range,            BTech::Strings::ModifierRange},
	{BTech::Modifier::Direction,        BTech::Strings::ModifierDirection},
	{BTech::Modifier::Terrain,          BTech::Strings::ModifierTerrain},
	{BTech::Modifier::Height,           BTech::Strings::ModifierHeight},
	{BTech::Modifier::AttackerMovement, BTech::Strings::ModifierAttackerMovement},
	{BTech::Modifier::TargetMovement,   BTech::Strings::ModifierTargetMovement},
};

const QHash <BTech::Range, int> BTech::maximalMaxRange {
	{BTech::Range::Short,  BTech::MAXIMAL_MAX_SHORT_RANGE},
	{BTech::Range::Medium, BTech::MAXIMAL_MAX_MEDIUM_RANGE},
	{BTech::Range::Long,   BTech::MAXIMAL_MAX_LONG_RANGE},
};

const QHash <BTech::Range, int> BTech::minimalMaxRange {
	{BTech::Range::Short,  BTech::MINIMAL_MAX_SHORT_RANGE},
	{BTech::Range::Medium, BTech::MINIMAL_MAX_MEDIUM_RANGE},
	{BTech::Range::Long,   BTech::MINIMAL_MAX_LONG_RANGE},
};


