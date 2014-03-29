#include "BTCommon/EnumHashFunctions.h"

#include "BTCommon/Position.h"
#include <QtGui>

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
	return direction + value;
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

int Direction::getAngle() const
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
	{ Terrain::Clear,      BTech::Colors::PeachPuff       },
	{ Terrain::LightWoods, BTech::Colors::CobaltGreen     },
	{ Terrain::HeavyWoods, BTech::Colors::DarkCyan        },
	{ Terrain::Rough,      BTech::Colors::LightYellow     },
	{ Terrain::Water,      BTech::Colors::MediumTurquoise },
};

const QHash <BTech::Terrain, int> BTech::travelPenalty {
	{ Terrain::Clear,      BTech::TravelPenalty::Clear      },
	{ Terrain::LightWoods, BTech::TravelPenalty::LightWoods },
	{ Terrain::HeavyWoods, BTech::TravelPenalty::HeavyWoods },
	{ Terrain::Rough,      BTech::TravelPenalty::Rough      },
	{ Terrain::Water,      BTech::TravelPenalty::Water      },
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
Position::Position()
{}

/* constructor */
Position::Position(int number, Direction direction)
	: number(number), direction(direction)
{}

Position & Position::operator = (const Position &obj)
{
	setDirection(obj.getDirection());
	setNumber(obj.getNumber());
	return *this;
}

bool Position::operator == (const Position &obj) const
{
	return	obj.getNumber() == getNumber() &&
		obj.getDirection() == getDirection();
}

bool Position::operator != (const Position &obj) const
{
	return !((*this) == obj);
}

void Position::setNumber(int number)
{
	this->number = number;
}

int Position::getNumber() const
{
	return number;
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
	out << position.number << position.direction;
	return out;
}

QDataStream & operator >> (QDataStream &in, Position &position)
{
	in >> position.number >> position.direction;
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

	result.srcTerrain = lhs.srcTerrain;
	result.destTerrain = lhs.destTerrain;

	result.heightBetween = qMax(lhs.heightBetween, rhs.heightBetween);
	result.heightBarrier = lhs.heightBarrier;

	return result;
}

/**
 * \class PathFinder
 */

PathFinder::~PathFinder()
{}

/**
 * \class VisibilityManager
 */

VisibilityManager::~VisibilityManager()
{}

/**
 * \class MechPosition
 */

MechPosition::~MechPosition()
{}
