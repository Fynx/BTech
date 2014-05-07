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

#include "BTCommon/Grid.h"
#include <limits>

Grid::Grid(QVector <Hex *> &vector, int width, int height)
	: width(width), height(height), hexes(vector)
{
	walkRangeVisible = false;
	shootRangeVisible = false;

	initGrid(width, height);
	GraphicsEntity::setPathFinder(this);
}

void Grid::toggleGrid()
{
	GraphicsHex::setGridVisible(!isGridVisible());
}

void Grid::setGridVisible(bool visible)
{
	GraphicsHex::setGridVisible(visible);
	if (!isGridVisible())
		GraphicsHex::setCoordinatesVisible(false);
}

bool Grid::isGridVisible() const
{
	return GraphicsHex::isGridVisible();
}

void Grid::toggleCoordinates()
{
	GraphicsHex::setCoordinatesVisible(!areCoordinatesVisible() && isGridVisible());
}

bool Grid::areCoordinatesVisible() const
{
	return GraphicsHex::areCoordinatesVisible();
}

QPoint Grid::getPosition(int number) const
{
	return GraphicsFactory::get(hexes[number])->pos().toPoint();
}

LineOfSight Grid::getLineOfSight(const Hex *src, const Hex *dest) const
{
	QList <QPair <const Hex *, const Hex *> > path = hexesBetween(src, dest);
	LineOfSight line;

	line.srcHeight = path.front().first->getHeight();
	line.destHeight = path.back().first->getHeight();

	for (QPair <const Hex *, const Hex *> pair : path)
		line += pairVisibilityScore(pair, src, dest);
	if (path.size() > 2) {
		if (line.srcHeight > line.destHeight)
			line.heightBarrier = pairVisibilityScore(path[path.size() - 2], src, dest).heightBetween == line.srcHeight;
		else if (line.srcHeight < line.destHeight)
			line.heightBarrier = pairVisibilityScore(path[1], src, dest).heightBetween == line.destHeight;
	}

	return line;
}

LineOfSight Grid::getLineOfSight(int src, int dest) const
{
	return getLineOfSight(hexes[src], hexes[dest]);
}

bool Grid::lineOfSightExists(int src, int dest) const
{
	return AttackObject::getTerrainModifier(BTech::ModifierType::Attack, getLineOfSight(src, dest)) != BTech::INF_ATTACK_MODIFIER;
}

Direction Grid::getAttackDirection(int src, int dest) const
{
	if (src == dest)
		return BTech::DirectionN;
	QList <QPair <const Hex *, const Hex *> > path = hexesBetween(hexes[src], hexes[dest]);
	const Hex *last = path[path.size() - 2].first;

	for (Direction direction : BTech::directions)
		if (last->getNumber() == nextHex(dest, direction))
			return direction.behind();
	return BTech::DirectionN;
}

Direction Grid::getAttackDirection(Direction unitDirection, Direction attackDirection) const
{
	Direction result = BTech::DirectionRear;

	while (attackDirection != unitDirection) {
		++attackDirection;
		--result;
	}

	return result;
}

void Grid::initGrid(int width, int height)
{
	int leftBorder  = GraphicsHex::getSize();
	int upperBorder = -GraphicsHex::getSize();

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j)
			GraphicsFactory::get(hexes[i * width + j])->setPos({
				qreal(leftBorder + j * GraphicsHex::getSize() * 3 / 2),
				qreal(upperBorder + (i * 2 + (j % 2 == 0)) * GraphicsHex::getSize())});
	}
	for (Hex *hex : hexes)
		initHex(hex);
}

void Grid::initHex(Hex *hex)
{
	int number = hex->getNumber();

	bool even = (number % width % 2 == 0);

	if (number >= width) {
		hex->setNeighbour(BTech::DirectionN, hexes[number - width]);
		if (!even) {
			if (number % width > 0)
				hex->setNeighbour(BTech::DirectionNW, hexes[number - width - 1]);
			if (number % width < width - 1)
				hex->setNeighbour(BTech::DirectionNE, hexes[number - width + 1]);
		}
	}
	if (number + width < hexes.size()) {
		hex->setNeighbour(BTech::DirectionS, hexes[number + width]);
		if (even) {
			if (number % width > 0)
				hex->setNeighbour(BTech::DirectionSW, hexes[number + width - 1]);
			if (number % width < width - 1)
				hex->setNeighbour(BTech::DirectionSE, hexes[number + width + 1]);
		}
	}
	if (!even) {
		if (number % width > 0)
			hex->setNeighbour(BTech::DirectionSW, hexes[number - 1]);
		if (number % width < width - 1)
			hex->setNeighbour(BTech::DirectionSE, hexes[number + 1]);
	} else {
		if (number % width > 0)
			hex->setNeighbour(BTech::DirectionNW, hexes[number - 1]);
		if (number % width < width - 1)
			hex->setNeighbour(BTech::DirectionNE, hexes[number + 1]);
	}
}

