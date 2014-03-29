#include "BTCommon/EnumHashFunctions.h"
#include "BTCommon/Hex.h"

const VisibilityManager *Hex::visibilityManager = nullptr;

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

/* static member */
void Hex::setVisibilityManager(const VisibilityManager *manager)
{
	visibilityManager = manager;
}

void Hex::setNeighbour(Direction direction, Hex * hex)
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

void Hex::setNumber(int number)
{
	this->number = number;
}

int Hex::getNumber() const
{
	return number;
}

void Hex::setPoint(const QPoint &point)
{
	this->point = point;
}

QPoint Hex::getPoint() const
{
	return point;
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
		if (moveObject[direction].getAction() != BTech::MovementAction::Idle)
			return true;
	return false;
}

void Hex::removeMoveObject()
{
	for (Direction direction : BTech::directions)
		moveObject[direction] = MoveObject(BTech::MovementAction::Idle);
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
	out << hex.number
	    << hex.point
	    << hex.height
	    << hex.terrain;
	return out;
}

QDataStream & operator >> (QDataStream &in, Hex &hex)
{
	in >> hex.number
	   >> hex.point
	   >> hex.height
	   >> hex.terrain;
	return in;
}

void Hex::clearData()
{
	clear();
	removeMech();
	setTerrain(BTech::Terrain::Clear);
}
