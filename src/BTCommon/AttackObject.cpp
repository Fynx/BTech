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

#include "BTCommon/EnumHashFunctions.h"

#include "BTCommon/AttackObject.h"
#include "BTCommon/Bases/WeaponBase.h"
#include "BTCommon/Utils.h"

/**
 * \class AttackObject
 */

int AttackObject::getRangeModifier(BTech::ModifierType type, BTech::Range range)
{
	return rangeModifier[{Rules::getVersion(), type}](range);
}

int AttackObject::getDirectionModifier(BTech::ModifierType type, Direction direction)
{
	return directionModifier[{Rules::getVersion(), type}](direction);
}

int AttackObject::getTerrainModifier(BTech::ModifierType type, const LineOfSight &lineOfSight)
{
	return terrainModifier[{Rules::getVersion(), type}](lineOfSight);
}

AttackObject::AttackObject()
	: distance(0)
{}

int AttackObject::getDistance() const
{
	return distance;
}

void AttackObject::setDistance(int distance)
{
	this->distance = distance;
}

Direction AttackObject::getDirection() const
{
	return direction;
}

void AttackObject::setDirection(Direction direction)
{
	this->direction = direction;
}

BTech::CombatAction AttackObject::getActionType() const
{
	return actionType;
}

void AttackObject::setActionType(const BTech::CombatAction actionType)
{
	this->actionType = actionType;
}

int AttackObject::getDamage() const
{
	return damage;
}

void AttackObject::setDamage(int damage)
{
	this->damage = damage;
}

const WeaponHolder * AttackObject::getWeaponHolder() const
{
	return weaponHolder;
}

void AttackObject::setWeaponHolder(const WeaponHolder *weaponHolder)
{
	this->weaponHolder = weaponHolder;
}

const Weapon * AttackObject::getWeapon() const
{
	// If weaponHolder is nullptr the program will rightously end with segfault.
	return getWeaponHolder()->getCurrentWeapon();
}

void AttackObject::setModifier(BTech::ModifierType type, BTech::Modifier modifier, int value)
{
	modifiers[{type, modifier}] = value;
}

void AttackObject::addModifier(BTech::ModifierType type, BTech::Modifier modifier, int value)
{
	int oldValue = modifiers.contains({type, modifier})
		? modifiers[{type, modifier}]
		: 0;
	setModifier(type, modifier, value + oldValue);
}

int AttackObject::getModifier(BTech::ModifierType type, BTech::Modifier modifier) const
{
	if (modifiers.contains({type, modifier}))
		return modifiers[{type, modifier}];
	return 0;
}

QHash <BTech::Modifier, int> AttackObject::getModifiers(BTech::ModifierType type) const
{
	QHash <BTech::Modifier, int> result;
	for (BTech::Modifier modifier : BTech::modifiers)
		result[modifier] = modifiers[{type, modifier}];
	return result;
}

int AttackObject::getTotalModifier(BTech::ModifierType type) const
{
	int sum = 0;
	for (BTech::Modifier modifier : BTech::modifiers)
		sum += getModifier(type, modifier);
	return sum;
}

void AttackObject::setIneffective()
{
	setDistance(0);
}

bool AttackObject::isEffective() const
{
	return distance > 0;
}

/*
 * BattleDroids (Basic)
 * Base To-Hit Numbers, page 5
 */
int AttackObject::getRangeAttackModifier_BBD(BTech::Range range)
{
	if (range == BTech::Range::Contact)
		return 0;
	else
		return 2;
}

int AttackObject::getRangeAttackModifier_ABD(BTech::Range range)
{
	return 0; //TODO
}

/*
 * BattleDroids (Basic)
 * Armor Value Modifier Diagram, page 6
 */
int AttackObject::getDirectionArmorPenetrationModifier_BBD(Direction direction)
{
	if (direction == BTech::DirectionRear)
		return 2;
	else if (direction == BTech::DirectionLeftRear || direction == BTech::DirectionRightRear)
		return 1;
	return 0;
}

