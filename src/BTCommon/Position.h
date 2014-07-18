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

#ifndef POSITION_H
#define POSITION_H

#include "BTCommon/Utils.h"

/**
 * \class Direction
 * Provides a flexible tool for direction states in the hex-constructed world.
 * It automatically converts strings or characters denoting given direction. It also allowes easy iterating through directions.
 */
class Direction
{
public:
	Direction(int number = 0);

	Direction operator + (int) const;
	Direction operator + (Direction direction) const;
	void operator += (int);
	void operator ++ ();
	void operator -- ();
	bool operator == (Direction) const;
	bool operator != (Direction) const;
	void operator = (Direction);
	bool operator <= (Direction) const;
	operator int() const;
	explicit operator QString() const;

	int toAngle() const;

	Direction onRight() const;
	Direction onLeft() const;
	Direction behind() const;

	static const int NUMBER = 6;

	friend QDataStream & operator << (QDataStream &out, const Direction &direction);
	friend QDataStream & operator >> (QDataStream &in, Direction &direction);

	friend uint qHash(const Direction &direction);

private:
	qint8 value;
};

namespace BTech {
	static const Direction DirectionN(0);
	static const Direction DirectionNE(1);
	static const Direction DirectionSE(2);
	static const Direction DirectionS(3);
	static const Direction DirectionSW(4);
	static const Direction DirectionNW(5);

	static const Direction DirectionFront(0);
	static const Direction DirectionRightFront(1);
	static const Direction DirectionRightRear(2);
	static const Direction DirectionRear(3);
	static const Direction DirectionLeftRear(4);
	static const Direction DirectionLeftFront(5);

	static const QVector <Direction> directions = {
		DirectionN,
		DirectionNE,
		DirectionSE,
		DirectionS,
		DirectionSW,
		DirectionNW
	};

	extern const BiHash <Direction, QString> directionStringChange;
	extern const BiHash <Direction, QString> directionSideStringChange;
}

/**
 * \enum BTech::Terrain
 */
namespace BTech {

	enum class Terrain : quint8 {
		Clear = 1,
		LightWoods = 2,
		HeavyWoods = 3,
		Rough = 4,
		Water = 5
	};

	static const Terrain terrainTypes[] = {Terrain::Clear, Terrain::LightWoods, Terrain::HeavyWoods, Terrain::Rough, Terrain::Water};

	namespace TravelPenalty {
		static const int Clear      = 1;
		static const int LightWoods = 2;
		static const int HeavyWoods = 3;
		static const int Rough      = 2;
		static const int Water      = 1;
	};

	extern const BiHash <Terrain, QString> terrainStringChange;
	extern const QHash <Terrain, QColor> terrainColorMap;
	extern const QHash <Terrain, int> travelPenalty;

	QDataStream & operator << (QDataStream &out, const BTech::Terrain &terrain);
	QDataStream & operator >> (QDataStream &in, BTech::Terrain &terrain);
}

/**
 * \class Coordinate
 * A thin abstraction over QPoint for representing coordinates of Hexes
 */

class Coordinate : public QPoint
{
	friend uint qHash(const Coordinate &c, uint seed);
public:
	constexpr Coordinate(QPoint p = QPoint{0, 0}) : QPoint(p) {}
	constexpr Coordinate(int x, int y) : Coordinate(QPoint(x, y)) {}

	QString toString() const {return QString("[%1, %2]").arg(y()).arg(x());}
};

inline uint qHash(const Coordinate &c, uint seed)
{
	return qHash(QPair <int, int>(c.x(), c.y()), seed);
}

template <typename T, typename U>
inline const Coordinate operator + (const T &c1, const U &c2)
{
	return Coordinate(QPoint(c1.x() + c2.x(), c1.y() + c2.y()));
}

/**
 * \class Position
 * Represents a position that a unit can have (not necessarily Mech).
 */
class Position
{
	friend uint qHash(const Position &p, uint seed);
public:
	Position() = default;
	Position(Coordinate position, Direction direction);
	Position(const Position &) = default;
	Position & operator = (const Position &) = default;

	bool operator == (const Position &obj) const;
	bool operator != (const Position &obj) const;

	void setCoordinate(Coordinate coordinate);
	Coordinate getCoordinate() const;
	void setDirection(Direction direction);
	Direction getDirection() const;

	void turnRight();
	void turnLeft();

	friend QDataStream & operator << (QDataStream &out, const Position &position);
	friend QDataStream & operator >> (QDataStream &in, Position &position);

private:
	Coordinate coordinate;
	Direction direction;
};

inline uint qHash(const Position &p, uint seed = 0)
{
	return qHash(QPair <uint, uint>(qHash(p.coordinate, seed), qHash(p.direction, seed)));
}

/**
 * \class CoordinateMapper
 * Maps Hex coordinates into its corresponding GraphicsHex position on the scene
 */

class CoordinateMapper
{
public:
	virtual QPointF mapCoordinateToScene(const Coordinate &c) const = 0;
};

/**
 * \class LineOfSight
 * LineOfSight is a container for the data about line of sight between two hexes.
 */
class LineOfSight
{
public:
	LineOfSight();

	int lightWoods;
	int heavyWoods;

	BTech::Terrain srcTerrain;
	BTech::Terrain destTerrain;

	int srcHeight;
	int destHeight;

	int heightBetween;
	bool heightBarrier;

	void operator += (const LineOfSight &lineOfSight);
	friend LineOfSight qMax(const LineOfSight &lhs, const LineOfSight &rhs);
};

/**
 * \class MechPosition
 * Provides functions that allow Units know something about terrain on which they stay.
 */
class MechPosition
{
public:
	virtual ~MechPosition() = 0;
	virtual BTech::Terrain getTerrain() const = 0;
	virtual void removeMech() = 0;
};

#endif // POSITION_H
