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

	int getAngle() const;

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

	static const QList <Direction> directions = {
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
 * \class Position
 * Represents a position that a unit can have (not necessarily Mech).
 */
class Position
{
public:
	Position();
	Position(int number, Direction direction);

	Position & operator = (const Position &obj);
	bool operator == (const Position &obj) const;
	bool operator != (const Position &obj) const;

	void setNumber(int number);
	int getNumber() const;
	void setDirection(Direction direction);
	Direction getDirection() const;

	void turnRight();
	void turnLeft();

	friend QDataStream & operator << (QDataStream &out, const Position &position);
	friend QDataStream & operator >> (QDataStream &in, Position &position);

private:
	int number;
	Direction direction;
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
 * \class PathFinder
 * Provides a functions findPath() and getPosition(). Grid derives after PathFinder.
 */
class PathFinder
{
public:
	virtual ~PathFinder() = 0;
	virtual QPoint getPosition(int number) const = 0;
};

/**
 * \class VisibilityManager
 * Provides info about lines of sight.
 */
class VisibilityManager
{
public:
	virtual ~VisibilityManager() = 0;
	virtual LineOfSight getLineOfSight(int src, int dest) const = 0;	/**< Returns the line of sight object for the hexes. */
	virtual Direction getAttackDirection(int src, int dest) const = 0;	/**< Returns the direction from which the target would be hit. */
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
