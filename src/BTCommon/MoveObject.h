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

#ifndef MOVEOBJECT_H
#define MOVEOBJECT_H

#include <QtWidgets>
#include "BTCommon/Position.h"

/**
 * WARNING
 * These classess contain some decent amount of data.
 * However, they are used as completely copiable objects, what means that NO pointer or reference
 * to a class variable should be returned by any class function. Objects of these classes are often
 * temporary so using references and pointers is highly error-prone.
 */

/**
 * \class MovementObject
 * MovementObject specifies the movement of the Unit.
 */
class MovementObject
{
public:
	MovementObject();
	MovementObject(BTech::MovementAction action);
	MovementObject(Position src,
	               int movePoints,
	               BTech::MovementAction action);
	Position getSrc() const;
	int getMovePoints() const;
	int getTerrainPenalty(BTech::Terrain terrain) const;
	int getHeightPenalty(int heightDifference) const;
	BTech::MovementAction getAction() const;
	QList <QPair <Direction, Direction> >  getAllowedMoves() const;

private:
	static const QHash <BTech::MovementAction, QList <QPair <Direction, Direction> > > allowedMoves;

	Position src;
	int movePoints;
	BTech::MovementAction action;
};

/**
 * \class MoveObject
 * MoveObject is a container for the data specyfiying the given move of the Unit.
 */
class MoveObject : public MovementObject
{
public:
	MoveObject();
	MoveObject(BTech::MovementAction action);
	MoveObject(const MovementObject &movement,
	           Position dest,
	           int movePointsUsed,
	           int distance,
	           const QList <Position> &path);
	MoveObject(Position src,
	           Position dest,
	           int movePoints,
	           int movePointsUsed,
	           int distance,
	           BTech::MovementAction action,
	           const QList <Position> &path);

	Position getDest() const;
	int getMovePoints() const;
	int getDistance() const;
	QList <Position> getPath() const;

private:
	Position dest;
	int movePointsUsed;
	int distance;
	QList <Position> path;
};

namespace BTech {

	const int MAX_CROSSABLE_HEIGHT_DIFFERENCE = 2;

	const int INF_MOVE_MODIFIER = 1000;
}

#endif // MOVEOBJECT_H
