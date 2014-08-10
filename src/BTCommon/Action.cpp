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

#include "BTCommon/Action.h"
#include "BTCommon/EnumSerialization.h"
#include "BTCommon/Weapon.h"

/**
 * \class Action
 */

Action::Action(BTech::ActionType actionType, WeaponHolder *weaponHolder)
	: actionType(actionType), weapon(nullptr), weaponHolder(weaponHolder)
{}

Action::Action(BTech::ActionType actionType, Weapon *weapon)
	: actionType(actionType), weapon(weapon)
{}

BTech::ActionType Action::getActionType() const
{
	return actionType;
}

bool Action::isCombat() const
{
	return BTech::combatActions.contains(actionType);
}

bool Action::isMovement() const
{
	return BTech::movementActions.contains(actionType);
}

Weapon * Action::getWeapon() const
{
	return weapon;
}

void Action::setWeapon(Weapon *weapon)
{
	this->weapon = weapon;
}

WeaponHolder * Action::getWeaponHolder() const
{
	if (weapon == nullptr)
		return weaponHolder;
	else
		return weapon->getWeaponHolder();
}

void Action::setWeaponHolder(WeaponHolder *weaponHolder)
{
	this->weaponHolder = weaponHolder;
}
