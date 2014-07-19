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

#include <QtGui>
#include "BTCommon/Position.h"

/**
 * \class Direction
 */

Direction::Direction(int number)
{
	value = number % NUMBER;
	if (value < 0)
		value += NUMBER;
}

Direction Direction::operator + (int change) const
{
	return Direction(value + change);
}

Direction Direction::operator + (Direction direction) const
{
	return this->operator + (direction.value);
}

void Direction::operator += (int change)
{
	value = (value + change) % NUMBER;
	if (value < 0)
		value += change;
}

void Direction::operator ++ ()
{
	return (*this) += 1;
}

void Direction::operator -- ()
{
	return (*this) += 5;
}

bool Direction::operator == (Direction d) const
{
	return value == d.value;
}

bool Direction::operator != (Direction d) const
{
	return value != d.value;
}

void Direction::operator = (Direction d)
{
	value = d.value;
}

bool Direction::operator <= (Direction d) const
{
	return value <= d.value;
}

Direction::operator int() const
{
	return value;
}

Direction::operator QString() const
{
	return BTech::directionStringChange[*this];
}

int Direction::toAngle() const
{
	return value * BTech::Math::HEX_ANGLE;
}

Direction Direction::onRight() const
{
	return (*this) + 1;
}

Direction Direction::onLeft() const
{
	return (*this) + 5;
}

Direction Direction::behind() const
{
	return (*this) + 3;
}

QDataStream & operator << (QDataStream &out, const Direction &direction)
{
	out << direction.value;
	return out;
}

QDataStream & operator >> (QDataStream &in, Direction &direction)
{
	in >> direction.value;
	return in;
}

uint qHash(const Direction &direction)
{
	return qHash(static_cast<uint>(direction.value));
}

const BiHash <Direction, QString> BTech::directionStringChange {
	{DirectionN,  Strings::DirectionN},
	{DirectionNE, Strings::DirectionNE},
	{DirectionSE, Strings::DirectionSE},
	{DirectionS,  Strings::DirectionS},
	{DirectionSW, Strings::DirectionSW},
	{DirectionNW, Strings::DirectionNW},
};

const BiHash <Direction, QString> BTech::directionSideStringChange {
	{DirectionFront, Strings::DirectionFront},
	{DirectionRightFront, Strings::DirectionRightFront},
	{DirectionRightRear, Strings::DirectionRightRear},
	{DirectionRear, Strings::DirectionRear},
};

/**
 * \namespace BTech::Terrain
 */

const BiHash <BTech::Terrain, QString> BTech::terrainStringChange {
	{Terrain::Clear,      Strings::TerrainClear},
	{Terrain::LightWoods, Strings::TerrainLightWoods},
	{Terrain::HeavyWoods, Strings::TerrainHeavyWoods},
	{Terrain::Rough,      Strings::TerrainRough},
	{Terrain::Water,      Strings::TerrainWater},
};

const QHash <BTech::Terrain, QColor> BTech::terrainColorMap {
	{ Terrain::Clear,      BTech::Colors::PeachPuff},
	{ Terrain::LightWoods, BTech::Colors::CobaltGreen},
	{ Terrain::HeavyWoods, BTech::Colors::DarkCyan},
	{ Terrain::Rough,      BTech::Colors::LightYellow},
	{ Terrain::Water,      BTech::Colors::MediumTurquoise},
};

const QHash <BTech::Terrain, int> BTech::travelPenalty {
	{Terrain::Clear,      BTech::TravelPenalty::Clear},
	{Terrain::LightWoods, BTech::TravelPenalty::LightWoods},
	{Terrain::HeavyWoods, BTech::TravelPenalty::HeavyWoods},
	{Terrain::Rough,      BTech::TravelPenalty::Rough},
	{Terrain::Water,      BTech::TravelPenalty::Water},
};

QDataStream & BTech::operator << (QDataStream &out, const BTech::Terrain &terrain)
{
	out << toUnderlying(terrain);
	return out;
}

QDataStream & BTech::operator >> (QDataStream &in, BTech::Terrain &terrain)
{
	in >> toUnderlyingRef(terrain);
	return in;
}

/**
 * \class Position
 */

/* constructor */
Position::Position(Coordinate coordinate, Direction direction)
	: coordinate(coordinate), direction(direction)
{}

bool Position::operator == (const Position &obj) const
{
	return	obj.getCoordinate() == getCoordinate() &&
		obj.getDirection() == getDirection();
}

bool Position::operator != (const Position &obj) const
{
	return !((*this) == obj);
}

void Position::setCoordinate(Coordinate coordinate)
{
	this->coordinate = coordinate;
}

Coordinate Position::getCoordinate() const
{
	return coordinate;
}

void Position::setDirection(Direction direction)
{
	this->direction = direction;
}

Direction Position::getDirection() const
{
	return direction;
}

void Position::turnRight()
{
	++direction;
}

void Position::turnLeft()
{
	direction += 5;
}

QDataStream & operator << (QDataStream &out, const Position &position)
{
	out << position.coordinate << position.direction;
	return out;
}

QDataStream & operator >> (QDataStream &in, Position &position)
{
	in >> position.coordinate >> position.direction;
	return in;
}

/**
 * \class LineOfSight
 */

LineOfSight::LineOfSight()
{
	lightWoods = 0;
	heavyWoods = 0;

	srcTerrain = BTech::Terrain::Clear;
	destTerrain = BTech::Terrain::Clear;

	srcHeight = 0;
	destHeight = 0;

	heightBetween = 0;
	heightBarrier = false;
}

void LineOfSight::operator += (const LineOfSight &line)
{
	lightWoods += line.lightWoods;
	heavyWoods += line.heavyWoods;

	heightBetween = qMax(heightBetween, line.heightBetween);
}

LineOfSight qMax(const LineOfSight &lhs, const LineOfSight &rhs)
{
	LineOfSight result;

	result.lightWoods = qMax(lhs.lightWoods, rhs.lightWoods);
	result.heavyWoods = qMax(lhs.heavyWoods, rhs.heavyWoods);

	result.srcTerrain  = lhs.srcTerrain;
	result.destTerrain = lhs.destTerrain;

	result.heightBetween = qMax(lhs.heightBetween, rhs.heightBetween);
	result.heightBarrier = lhs.heightBarrier;

	return result;
}

/**
 * \class MechPosition
 */

MechPosition::~MechPosition()
{}
