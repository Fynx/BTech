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

/**
 * \class Action
 */

Action::~Action()
{}

bool Action::operator == (const Action *rhs) const
{
	if (this->getActionType() != rhs->getActionType())
		return false;

	switch (getActionType()) {
		case Action::Type::Movement:
			return static_cast<const MovementAction *>(this) == static_cast<const MovementAction *>(rhs);
		case Action::Type::Combat:
			return static_cast<const CombatAction *>(this) == static_cast<const CombatAction *>(rhs);
		default:
			return false;
	}
}

/**
 * \class MovementAction
 */

MovementAction::MovementAction(BTech::MovementAction type)
	: type(type)
{}

bool MovementAction::operator == (const MovementAction *rhs) const
{
	return this->getType() == rhs->getType();
}

Action::Type MovementAction::getActionType() const
{
	return Action::Type::Movement;
}

BTech::MovementAction MovementAction::getType() const
{
	return type;
}

/**
 * \class CombatAction
 */

CombatAction::CombatAction(BTech::CombatAction type,
                           const WeaponHolder *weaponHolder,
                           BTech::MechPartSide side)
	: type(type), weaponHolder(weaponHolder), side(side)
{}

bool CombatAction::operator == (const CombatAction *rhs) const
{
	return this->getType() == rhs->getType()
	    && this->getWeaponHolder() == rhs->getWeaponHolder();
}

Action::Type CombatAction::getActionType() const
{
	return Action::Type::Combat;
}

BTech::CombatAction CombatAction::getType() const
{
	return type;
}

const WeaponHolder * CombatAction::getWeaponHolder() const
{
	return weaponHolder;
}

void CombatAction::setWeaponHolder(const WeaponHolder *weaponHolder)
{
	this->weaponHolder = weaponHolder;
}

const Weapon * CombatAction::getWeapon() const
{
	return getWeaponHolder() == nullptr
		? nullptr
		: getWeaponHolder()->getCurrentWeapon();
}

bool CombatAction::hasWeapon() const
{
	return weaponHolder->getCurrentWeapon() != nullptr;
}

BTech::MechPartSide CombatAction::getMechPartSide() const
{
	return side;
}