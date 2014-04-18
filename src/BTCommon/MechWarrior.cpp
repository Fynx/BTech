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
#include "BTCommon/MechWarrior.h"

/**
 * \enum BTech::Skill
 */

QDataStream & BTech::operator << (QDataStream &out, const BTech::Skill &skill)
{
	out << toUnderlying(skill);
	return out;
}

QDataStream & BTech::operator >> (QDataStream &in, BTech::Skill &skill)
{
	in >> toUnderlyingRef(skill);
	return in;
}

/**
 * \class MechWarrior
 */

MechWarrior::MechWarrior()
{
	skills[BTech::Skill::Gunnery] = 4;	// works for <= Advanced.
}

int MechWarrior::getSkill(const BTech::Skill &skill)
{
	return skills[skill];
}

QDataStream & operator << (QDataStream &out, const MechWarrior &mechWarrior)
{
	out << mechWarrior.skills;
	return out;
}

QDataStream & operator >> (QDataStream &in, MechWarrior &mechWarrior)
{
	in >> mechWarrior.skills;
	return in;
}
