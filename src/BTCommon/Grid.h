#ifndef GRID_H
#define GRID_H

#include <QtWidgets>
#include "BTCommon/GraphicsEntity.h"
#include "BTCommon/GraphicsFactory.h"
#include "BTCommon/GraphicsHex.h"
#include "BTCommon/MechEntity.h"
#include "BTCommon/Player.h"
#include "BTCommon/Position.h"
#include "BTCommon/Rules.h"

/**
 * \class Grid
 * Provides Map and GraphicsMap with Hex-managing functions. For this it requires reference to QVector of pointers to Hexes.
 */
class Grid : public PathFinder, public VisibilityManager
{

public:
	Grid(QVector <Hex *> &vector, int width, int height);

	void toggleGrid();
	void setGridVisible(bool visible);
	bool isGridVisible() const;
	void toggleCoordinates();
	void setCoordinatesVisible(bool visible);
	bool areCoordinatesVisible() const;

	void showWalkRange(const MovementObject &movement);
	void hideWalkRange();
	void showShootRange(const MechEntity *mech);
	void hideShootRange();
	void hideAll();

	void clearHexes();

	void drawWalkRange(const MovementObject &movement);
	void drawShootRange(const MechEntity *mech);
	void drawFriendlyMechs(const Player *player);

	QPoint getPosition(int number) const;

	LineOfSight getLineOfSight(const Hex *src, const Hex *dest) const;
	LineOfSight getLineOfSight(int src, int dest) const;
	bool lineOfSightExists(int src, int dest) const;
	Direction getAttackDirection(int src, int dest) const;
	Direction getAttackDirection(Direction unitDirection, Direction attackDirection) const;

private:
	void initTemp();
	void countPoints(int width, int height, int hexS = GraphicsHex::getSize());
	void initHex(Hex *hex);

	int width;	/**< Number of Hexes in the row. */
	int height;	/**< Number of Hexes in the column. */

	QVector <Hex *> &hexes;

	bool walkRangeVisible;
	bool shootRangeVisible;

	mutable MechEntity *activatedMech;

	/**
	 * \class TState
	 * This class is a container storing states in Hex searching.
	 */
	class TState {
	public:
		TState(Position position = Position(), Position father = Position(), int movePoints = 0, int distance = 0);

		Position position;
		Position father;
		int movePoints;
		int distance;
	};

	AttackObject getAttackObject(const MechEntity *attacker, const MechEntity *target) const;

	QList <MoveObject> getWalkRange(const MovementObject &movementObject) const;
	QList <int> getShootRange(int src, Direction direction) const;

	int nextHex(int hex, Direction direction) const;
	QList <QPair <const Hex *, const Hex *> > hexesBetween(const Hex *src, const Hex *dest) const;
	int getHexDistance(int src, int dest) const;

	LineOfSight visibilityScore(const Hex *hex, const Hex *src, const Hex *dest) const;
	LineOfSight pairVisibilityScore(QPair <const Hex *, const Hex *> hexes, const Hex *src, const Hex *dest) const;
};

#endif // GRID_H
