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

#include "BTCommon/MoveObject.h"

/**
 * \class MovementObject
 */

MovementObject::MovementObject()
{}

MovementObject::MovementObject(BTech::ActionType actionType)
	: actionType(actionType)
{
	Q_ASSERT(BTech::movementActions.contains(actionType));
}

MovementObject::MovementObject(Position src,
                               int movePoints,
                               BTech::ActionType actionType)
	: src(src), movePoints(movePoints), actionType(actionType)
{
	Q_ASSERT(BTech::movementActions.contains(actionType));
}

static QList <QPair <Direction, Direction> > getAllDirectionPairs()
{
	static const QList <QPair <Direction, Direction> > result = []() {
		QList <QPair <Direction, Direction> > dirPairs;
		for (Direction l : BTech::directions)
			for (Direction r : BTech::directions)
				dirPairs.append({l, r});
		return dirPairs;
	}();
	return result;
}

const QHash <BTech::ActionType, QList <QPair <Direction, Direction> > > MovementObject::allowedMoves {
	{BTech::ActionType::Idle, {} },
	{BTech::ActionType::Walk, { {BTech::DirectionFront, BTech::DirectionFront},
	                            {BTech::DirectionRear,  BTech::DirectionFront}, } },
	{BTech::ActionType::Run,  { {BTech::DirectionFront, BTech::DirectionFront}, } },
	{BTech::ActionType::Jump, getAllDirectionPairs()},
};

Position MovementObject::getSrc() const
{
	return src;
}

int MovementObject::getMovePoints() const
{
	return movePoints;
}

int MovementObject::getTerrainPenalty(BTech::Terrain terrain) const
{
	return BTech::travelPenalty[terrain];
}

int MovementObject::getHeightPenalty(int heightDifference) const
{
	int diff = qAbs(heightDifference);
	if (diff > BTech::MAX_CROSSABLE_HEIGHT_DIFFERENCE)
		return BTech::INF_MOVE_MODIFIER;
	return diff;
}

BTech::ActionType MovementObject::getActionType() const
{
	return actionType;
}

QList <QPair <Direction, Direction> > MovementObject::getAllowedMoves() const
{
	return allowedMoves[actionType];
}

/**
 * \class MoveObject
 */

MoveObject::MoveObject()
{}

MoveObject::MoveObject(BTech::ActionType actionType)
	: MovementObject(actionType)
{}

MoveObject::MoveObject(const MovementObject &movement,
                       Position dest,
                       int movePointsUsed,
                       int distance,
                       const QList <Position> &path)
	: MovementObject(movement), dest(dest), movePointsUsed(movePointsUsed), distance(distance), path(path)
{}

MoveObject::MoveObject(Position src,
                       Position dest,
                       int movePoints,
                       int movePointsUsed,
                       int distance,
                       BTech::ActionType action,
                       const QList <Position> &path)
	: MovementObject(src, movePoints, action), dest(dest), movePointsUsed(movePointsUsed), distance(distance), path(path)
{}

Position MoveObject::getDest() const
{
	return dest;
}

int MoveObject::getMovePoints() const
{
	return movePointsUsed;
}

int MoveObject::getDistance() const
{
	return distance;
}

QList <Position> MoveObject::getPath() const
{
	return path;
}