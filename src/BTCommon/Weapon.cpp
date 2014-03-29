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
