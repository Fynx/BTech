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

#include "BTCommon/BiHash.h"
#include "BTCommon/Position.h"

template <typename T, typename U>
BiHash <T, U>::BiHash(std::initializer_list <std::pair <T, U> > initList)
{
	for (const std::pair <T, U> &p : initList)
		insert(p.first, p.second);
}

template <typename T, typename U>
U BiHash <T, U>::operator [] (const T &arg) const
{
	return left[arg];
}

template <typename T, typename U>
T BiHash <T, U>::operator [] (const U &arg) const
{
	return right[arg];
}

template <typename T, typename U>
bool BiHash <T, U>::contains(const T &arg) const
{
	return left.contains(arg);
}

template <typename T, typename U>
bool BiHash <T, U>::contains(const U &arg) const
{
	return right.contains(arg);
}

template <typename T, typename U>
void BiHash <T, U>::insert(const T &lhs, const U &rhs)
{
	left.insert(lhs, rhs);
	right.insert(rhs, lhs);
}

template <typename T, typename U>
void BiHash <T, U>::insert(const U &lhs, const T &rhs)
{
	right.insert(lhs, rhs);
	left.insert(rhs, lhs);
}

template <typename T, typename U>
int BiHash <T, U>::size() const
{
	return left.size();
}

template class BiHash <BTech::EffectType,   QString>;
template class BiHash <BTech::EntityType,   QString>;
template class BiHash <BTech::GamePhase,    QString>;
template class BiHash <BTech::GameVersion,  QString>;
template class BiHash <BTech::MechPartType, QString>;
template class BiHash <BTech::MechPartSide, QString>;
template class BiHash <BTech::Range,        QString>;
template class BiHash <BTech::Terrain,      QString>;
template class BiHash <BTech::WeaponType,   QString>;
template class BiHash <Direction,           QString>;