int Grid::nextHex(int pointNum, Direction direction) const
{
	if (pointNum < 0)
		return -1;
	Hex *dest = hexes[pointNum]->getNeighbour(direction);
	if (dest == nullptr)
		return -1;
	else
		return dest->getNumber();
}

static bool onTheBorder(const Hex *src, const Hex *dest)
{
	int sx = src->getPoint().x();
	int sy = src->getPoint().y();
	int dx = dest->getPoint().x();
	int dy = dest->getPoint().y();

	if (sy > dy)
		return onTheBorder(dest, src);

	int xDifference = qAbs(sx - dx);
	int yDifference = dy - sy;	/* always positive */
	bool xDiffersEven = (xDifference % 2 == 0);
	//bool change = xDiffersEven == (sx % 2 == 0);

	return (yDifference == 0 && xDiffersEven);// || (xDifference * 3 / 2 + (int)(change) == yDifference);	TODO
}

QList <QPair <const Hex *, const Hex *> > Grid::hexesBetween(const Hex *src, const Hex *dest) const
{
	QList <QPair <const Hex *, const Hex *> > result;
	const Hex *cur = src;

	BTech::Matrix auxCur, auxPrev;
	int i;
	result.append(QPair <const Hex *, const Hex *>(cur, nullptr));
	while (cur != dest) {
		i = 0;
		auxCur = BTech::Matrix();
		do {
			++i;
			auxPrev = auxCur;
			auxCur = BTech::Matrix(GraphicsFactory::get(dest)->pos().x() - GraphicsFactory::get(cur)->pos().x(),
			                       GraphicsFactory::get(dest)->pos().y() - GraphicsFactory::get(cur)->pos().y(),
			                       GraphicsFactory::get(cur)->getAbsNode(i).x() - GraphicsFactory::get(cur)->pos().x(),
			                       GraphicsFactory::get(cur)->getAbsNode(i).y() - GraphicsFactory::get(cur)->pos().y());
		} while (i < 2 || auxPrev.det() < 0 || auxCur.det() >= 0);

		if (!onTheBorder(cur, dest)) {
			cur = cur->getNeighbour(i);
			result.append({cur, nullptr});
		} else {
			if (cur->getNeighbour(i) != nullptr) {
				result.append({cur->getNeighbour(i), cur->getNeighbour(i - 1)});
				cur = cur->getNeighbour(i - 1);
			} else {
				result.append({cur->getNeighbour(i - 1), nullptr});
				cur = cur->getNeighbour(i - 1);
			}
		}
	}

	return result;
}

int Grid::getHexDistance(int src, int dest) const
{
	return hexesBetween(hexes[src], hexes[dest]).size() - 1;
}

void Grid::showWalkRange(const MovementObject &movement)
{
	hideWalkRange();
	drawWalkRange(movement);
	walkRangeVisible = true;
}

void Grid::hideWalkRange()
{
	if (!walkRangeVisible)
		return;
	walkRangeVisible = false;
	for (Hex *hex : hexes)
		hex->removeMoveObject();
}

void Grid::showShootRange(const MechEntity *mech)
{
	if (mech == nullptr)
		return;

	drawShootRange(mech);
	shootRangeVisible = true;
}

void Grid::hideShootRange()
{
	shootRangeVisible = false;
	for (Hex *hex : hexes)
		hex->removeAttackObject();
}

void Grid::hideAll()
{
	hideWalkRange();
	hideShootRange();
	for (Hex *hex : hexes)
		hex->clear();
}

void Grid::clearHexes()
{
	for (Hex *hex : hexes)
		GraphicsFactory::get(hex)->setClicked(false);
}

