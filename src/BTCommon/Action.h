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

#ifndef ACTION_H
#define ACTION_H

#include <QtWidgets>
#include "BTCommon/Utils.h"

class Weapon;
class WeaponHolder;

/**
 * \class Action
 * Represents an action.
 */
class Action {

public:
	Action(BTech::ActionType actionType = BTech::ActionType::Idle, WeaponHolder *weaponHolder = nullptr);
	Action(BTech::ActionType actionType, Weapon *weapon);

	inline bool operator==(const Action &rhs) const;

	BTech::ActionType getActionType() const;
	bool isCombat() const;
	bool isMovement() const;

	Weapon * getWeapon() const;
	void setWeapon(Weapon *weapon);

	WeaponHolder * getWeaponHolder() const;
	void setWeaponHolder(WeaponHolder *weaponHolder);

private:
	BTech::ActionType actionType;

	Weapon *weapon;
	WeaponHolder *weaponHolder;
};

bool Action::operator==(const Action &rhs) const
{
	return this->actionType == rhs.actionType
	    && this->weapon == rhs.weapon
	    && this->weaponHolder == rhs.weaponHolder;
}

#endif // ACTION_H