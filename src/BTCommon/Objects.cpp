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

#include "BTCommon/Objects.h"

/**
 * \class Entity
 */

bool Entity::isRotable() const
{
	return false;
}

bool Entity::isMovable() const
{
	return false;
}

void Entity::setCurrentCoordinate(Coordinate coordinate)
{
	currentPosition.setCoordinate(coordinate);
	emit coordinateSet();
}

Coordinate Entity::getCurrentCoordinate() const
{
	return currentPosition.getCoordinate();
}

void Entity::setCurrentDirection(Direction direction)
{
	currentPosition.setDirection(direction);
	emit directionSet();
}

Direction Entity::getCurrentDirection() const
{
	return currentPosition.getDirection();
}

void Entity::setCurrentPosition(Coordinate coordinate, Direction direction)
{
	setCurrentCoordinate(coordinate);
	setCurrentDirection(direction);
}

void Entity::setCurrentPosition(const Position &position)
{
	setCurrentCoordinate(position.getCoordinate());
	setCurrentDirection(position.getDirection());
}

Position Entity::getCurrentPosition() const
{
	return currentPosition;
}

QDataStream & operator << (QDataStream &out, const Entity &entity)
{
	qDebug() << "\t\tposition:        " << entity.getCurrentCoordinate();
	qDebug() << "\t\tdirection:       " << BTech::directionStringChange[entity.getCurrentDirection()];
	out << entity.currentPosition;
	operator << (out, static_cast<const EffectProne &>(entity));
	return out;
}

QDataStream & operator >> (QDataStream &in, Entity &entity)
{
	in >> entity.currentPosition;
	qDebug() << "\t\tposition:        " << entity.getCurrentCoordinate();
	qDebug() << "\t\tdirection:       " << BTech::directionStringChange[entity.getCurrentDirection()];
	operator >> (in, static_cast<EffectProne &>(entity));
	return in;
}

/**
 * \class Rotable
 */

bool Rotable::isRotable() const
{
	return true;
}

void Rotable::setDirection(Direction direction)
{
	setCurrentDirection(direction);
	destinationDirection = direction;
}

void Rotable::setDestinationDirection(Direction direction)
{
	destinationDirection = direction;
	emit directionChanged();
}

Direction Rotable::getDestinationDirection() const
{
	return destinationDirection;
}

void Rotable::reachDestination()
{
	setCurrentDirection(getDestinationDirection());
}

bool Rotable::isInMove() const
{
	return getCurrentDirection() != getDestinationDirection();
}

void Rotable::setMoveObject(const MoveObject &moveObject)
{
	this->moveObject = moveObject;
}

MoveObject Rotable::getMoveObject() const
{
	return moveObject;
}

QDataStream & operator << (QDataStream &out, const Rotable &rotable)
{
	out << static_cast<const Entity &>(rotable) << rotable.destinationDirection;
	return out;
}

QDataStream & operator >> (QDataStream &in, Rotable &rotable)
{
	in >> static_cast<Entity &>(rotable) >> rotable.destinationDirection;
	return in;
}

/**
 * \class Movable
 */

bool Movable::isMovable() const
{
	return true;
}

void Movable::setCoordinate(Coordinate coordinate)
{
	setCurrentCoordinate(coordinate);
	destinationCoordinate = coordinate;
}

void Movable::setDestinationCoordinate(Coordinate coordinate)
{
	destinationCoordinate = coordinate;
	emit coordinateChanged();
}

void Movable::setPosition(Coordinate coordinate, Direction direction)
{
	setPosition(Position(coordinate, direction));
}

void Movable::setPosition(const Position &position)
{
	setCoordinate(position.getCoordinate());
	setDirection(position.getDirection());
}

Coordinate Movable::getDestinationCoordinate() const
{
	return destinationCoordinate;
}

void Movable::setDestinationPosition(const Position &position)
{
	setDestinationPosition(position.getCoordinate(), position.getDirection());
}

void Movable::setDestinationPosition(Coordinate coordinate, Direction direction)
{
	setDestinationCoordinate(coordinate);
	setDestinationDirection(direction);
}

Position Movable::getDestinationPosition() const
{
	return Position(getDestinationCoordinate(), getDestinationDirection());
}

void Movable::reachDestination()
{
	setCurrentPosition(getDestinationPosition());
}

bool Movable::isInMove() const
{
	return getCurrentPosition().getCoordinate() != getDestinationCoordinate() || Rotable::isInMove();
}

QDataStream & operator << (QDataStream &out, const Movable &movable)
{
	out << static_cast<const Rotable &>(movable) << movable.destinationCoordinate;
	return out;
}

QDataStream & operator >> (QDataStream &in, Movable &movable)
{
	in >> static_cast<Rotable &>(movable) >> movable.destinationCoordinate;
	return in;
}