QList <int> Grid::getShootRange(int src, Direction direction) const
{
	if (src < 0)
		return QList <int>();

	bool visited[hexes.size()];
	for (int i = 0; i < hexes.size(); ++i)
		visited[i] = false;
	visited[src] = true;

	int cur, next;

	cur = nextHex(src, direction.onRight());
	Direction curDir = direction.onRight();
	for (int i = 0; i < 2; ++i) {
		cur = src;
		while (cur != -1) {
			visited[cur] = true;
			cur = nextHex(cur, curDir);
		}
		curDir = direction.onLeft();
	}

	cur = nextHex(src, direction);
	QQueue <int> queue;
	if (cur != -1) {
		queue.enqueue(cur);
		visited[cur] = true;
	}

	while (!queue.empty()) {
		cur = queue.dequeue();
		for (Direction curDirection : BTech::directions) {
			next = nextHex(cur, curDirection);
			if (next >= 0 && !visited[next]) {
				queue.enqueue(next);
				visited[next] = true;
			}
		}
	}
	QList <int> result;
	visited[src] = false;
	for (int i = 0; i < hexes.size(); ++i)
		if (visited[i])
			result.append(i);

	return result;
}

void Grid::drawShootRange(const MechEntity *mech)
{
	QList <int> vArc = getShootRange(mech->getCurrentPositionNumber(),
	                                 mech->getTorsoDirection() + mech->getCurrentDirection());
	for (int dest : vArc)
		hexes[dest]->setAttackObject(getAttackObject(mech, hexes[dest]->getMech()));
}

void Grid::drawFriendlyMechs(const Player *player)
{
	for (MechEntity *mech : player->getMechs())
		mech->setFriendly(true);
}

Grid::TState::TState(Position position, Position father, int movePoints, int distance)
	: position(position), father(father), movePoints(movePoints), distance(distance)
{}

AttackObject Grid::getAttackObject(const MechEntity *attacker, const MechEntity *target) const
{
	AttackObject obj;
	if (target == nullptr)
		return obj;

	int src = attacker->getCurrentPositionNumber();
	int dest = target->getCurrentPositionNumber();

	/** Values */

	obj.setWeaponHolder(attacker);

	obj.setDistance(getHexDistance(src, dest));

	obj.setDirection(getAttackDirection(target->getTorsoDirection() + target->getCurrentDirection(),
	                                    getAttackDirection(src, dest)));
	obj.setActionType(attacker->getCurrentCombatAction()->getType());

	/** Damage */

	switch (obj.getActionType()) {
		case BTech::CombatAction::Idle:
			qCritical() << "Error: Creating AttackObject with action 'idle'";
			exit(EXIT_FAILURE);
		case BTech::CombatAction::SimpleAttack:
			obj.setDamage(attacker->getDamageValue(obj.getDistance()));
			break;
		case BTech::CombatAction::Punch:
			obj.setDamage(attacker->getTonnage() / 10);
			break;
		case BTech::CombatAction::Kick:
			obj.setDamage(attacker->getTonnage() / 5);
			break;
		case BTech::CombatAction::Push:
			break;
		case BTech::CombatAction::Charge:
			break;
		case BTech::CombatAction::WeaponAttack:
			obj.setDamage(obj.getWeapon()->getDamage());
			break;
	}

	/** Attack modifiers */

	obj.addModifier(BTech::ModifierType::Attack,
	                BTech::Modifier::Base,
	                attacker->getBaseAttackModifier());

	//TODO ABD
	obj.addModifier(BTech::ModifierType::Attack,
	                BTech::Modifier::Range,
	                AttackObject::getRangeModifier(BTech::ModifierType::Attack, attacker->distanceToRange(obj.getDistance())));

	obj.addModifier(BTech::ModifierType::Attack,
	                BTech::Modifier::Terrain,
	                AttackObject::getTerrainModifier(BTech::ModifierType::Attack, getLineOfSight(src, dest)));

	obj.addModifier(BTech::ModifierType::Attack,
	                BTech::Modifier::AttackerMovement,
	                attacker->getAttackAttackerMovementModifier());

	obj.addModifier(BTech::ModifierType::Attack,
	                BTech::Modifier::TargetMovement,
	                target->getAttackTargetMovementModifier());

	/** Armor penetration modifiers */

	if (Rules::getVersion() == BTech::GameVersion::BasicBattleDroids) {
		obj.addModifier(BTech::ModifierType::ArmorPenetration,
		                BTech::Modifier::Base,
		                BTech::armorPenetrationTable[attacker->getDamageValue(obj.getDistance())][target->getArmorValue()]);
	}

	obj.addModifier(BTech::ModifierType::ArmorPenetration,
	                BTech::Modifier::Direction,
	                AttackObject::getDirectionModifier(BTech::ModifierType::ArmorPenetration,
	                                                   obj.getDirection()));

	return obj;
}

