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

#ifndef OBJECTS_H
#define OBJECTS_H

#include <QtWidgets>
#include "BTCommon/Effect.h"
#include "BTCommon/MoveObject.h"
#include "BTCommon/Position.h"

/**
 * WARNING
 * If you want make Movable move, set destination coordinate. The best way to do this is to call setDestinationCoordinate.
 * However, you may need to separate setting destination rotation and coordinate.
 * In that case... for gods' sake, do it in order 1. COORDINATE 2. ROTATION.
 * If you don't, happy debugging.
 * The reason is simple: the program functions assume the signals from these classes go in order: Movable, Rotable.
 */

/**
 * \class Entity
 * Represents any being/structure/unit that can find its place in this game.
 */
class Entity : public QObject, public EffectProne
{
Q_OBJECT;

public:
	virtual BTech::EntityType getEntityType() const = 0;

	virtual bool isRotable() const;
	virtual bool isMovable() const;

	void setCurrentCoordinate(Coordinate coordinate);
	Coordinate getCurrentCoordinate() const;

	void setCurrentDirection(Direction direction);
	Direction getCurrentDirection() const;

	void setCurrentPosition(Coordinate coordinate, Direction direction);
	void setCurrentPosition(const Position &position);
	Position getCurrentPosition() const;

	friend QDataStream & operator << (QDataStream &out, const Entity &entity);
	friend QDataStream & operator >> (QDataStream &in, Entity &entity);

signals:
	void coordinateSet();
	void directionSet();

private:
	Position currentPosition;
};

/**
 * \class Rotable
 * Represents any Entity able to rotate.
 */
class Rotable : public Entity
{
Q_OBJECT;

public:
	Rotable() = default;

	bool isRotable() const;

	void setDirection(Direction direction);

	void setDestinationDirection(Direction direction);
	Direction getDestinationDirection() const;

	virtual void reachDestination();
	virtual bool isInMove() const;

	void setMoveObject(const MoveObject &moveObject);
	MoveObject getMoveObject() const;

	friend QDataStream & operator << (QDataStream &out, const Rotable &rotable);
	friend QDataStream & operator >> (QDataStream &in, Rotable &rotable);

signals:
	void directionChanged();

private:
	MoveObject moveObject;
	Direction destinationDirection;
};

/**
 * \class Movable
 * Represents any Entity able to move.
 */
class Movable : public Rotable
{
Q_OBJECT;

public:
	Movable() = default;

	bool isMovable() const;

	void setCoordinate(Coordinate coordinate);
	void setDestinationCoordinate(Coordinate coordinate);
	Coordinate getDestinationCoordinate() const;

	void setPosition(Coordinate coordinate, Direction direction);
	void setPosition(const Position &position);

	void setDestinationPosition(Coordinate coordinate, Direction direction);
	void setDestinationPosition(const Position &position);
	Position getDestinationPosition() const;

	virtual void reachDestination();
	virtual bool isInMove() const;

	friend QDataStream & operator << (QDataStream &out, const Movable &movable);
	friend QDataStream & operator >> (QDataStream &in, Movable &movable);

signals:
	void coordinateChanged();

private:
	Coordinate destinationCoordinate;
};

#endif // OBJECTS_H
