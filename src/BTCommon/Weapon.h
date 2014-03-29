#ifndef WEAPON_H
#define WEAPON_H

#include <QtWidgets>
#include "BTCommon/Position.h"
#include "BTCommon/Rules.h"
#include "BTCommon/Utils.h"
#include "BTCommon/WeaponBase.h"

class Weapon;

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
