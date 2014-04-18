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

void Entity::setCurrentDirection(Direction direction)
{
	currentPosition.setDirection(direction);
	emit directionSet();
}

Direction Entity::getCurrentDirection() const
{
	return currentPosition.getDirection();
}

void Entity::setCurrentPositionNumber(int position)
{
	currentPosition.setNumber(position);
	emit positionNumberSet();
}

int Entity::getCurrentPositionNumber() const
{
	return currentPosition.getNumber();
}

void Entity::setCurrentPosition(const Position &position)
{
	currentPosition = position;
}

void Entity::setCurrentPosition(int position, Direction direction)
{
	setCurrentPositionNumber(position);
	setCurrentDirection(direction);
}

Position Entity::getCurrentPosition() const
{
	return currentPosition;
}

QDataStream & operator << (QDataStream &out, const Entity &entity)
{
	out << entity.currentPosition << *(const EffectProne *)(&entity);
	return out;
}

QDataStream & operator >> (QDataStream &in, Entity &entity)
{
	in >> entity.currentPosition >> *(EffectProne *)(&entity);
	return in;
}

/**
 * \class Rotable
 */

Rotable::Rotable()
{}

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

Movable::Movable()
{}

bool Movable::isMovable() const
{
	return true;
}

void Movable::setPositionNumber(int position)
{
	setCurrentPositionNumber(position);
	destinationPositionNumber = position;
}

void Movable::setPosition(const Position &position)
{
	setPositionNumber(position.getNumber());
	setDirection(position.getDirection());
}

void Movable::setPosition(int positionNumber, Direction direction)
{
	setPosition(Position(positionNumber, direction));
}

void Movable::setDestinationPositionNumber(int position)
{
	destinationPositionNumber = position;
	emit positionChanged();
}

int Movable::getDestinationPositionNumber() const
{
	return destinationPositionNumber;
}

void Movable::setDestinationPosition(const Position &position)
{
	setDestinationPosition(position.getNumber(), position.getDirection());
}

void Movable::setDestinationPosition(int position, Direction direction)
{
	setDestinationPositionNumber(position);
	setDestinationDirection(direction);
}

Position Movable::getDestinationPosition() const
{
	return Position(getDestinationPositionNumber(), getDestinationDirection());
}

void Movable::reachDestination()
{
	setCurrentPosition(getDestinationPosition());
}

bool Movable::isInMove() const
{
	return getCurrentPositionNumber() != getDestinationPositionNumber() || Rotable::isInMove();
}

QDataStream & operator << (QDataStream &out, const Movable &movable)
{
	out << static_cast<const Rotable &>(movable) << movable.destinationPositionNumber;
	return out;
}

QDataStream & operator >> (QDataStream &in, Movable &movable)
{
	in >> static_cast<Rotable &>(movable) >> movable.destinationPositionNumber;
	return in;
}
