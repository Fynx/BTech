#include <QtTest>

class Grid;
class Hex;

class WalkRangeTest : public QObject
{
	Q_OBJECT;

private slots:
	void initTestCase();
	void cleanupTestCase();

	void cleanup();

	void simpleRotationTest();

private:
	QVector <Hex *> hexes;
	Grid *grid;

};
