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
#include "BTCommon/Weapon.h"

/**
 * \class Action
 * Represents an action.
 */
class Action {

public:
	virtual ~Action();

	bool operator == (const Action *rhs) const;

	enum class Type {
		Movement,
		Combat,
	};

	virtual Type getActionType() const = 0;
};

/**
 * \class MovementAction
 * Represents an action that includes changing position.
 */
class MovementAction : public Action {

public:
	MovementAction(BTech::MovementAction type);

	bool operator == (const MovementAction *rhs) const;

	Action::Type getActionType() const;
	BTech::MovementAction getType() const;

private:
	BTech::MovementAction type;
};

/**
 * \class CombatAction
 * Represents an action that includes hurting other things.
 */
class CombatAction : public Action {

public:
	CombatAction(BTech::CombatAction type,
	             const WeaponHolder *weaponHolder = nullptr,
	             BTech::MechPartSide = BTech::MechPartSide::Front);

	bool operator == (const CombatAction *rhs) const;

	Action::Type getActionType() const;
	BTech::CombatAction getType() const;

	const WeaponHolder * getWeaponHolder() const;
	void setWeaponHolder(const WeaponHolder *weaponHolder);

	const Weapon * getWeapon() const;
	bool hasWeapon() const;

	BTech::MechPartSide getMechPartSide() const;

private:
	BTech::CombatAction type;
	const WeaponHolder *weaponHolder;
	BTech::MechPartSide side;
};

#endif // ACTION_H