int AttackObject::getDirectionArmorPenetrationModifier_ABD(Direction direction)
{
	return 0;
}

/*
 * BattleDroids (Basic)
 * Terrain Modifiers, page 5
 */

int AttackObject::getTerrainAttackModifier_BBD(const LineOfSight &path)
{
	if (path.lightWoods > 2 || path.heavyWoods > 0 || path.heightBarrier || path.heightBetween > 0)
		return BTech::INF_ATTACK_MODIFIER;
	int srcHeavyWoods = (int)(path.srcTerrain == BTech::Terrain::HeavyWoods);
	int destHeavyWoods = (int)(path.destTerrain == BTech::Terrain::HeavyWoods);

	return path.lightWoods + 2 * (srcHeavyWoods + destHeavyWoods);
}

/*
 * BattleDroids (Advanced)
 * Terrain Modifiers, pages 9-10
 */

int AttackObject::getTerrainAttackModifier_ABD(const LineOfSight &path)
{
	if (path.heightBarrier || path.heightBetween > 1)
		return BTech::INF_ATTACK_MODIFIER;
	int srcLightWoods = (int)(path.srcTerrain == BTech::Terrain::LightWoods);
	int destLightWoods = (int)(path.srcTerrain == BTech::Terrain::LightWoods);
	int lightWoodsModifier = path.lightWoods + srcLightWoods + 2 * destLightWoods;

	int srcHeavyWoods = (int)(path.srcTerrain == BTech::Terrain::HeavyWoods);
	int destHeavyWoods = (int)(path.destTerrain == BTech::Terrain::HeavyWoods);
	int heavyWoodsModifier = 2 * (path.heavyWoods + srcHeavyWoods + 2 * destHeavyWoods);

	int waterModifier = (int)(path.srcTerrain == BTech::Terrain::Water) - (int)(path.destTerrain == BTech::Terrain::Water);

	int partialCoverModifier = 3 * (int)(path.heightBetween == 1);

	return lightWoodsModifier + heavyWoodsModifier + waterModifier + partialCoverModifier;
}

const QHash <QPair <BTech::GameVersion, BTech::ModifierType>, int (*)(BTech::Range)> AttackObject::rangeModifier {
	{ {BTech::GameVersion::BasicBattleDroids,    BTech::ModifierType::Attack}, getRangeAttackModifier_BBD},
	{ {BTech::GameVersion::AdvancedBattleDroids, BTech::ModifierType::Attack}, getRangeAttackModifier_ABD},
};

const QHash <QPair <BTech::GameVersion, BTech::ModifierType>, int (*)(Direction)> AttackObject::directionModifier {
	{ {BTech::GameVersion::BasicBattleDroids,    BTech::ModifierType::ArmorPenetration}, getDirectionArmorPenetrationModifier_BBD},
	{ {BTech::GameVersion::AdvancedBattleDroids, BTech::ModifierType::ArmorPenetration}, getDirectionArmorPenetrationModifier_ABD},
};

const QHash <QPair <BTech::GameVersion, BTech::ModifierType>, int (*)(const LineOfSight &)> AttackObject::terrainModifier {
	{ {BTech::GameVersion::BasicBattleDroids,    BTech::ModifierType::Attack}, getTerrainAttackModifier_BBD},
	{ {BTech::GameVersion::AdvancedBattleDroids, BTech::ModifierType::Attack}, getTerrainAttackModifier_ABD},
};

/**
 * \class Attackable
 */

Attackable::~Attackable()
{}

AttackObject Attackable::getAttackObject() const
{
	return attackObject;
}

void Attackable::setAttackObject(const AttackObject &attackObject)
{
	this->attackObject = attackObject;
}

bool Attackable::hasAttackObject() const
{
	return attackObject.isEffective();
}

void Attackable::removeAttackObject()
{
	attackObject.setIneffective();
}

void Attackable::receiveAttack()
{
	incomingAttacks.append(getAttackObject());
}

QList <AttackObject> Attackable::getIncomingAttacks() const
{
	return incomingAttacks;
}