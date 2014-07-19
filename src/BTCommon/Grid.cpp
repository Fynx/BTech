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

#include "BTCommon/GraphicsFactory.h"
#include "BTCommon/GraphicsHex.h"
#include "BTCommon/Grid.h"
#include "BTCommon/Hex.h"
#include "BTCommon/MechEntity.h"
#include "BTCommon/Player.h"

Grid::Grid()
{
}

void Grid::initGrid(const QVector <Hex *> &hexes)
{
	coordinateToHex.clear();
	for (Hex *hex : hexes)
		coordinateToHex.insert(hex->getCoordinate(), hex);

	for (Hex *hex : hexes) {
		for (Direction direction : BTech::directions) {
			const Coordinate neighbourCoord = nextCoord(hex->getCoordinate(), direction);
			Hex *neighbour = getHexByCoordinate(neighbourCoord);
			hex->setNeighbour(direction, neighbour);
		}
	}
}

AttackObject Grid::getAttackObject(const MechEntity *attacker, const MechEntity *target) const
{
	AttackObject obj;
	if (target == nullptr)
		return obj;

	Hex *src = getHexByCoordinate(attacker->getCurrentCoordinate());
	Hex *dest = getHexByCoordinate(target->getCurrentCoordinate());

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

Hex * Grid::getHexByCoordinate(const Coordinate &coordinate) const
{
	return coordinateToHex.value(coordinate, nullptr);
}

QList <Hex *> Grid::getShootRange(Hex *src, Direction direction) const
{
	if (src == nullptr)
		return QList <Hex *>();

	QSet <Hex *> visited;
	visited.insert(src);

	Hex *cur, *next;

	QQueue <Hex *> queue;
	queue.enqueue(src);

	QList <Direction> directions {direction.onLeft(), direction, direction.onRight()};
	while (!queue.empty()) {
		cur = queue.dequeue();
		for (Direction curDirection : directions) {
			next = nextHex(cur, curDirection);
			if (next != nullptr && !visited.contains(next)) {
				queue.enqueue(next);
				visited.insert(next);
			}
		}
	}

	visited.remove(src);
	return visited.toList();
}

QList <MoveObject> Grid::getWalkRange(const MovementObject &movementObject) const
{
	Position src = movementObject.getSrc();
	int movePoints = movementObject.getMovePoints();

	/**
	 * \struct BfsState
	 * This class is a container storing states in Hex searching.
	 */
	struct BfsState {
		Position position;
		Position parent;
		int movePoints;
		int distance;
	};

	QHash <Position, BfsState> states;
	states[src] = {src, src, 0, 0};

	QQueue <Position> queue;
	queue.enqueue(src);

	/* BFS */
	while (!queue.empty()) {
		BfsState cur  = states[queue.dequeue()];
		Position cPos = cur.position;
		int cMPS      = cur.movePoints;
		int cDist     = cur.distance;

		if (cMPS <= movePoints) {
			/* Turn right, turn left */
			Position nPos{cPos.getCoordinate(), cPos.getDirection().onRight()};
			repeat_twice {
				if (!states.contains(nPos) || states[nPos].movePoints > cMPS + 1) {
					states[nPos] = {nPos, cPos, cMPS + 1, cDist};
					queue.enqueue(nPos);
				}
				nPos.setDirection(cPos.getDirection().onLeft());
			}

			/* Make progress */
			for (QPair <Direction, Direction> next : movementObject.getAllowedMoves()) {
				Hex *cHex = getHexByCoordinate(cPos.getCoordinate());
				Hex *nHex = nextHex(cHex, cPos.getDirection() + next.first);
				if (nHex != nullptr && nHex->getMech() == nullptr) {
					Position nPos{nHex->getCoordinate(), cPos.getDirection() + next.second};
					int heightDifference = qAbs(cHex->getHeight() - nHex->getHeight());
					int travelCost = movementObject.getHeightPenalty(heightDifference)
					               + movementObject.getTerrainPenalty(nHex->getTerrain()); // TODO jump (right now it's cheat)
					if (!states.contains(nPos) || states[nPos].movePoints > cMPS + travelCost) {
						states[nPos] = {nPos, cPos, cMPS + travelCost, cDist + 1};
						queue.enqueue(nPos);
					}
				}
			}
		}
	}

	//TODO in ABD mech can cross hexes with allied units

	/*
	 * For each position the path from the source to this position is found.
	 * Then, the MoveObject is created and inserted into the result list.
	 */

	QList <MoveObject> result;
	for (BfsState state : states) {
		if (state.movePoints <= movePoints && state.position != movementObject.getSrc()) {
			QList <Position> path;
			BfsState current = state;

			int i_ = 0;
			while (current.position != movementObject.getSrc()) {
				path.prepend(current.position);
				current = states[current.parent];
				++i_;
				if (i_ > 50)
					exit(0);
			}

			result.append(MoveObject(movementObject,
			                         state.position,
			                         state.movePoints,
			                         state.distance,
			                         path));
		}
	}

	return result;
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

Direction Grid::getAttackDirection(Hex *src, Hex *dest) const
{
	if (src == dest)
		return BTech::DirectionN;
	QList <QPair <const Hex *, const Hex *> > path = hexesBetween(src, dest);
	const Hex *last = path[path.size() - 2].first;

	for (Direction direction : BTech::directions)
		if (last == nextHex(dest, direction))
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

bool Grid::lineOfSightExists(const Hex *src, const Hex *dest) const
{
	return AttackObject::getTerrainModifier(BTech::ModifierType::Attack, getLineOfSight(src, dest)) != BTech::INF_ATTACK_MODIFIER;
}

const Coordinate Grid::nextCoord(const Coordinate &coordinate, Direction direction) const
{
	//Coordinate change depends on direction AND parity of x()
	static const Coordinate coordChange[6][2] {
		{Coordinate{ 0, -1}, Coordinate{ 0, -1}}, //DirectionN
		{Coordinate{ 1,  0}, Coordinate{ 1, -1}}, //DirectionNE
		{Coordinate{ 1,  1}, Coordinate{ 1,  0}}, //DirectionSE
		{Coordinate{ 0,  1}, Coordinate{ 0,  1}}, //DirectionS
		{Coordinate{-1,  1}, Coordinate{-1,  0}}, //DirectionSW
		{Coordinate{-1,  0}, Coordinate{-1, -1}}, //DirectionNW
	};

	return coordinate + coordChange[direction][coordinate.x() & 1];
}

Hex * Grid::nextHex(Hex *hex, Direction direction) const
{
	return hex->getNeighbour(direction);
}

static bool onTheBorder(const Hex *src, const Hex *dest)
{
	int sx = src->getCoordinate().x();
	int sy = src->getCoordinate().y();
	int dx = dest->getCoordinate().x();
	int dy = dest->getCoordinate().y();

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

int Grid::getHexDistance(const Hex *src, const Hex *dest) const
{
	return hexesBetween(src, dest).size() - 1;
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
