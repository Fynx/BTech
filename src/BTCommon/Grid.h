/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
Copyright (C) 2014 by Bartosz Szreder <szreder [at] mimuw [dot] edu [dot] pl>
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

#ifndef GRID_H
#define GRID_H

#include <QtWidgets>
#include "BTCommon/AttackObject.h"
#include "BTCommon/MoveObject.h"
#include "BTCommon/Position.h"

class Hex;
class MechEntity;
class MovementObject;
class Player;

/**
 * \class Grid
 * Provides Map and GraphicsMap with Hex-managing functions. For this it requires reference to QVector of pointers to Hexes.
 */
class Grid
{

public:
	Grid();

	void initGrid(const QVector <Hex *> &hexes);

	AttackObject getAttackObject(const MechEntity *attacker, const MechEntity *target) const;
	Hex * getHexByCoordinate(const Coordinate &coordinate) const;
	QList <Hex *> getShootRange(Hex *src, Direction direction) const;
	QList <MoveObject> getWalkRange(const MovementObject &movementObject) const;

private:
	Direction getAttackDirection(Hex *src, Hex *dest) const;
	Direction getAttackDirection(Direction unitDirection, Direction attackDirection) const;
	LineOfSight getLineOfSight(const Hex *src, const Hex *dest) const;
	bool lineOfSightExists(const Hex *src, const Hex *dest) const;

	QHash <Coordinate, Hex *> coordinateToHex;

	const Coordinate nextCoord(const Coordinate &coordinate, Direction direction) const;
	Hex * nextHex(Hex *hex, Direction direction) const;
	QList <QPair <const Hex *, const Hex *> > hexesBetween(const Hex *src, const Hex *dest) const;
	int getHexDistance(const Hex *src, const Hex *dest) const;

	LineOfSight visibilityScore(const Hex *hex, const Hex *src, const Hex *dest) const;
	LineOfSight pairVisibilityScore(QPair <const Hex *, const Hex *> hexes, const Hex *src, const Hex *dest) const;
};

#endif // GRID_H
