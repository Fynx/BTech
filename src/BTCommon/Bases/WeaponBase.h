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

#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include <QtWidgets>
#include "BTCommon/CommonStrings.h"
#include "BTCommon/EnumSerialization.h"
#include "BTCommon/FileIO.h"
#include "BTCommon/Paths.h"
#include "BTCommon/Uid.h"
#include "BTCommon/Utils.h"

class WeaponBase
{

public:
	WeaponBase();
	WeaponBase(const QString &name, UID uid = EmptyUid);
	WeaponBase(const WeaponBase &weapon);
	WeaponBase(const QString &name,
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
	           int missilesNumberShot);

	WeaponBase & operator = (const WeaponBase &weapon) = default;

	explicit operator QString () const;

	QString getName() const;
	void setName(const QString &name);

	BTech::WeaponType getWeaponType() const;
	void setWeaponType(BTech::WeaponType weaponType);

	int getRangeModifier(int distance) const;
	int getRangeModifier(BTech::Range range) const;
	BTech::Range distanceToRange(int distance) const;

	int getMinRange() const;
	void setMinRange(int minRange);

	int getMaxRange(BTech::Range range) const;
	void setMaxRange(BTech::Range range, int maxRange);

	int getHeat() const;
	void setHeat(int heat);

	int getDamage() const;
	void setDamage(int damage);

	int getTonnage() const;
	void setTonnage(int tonnage);

	int getCriticalSpaces() const;
	void setCriticalSpaces(int criticalSpaces);

	int getAmmoPerTon() const;
	void setAmmoPerTon(int ammoPerTon);

	int getMissilesPerShot() const;
	void setMissilesPerShot(int missilesPerShot);

	static const int MAX_AMMO_SHOTS_PER_TON = 10000;

	friend QDataStream & operator << (QDataStream &out, const WeaponBase &weapon);
	friend QDataStream & operator >> (QDataStream &in, WeaponBase &weapon);

	friend QDebug & operator << (QDebug &out, const WeaponBase &weapon);

	UID getUid() const;

private:
	void load(const QString &name,
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
	          int missilesPerShot);
	void load(const WeaponBase &weapon);

	QString name;

	BTech::WeaponType type;

	static const QHash <BTech::Range, int> rangeModifier;
	int minRange;
	QHash <BTech::Range, int> maxRange;

	int heat;
	int damage;
	int tonnage;
	int criticalSpaces;
	int ammoPerTon;
	int missilesPerShot;

	UID uid;
};

namespace BTech {

	//TODO CFiend przydałaby się zmiana nazwy: czego to jest modifier? AttackModifier?
	enum class Modifier : quint8 {
		Base,
		Range,
		Direction,
		Terrain,
		Height,
		AttackerMovement,
		TargetMovement
	};

	extern const QHash <Modifier, QString> modifierStringChange;

	static const QVector <Modifier> modifiers {
		Modifier::Base,
		Modifier::Range,
		Modifier::Direction,
		Modifier::Terrain,
		Modifier::Height,
		Modifier::AttackerMovement,
		Modifier::TargetMovement
	};

	enum class ModifierType : quint8 {
		Attack,
		ArmorPenetration
	};

	static const int INF_ATTACK_MODIFIER           = 100;
	static const int OUT_OF_RANGE_ATTACK_MODIFIER  = INF_ATTACK_MODIFIER;

	static const int MAXIMAL_MAX_SHORT_RANGE  = 97;
	static const int MAXIMAL_MAX_MEDIUM_RANGE = MAXIMAL_MAX_SHORT_RANGE + 1;
	static const int MAXIMAL_MAX_LONG_RANGE   = MAXIMAL_MAX_MEDIUM_RANGE + 1;

	extern const QHash <BTech::Range, int> maximalMaxRange;

	static const int MINIMAL_MAX_SHORT_RANGE  = 1;
	static const int MINIMAL_MAX_MEDIUM_RANGE = MINIMAL_MAX_SHORT_RANGE + 1;
	static const int MINIMAL_MAX_LONG_RANGE   = MINIMAL_MAX_MEDIUM_RANGE + 1;

	extern const QHash <BTech::Range, int> minimalMaxRange;

	static const int DEFAULT_AMMO_PER_TON         = 0;
	static const int DEFAULT_MISSILES_PER_SHOT    = 1;

	/*
	 * BatleDroids (Advanced)
	 * Missile Hit Table, page 10
	 */
	static const QHash <QPair <int, BTech::DiceRoll>, int> missileHitTable {
		{{ 2,  2},  1}, {{ 4,  2},  1}, {{ 5,  2},  1}, {{ 6,  2},  2}, {{10,  2},  3}, {{15,  2},  5}, {{20,  2},  6},
		{{ 2,  3},  1}, {{ 4,  3},  2}, {{ 5,  3},  2}, {{ 6,  3},  2}, {{10,  3},  3}, {{15,  3},  5}, {{20,  3},  6},
		{{ 2,  4},  1}, {{ 4,  4},  2}, {{ 5,  4},  2}, {{ 6,  4},  3}, {{10,  4},  4}, {{15,  4},  6}, {{20,  4},  9},
		{{ 2,  5},  1}, {{ 4,  5},  2}, {{ 5,  5},  3}, {{ 6,  5},  3}, {{10,  5},  6}, {{15,  5},  9}, {{20,  5}, 12},
		{{ 2,  6},  1}, {{ 4,  6},  2}, {{ 5,  6},  3}, {{ 6,  6},  4}, {{10,  6},  6}, {{15,  6},  9}, {{20,  6}, 12},
		{{ 2,  7},  1}, {{ 4,  7},  3}, {{ 5,  7},  3}, {{ 6,  7},  4}, {{10,  8},  6}, {{15,  8},  9}, {{20,  7}, 12},
		{{ 2,  8},  2}, {{ 4,  8},  3}, {{ 5,  8},  3}, {{ 6,  8},  4}, {{10,  8},  6}, {{15,  8},  9}, {{20,  8}, 12},
		{{ 2,  9},  2}, {{ 4,  9},  3}, {{ 5,  9},  4}, {{ 6,  9},  5}, {{10,  9},  8}, {{15,  9}, 12}, {{20,  9}, 16},
		{{ 2, 10},  2}, {{ 4, 10},  3}, {{ 5, 10},  4}, {{ 6, 10},  5}, {{10, 10},  8}, {{15, 10}, 12}, {{20, 10}, 16},
		{{ 2, 11},  2}, {{ 4, 11},  4}, {{ 5, 11},  5}, {{ 6, 11},  6}, {{10, 11}, 10}, {{15, 11}, 15}, {{20, 11}, 20},
		{{ 2, 12},  2}, {{ 4, 12},  4}, {{ 5, 12},  5}, {{ 6, 12},  6}, {{10, 12}, 10}, {{15, 12}, 15}, {{20, 12}, 20},
	};
}


#endif
