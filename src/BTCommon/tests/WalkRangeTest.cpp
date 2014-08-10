#include <initializer_list>
#include "BTCommon/Grid.h"
#include "BTCommon/Hex.h"
#include "BTCommon/tests/WalkRangeTest.h"

struct HexDescriptor {
	BTech::Terrain terrain;
	Coordinate coord;
	int height;
	int depth;
};

static const int MovePointsInfinite = 99;

inline bool operator == (const MoveObject &a, const MoveObject &b)
{
	return a.getSrc() == b.getSrc()
		&& a.getAction() == b.getAction()
		&& a.getDest() == b.getDest()
		&& a.getMovePoints() == b.getMovePoints()
		&& a.getDistance() == b.getDistance()
		&& a.getPath() == b.getPath();
}

void WalkRangeTest::initTestCase()
{
	grid = new Grid;
	grid->initGrid(hexes);
}

void WalkRangeTest::cleanupTestCase()
{
	delete grid;
}

void WalkRangeTest::cleanup()
{
	qDeleteAll(hexes);
	hexes.clear();
	grid->initGrid(hexes);
}

void WalkRangeTest::simpleRotationTest()
{
	for (const auto &hex : std::initializer_list <HexDescriptor> {
			{BTech::Terrain::Clear, {1, 2}, 0, 0},
			{BTech::Terrain::Clear, {1, 3}, 0, 0},
			{BTech::Terrain::Clear, {2, 1}, 0, 0},
			{BTech::Terrain::Clear, {2, 2}, 0, 0},
			{BTech::Terrain::Clear, {2, 3}, 0, 0},
			{BTech::Terrain::Clear, {3, 2}, 0, 0},
			{BTech::Terrain::Clear, {3, 3}, 0, 0}

		}) {
			Hex *newHex = new Hex;
			newHex->setTerrain(hex.terrain);
			newHex->setCoordinate(hex.coord);
			newHex->setHeight(hex.height);
			hexes.append(newHex);
		}

	grid->initGrid(hexes);

	const Coordinate originCoord{2, 2};
	const Position origin{originCoord, BTech::DirectionN};
	const Position destination{originCoord, BTech::DirectionSE};
	const MovementObject originObject{origin, MovePointsInfinite, BTech::MovementAction::Walk};
	const QList <Position> expectedPath {
		{originCoord, BTech::DirectionNE},
		{originCoord, BTech::DirectionSE},
	};
	const MoveObject destinationObject{originObject, destination, 2, 0, expectedPath};

	const QList <MoveObject> possibleMoves = grid->getWalkRange(originObject);

	QVERIFY(possibleMoves.contains(destinationObject));
}

QTEST_MAIN(WalkRangeTest)
