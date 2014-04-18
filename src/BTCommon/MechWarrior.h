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

#ifndef MECH_WARRIOR_H
#define MECH_WARRIOR_H

#include <QtWidgets>

namespace BTech {
	enum class Skill : quint8 {
		NoSkill,
		Gunnery,
	};

	QDataStream & operator << (QDataStream &out, const BTech::Skill &skill);
	QDataStream & operator >> (QDataStream &in, BTech::Skill &skill);
}

class MechWarrior
{

public:
	MechWarrior();

	int getSkill(const BTech::Skill &skill);

	friend QDataStream & operator << (QDataStream &out, const MechWarrior &mechWarrior);
	friend QDataStream & operator >> (QDataStream &in, MechWarrior &mechWarrior);
	friend QDebug & operator << (QDebug out, const MechWarrior &mechWarrior);

private:
	QMap <BTech::Skill, int> skills;
};

#endif // MECH_WARRIOR_H