QList <MoveObject> Grid::getWalkRange(const MovementObject &movementObject) const
{
	int src = movementObject.getSrc().getNumber();
	Direction direction = movementObject.getSrc().getDirection();
	int movePoints = movementObject.getMovePoints();

	QList <MoveObject> result;

	if (src < 0)
		return result;

	TState states[hexes.size()][Direction::NUMBER];

	for (int i = 0; i < hexes.size(); ++i)
		for (Direction curDir : BTech::directions)
			states[i][curDir] = TState(Position(i, curDir),
			                           Position(src, direction),
			                           std::numeric_limits<int>::max(),
			                           0);
	states[src][direction].movePoints = 0;

	QQueue <TState> queue;
	queue.enqueue(TState(Position(src, direction),
	                     Position(src, direction),
	                     0,
	                     0));

	int cNum, cMPS, nNum, cDist;
	Position cPos;
	Direction cDir, nDir;
	TState cur;

	/** BFS */
	while (!queue.empty()) {
		cur = queue.dequeue();
		cPos = cur.position;
		cDir = cPos.getDirection();
		cNum = cPos.getNumber();
		cMPS = cur.movePoints;
		cDist = cur.distance;
		if (cNum >= 0 && cMPS <= movePoints) {

			/** Turn right, turn left */
			nDir = cDir.onRight();
			repeat_twice {
				if (states[cNum][nDir].movePoints > cMPS + 1) {
					states[cNum][nDir].father = cPos;
					states[cNum][nDir].movePoints = cMPS + 1;
					states[cNum][nDir].distance = cDist;
					queue.enqueue(states[cNum][nDir]);
				}
				nDir = cDir.onLeft();
			}

			/** Make progress */
			for (QPair <Direction, Direction> next : movementObject.getAllowedMoves()) {
				nNum = nextHex(cNum, cDir + next.first);
				nDir = cDir + next.second;

				if (nNum >= 0 && hexes[nNum]->getMech() == nullptr) {
					int heightDifference = qAbs(hexes[cNum]->getHeight() - hexes[nNum]->getHeight());
					int travelCost = movementObject.getHeightPenalty(heightDifference)
						         + movementObject.getTerrainPenalty(hexes[nNum]->getTerrain()); // TODO jump (right now it's cheat)
					if (states[nNum][nDir].movePoints > cMPS + travelCost) {
						states[nNum][nDir].father = cPos;
						states[nNum][nDir].movePoints = cMPS + travelCost;
						states[nNum][nDir].distance = cDist + 1;
						queue.enqueue(states[nNum][nDir]);
					}
				}
			}
		}
	}

	//TODO in ABD mech can cross hexes with allied units

	/**
	 * For each position the path from the source to this position is found.
	 * Then, the MoveObject is created and inserted into the result list.
	 */
	for (int i = 0; i < hexes.size(); ++i) {
		for (Direction curDir : BTech::directions) {
			TState state = states[i][curDir];
			TState current = state;

			if (state.movePoints <= movePoints
			    && state.position != movementObject.getSrc()) {

				QList <Position> path;
				do {
					path.prepend(current.position);
					current = states[current.father.getNumber()][current.father.getDirection()];
				} while (current.position != movementObject.getSrc());

				result.append(MoveObject(movementObject,
				                         state.position,
				                         state.movePoints,
				                         state.distance,
				                         path));
			}
		}
	}

	return result;
}

void Grid::drawWalkRange(const MovementObject &movement)
{
	QList <MoveObject> wRange = getWalkRange(movement);
	for (MoveObject &object : wRange)
		hexes[object.getDest().getNumber()]->setMoveObject(object);
}

LineOfSight Grid::visibilityScore(const Hex *hex, const Hex *src, const Hex *dest) const
{
	LineOfSight result;
	if (hex != nullptr && hex != src && hex != dest) {
		int height = qMax(src->getHeight(), dest->getHeight());
		result.heightBetween = qMax(0, hex->getHeight() - height);
		result.lightWoods += (int)(hex->getTerrain() == BTech::Terrain::LightWoods);
		result.heavyWoods += (int)(hex->getTerrain() == BTech::Terrain::HeavyWoods);
	}
	return result;
}

LineOfSight Grid::pairVisibilityScore(QPair <const Hex *, const Hex *> hexes, const Hex *src, const Hex *dest) const
{
	return qMax(visibilityScore(hexes.first, src, dest),
		    visibilityScore(hexes.second, src, dest));
}
