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

#include "BTCommon/EnumHashFunctions.h"
#include "BTCommon/Hex.h"
#include "BTCommon/MechEntity.h"

/* constructor */
Hex::Hex()
	: mech(nullptr)	// this is done in clearData as well, but...
{
	initNeighbours();
	clearData();
}

/* destructor */
Hex::~Hex()
{}

void Hex::setCoordinate(const Coordinate &coordinate)
{
	this->coordinate = coordinate;
}

Coordinate Hex::getCoordinate() const
{
	return coordinate;
}

void Hex::setNeighbour(Direction direction, Hex *hex)
{
	neighbour[direction] = hex;
}

Hex * Hex::getNeighbour(Direction direction) const
{
	return neighbour[direction];
}

void Hex::initNeighbours()
{
	for (int i = 0; i < Direction::NUMBER; ++i)
		neighbour[i] = nullptr;
}

void Hex::setHeight(int height)
{
	this->height = height;
}

int Hex::getHeight() const
{
	return height;
}

void Hex::setDepth(int depth)
{
	this->depth = depth;
}

int Hex::getDepth() const
{
	return depth;
}

void Hex::setTerrain(BTech::Terrain terrain)
{
	this->terrain = terrain;
}

BTech::Terrain Hex::getTerrain() const
{
	return terrain;
}

int Hex::travelPenalty() const
{
	return BTech::travelPenalty[getTerrain()] + getDepth();
}

void Hex::setMech(MechEntity *mech)
{
	this->mech = mech;
	getMech()->setMechPosition(this);
}

MechEntity * Hex::getMech() const
{
	return mech;
}

void Hex::removeMech()
{
	mech = nullptr;
}

void Hex::setMoveObject(const MoveObject &move)
{
	moveObject[move.getDest().getDirection()] = move;
}

void Hex::setMoveObject(int areaNumber)
{
	currentMovementObjectNumber = areaNumber;
}

MoveObject Hex::getMoveObject(Direction direction) const
{
	return moveObject[direction];
}

MoveObject Hex::getMoveObject() const
{
	return moveObject[currentMovementObjectNumber];
}

bool Hex::hasMoveObject() const
{
	for (Direction direction : BTech::directions)
		if (moveObject[direction].getActionType() != BTech::ActionType::Idle)
			return true;
	return false;
}

void Hex::removeMoveObject()
{
	for (Direction direction : BTech::directions)
		moveObject[direction] = MoveObject(BTech::ActionType::Idle);
	currentMovementObjectNumber = 0;
}

void Hex::setAttackObject(const AttackObject &attack)
{
	attackable = true;
	if (getMech() == nullptr)
		return;
	getMech()->setAttackObject(attack);
}

AttackObject Hex::getAttackObject() const
{
	return getMech()->getAttackObject();
}

bool Hex::hasAttackObject() const
{
	return getMech() != nullptr && getMech()->hasAttackObject();
}

void Hex::removeAttackObject()
{
	attackable = false;
	if (getMech() != nullptr)
		getMech()->removeAttackObject();
}

bool Hex::isAttackable() const
{
	return attackable;
}

void Hex::clear()
{
	removeMoveObject();
	removeAttackObject();
}

QDataStream & operator << (QDataStream &out, const Hex &hex)
{
	out << hex.coordinate
	    << hex.height
	    << hex.terrain;
	return out;
}

QDataStream & operator >> (QDataStream &in, Hex &hex)
{
	in >> hex.coordinate
	   >> hex.height
	   >> hex.terrain;
	return in;
}

void Hex::clearData()
{
	clear();
	removeMech();
	setTerrain(BTech::Terrain::Clear);
	setDepth(0);
	setHeight(0);
}
