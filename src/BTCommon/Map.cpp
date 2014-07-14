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
#include "BTCommon/Grid.h"
#include "BTCommon/Hex.h"
#include "BTCommon/Map.h"
#include "BTCommon/MechEntity.h"
#include "BTCommon/Player.h"
#include "BTCommon/TileManager.h"

const QColor Map::DefaultMessageColor = Qt::white;

Map::Map()
	: mapLoaded(false), grid(new Grid())
{
}

qint16 Map::getMapWidth() const
{
	return mapWidth;
}

qint16 Map::getMapHeight() const
{
	return mapHeight;
}

void Map::setDescription(const QString &description)
{
	this->description = description;
}

QString Map::getDescription() const
{
	return description;
}

QString & Map::getDescriptionReference()
{
	return description;
}

QList <BTech::GameVersion> & Map::getAllowedVersionsRef()
{
	return allowedVersions;
}

void Map::createNewMap(int width, int height)
{
	setMapFileName(QString());
	this->mapWidth = width;
	this->mapHeight = height;
	for (int i = 0; i < mapHeight; ++i) {
		for (int j = 0; j < mapWidth; ++j) {
			Hex *hex = new Hex;
			hex->setCoordinate({j + 1, i + 1});
			hex->setTerrain(BTech::Terrain::Clear);
			hex->setHeight(0);
			hexes << hex;
		}
	}
	setDescription(QString());
	allowedVersions = QList <BTech::GameVersion> ();
	allowedVersions.append(BTech::GameVersion::BasicBattleDroids);

	setCurrentPhase(BTech::GamePhase::None);
	setCurrentSubPhase(GameSubPhase::None);
}

bool Map::loadMap(const QString &mapFileName)
{
	QFile file(mapFileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	QDataStream in(&file);
	in >> *this;

	setCurrentPhase(BTech::GamePhase::None);
	setCurrentSubPhase(GameSubPhase::None);

	for (Player *player : players) {
		for (MechEntity *mech : player->getMechs()) {
			const Coordinate mechCoord = mech->getCurrentPosition().getCoordinate();
			Hex *hex = grid->getHexByCoordinate(mechCoord);
			hex->setMech(mech);
			mech->setMechPosition(hex);
		}
	}
	initPlayers();

	setMapFileName(mapFileName);
	emitMessageSent(BTech::Messages::MapLoaded + mapFileName);
	emitMessageSent(BTech::Messages::Separator);
	return true;
}

bool Map::isLoaded() const
{
	return mapLoaded;
}

void Map::setMapFileName(const QString &path)
{
	mapFileName = path;
}

QString Map::getMapFileName() const
{
	return mapFileName;
}

void Map::addPlayer(Player *player)
{
	players.append(player);
}

void Map::removePlayer(Player *player)
{
	for (int i = 0; i < players.size(); ++i)
		if (players[i] == player)
			players.remove(i);
}

void Map::updatePlayers()
{
	for (Player *player : players)
		for (MechEntity *mech : player->getMechs())
			mech->setOwnerName(player->getName());
}

void Map::startGame()
{
	qDebug() << "Start game\n";
	emitMessageSent(BTech::Messages::GameStarted);
	emitMessageSent(BTech::Messages::Separator);
	emitGameStarted();
	setCurrentPhase(BTech::GamePhase::Initiative);
	initiativePhase();
	updateHexes();
}

void Map::endGame()
{
	qDebug() << "\nEnd game\n";
	emitMessageSent(BTech::Messages::GameOver);

	Player *winner = getWinner();
	if (winner == nullptr) {
		emitMessageSent(BTech::Messages::NoWinner);
	} else {
		QString winner = getWinner()->getName();
		emitMessageSent(BTech::Messages::WinMessage);
		emitMessageSent(winner, playerNameToColor[winner]);
	}

	emitGameEnded();
	clearMap();
}

void Map::updateHexes()
{
	emitHexesNeedUpdating();
}

QVector <Player *> & Map::getPlayers()
{
	return players;
}

QVector <Hex *> & Map::getHexes()
{
	return hexes;
}

MechEntity * Map::getCurrentMech() const
{
	return currentMech;
}

Hex * Map::getCurrentHex() const
{
	return currentHex;
}

Player * Map::getCurrentPlayer() const
{
	return currentPlayer;
}

BTech::GamePhase Map::getCurrentPhase() const
{
	return currentPhase;
}

QDataStream & operator << (QDataStream &out, const Map &map)
{
	out << map.mapFileName << map.description << map.allowedVersions;
	qDebug() << "Saving map" << map.mapFileName << "...";

	out << Rules::getVersion();

	out << map.mapWidth << map.mapHeight;
	for (Hex *hex : map.hexes)
		out << *hex;

	out << map.players.size();
	for (Player *player : map.players)
		out << *player;

	TileManager::saveTileDictionary(out, map.hexes);

	qDebug() << "Done";

	return out;
}

QDataStream & operator >> (QDataStream &in, Map &map)
{
	in >> map.mapFileName >> map.description >> map.allowedVersions;
	qDebug() << "\nLoading map" << map.mapFileName << "...";

	BTech::GameVersion version;
	in >> version;
	Rules::setVersion(version);

	in >> map.mapWidth >> map.mapHeight;

	for (int i = 0; i < map.mapWidth * map.mapHeight; ++i) {
		Hex *hex = new Hex;
		in >> *hex;
		map.hexes.append(hex);
	}
	map.grid->initGrid(map.hexes);
	qDebug() << "Hexes loaded.";

	int playersSize;
	in >> playersSize;
	for (int i = 0; i < playersSize; ++i) {
		Player *player = new Player;
		in >> *player;
		map.players.append(player);
	}

	qDebug() << "Players loaded.";

	TileManager::loadTileDictionary(in, map.hexes);
	qDebug() << "Tiles loaded.";

	qDebug() << "Done.\n";

	return in;
}

const Grid * Map::getGrid() const
{
	return grid;
}

void Map::setMechsMoved(bool moved)
{
	for (Player *player : players)
		for (MechEntity *mech : player->getMechs())
			mech->setMoved(moved);
}

void Map::clearMechs()
{
	for (Player *player : players)
		for (MechEntity *mech : player->getMechs())
			mech->clear();
}

void Map::cleanMechs()
{
	setMechsMoved(false);
	clearMechs();
}

void Map::decimateMechs()
{
	QList <QPair <Player *, MechEntity *> > toRemove;

	for (Player *player : players) {
		for (MechEntity *mech : player->getMechs()) {
			setCurrentMech(mech);
			mech->resolveAttacks();
			if (mech->hasEffect(BTech::EffectType::Destroyed))
				toRemove.append({player, mech});
		}
	}

	for (auto &obj : toRemove)
		obj.first->removeMech(obj.second);

	cleanMechs();
}

void Map::triggerMechTurnRecovery()
{
	qDebug() << "Mech turn recovery triggered...";
	for (Player *player : players)
		for (MechEntity *mech : player->getMechs())
			mech->recover();
	qDebug() << "Complete.";
}

Player * Map::nextPlayer() const
{
	for (int i = 0; i < players.size(); ++i)
		if (players[i] == getCurrentPlayer())
			return players[(i + 1) % players.size()];
	return nullptr;
}

void Map::endMove()
{
	qDebug() << "End move";
	if (getCurrentMech() != nullptr)
		getCurrentMech()->setMoved(true);
	clearMechs();
	emitHexesNeedClearing();
	emitMechInfoNotNeeded();
	emitMechActionsNotNeeded();
	resetCurrentValues();
	if (allPlayersEnded()) {
		endThisPhase();
	} else {
		do
			setCurrentPlayer(nextPlayer());
		while (playerEnded(getCurrentPlayer()));
		emitMessageSent(getCurrentPlayer()->getName(), playerNameToColor[getCurrentPlayer()->getName()]);
		emitPlayerTurn(getCurrentPlayer());
	}
}

bool Map::playerEnded(Player *player) const
{
	for (MechEntity *mech : player->getMechs())
		if (!mech->isMoved())
			return false;
	return true;
}

bool Map::allPlayersEnded() const
{
	for (Player *player : players)
		if (!playerEnded(player))
			return false;
	return true;
}

bool Map::gameOver()
{
	bool oneLives = false;
	for (Player *player : players) {
		if (!player->getMechs().isEmpty()) {
			if (oneLives)
				return false;
			oneLives = true;
		}
	}
	return true;
}

Player * Map::getWinner() const
{
	for (Player * player : players)
		if (!player->getMechs().isEmpty())
			return player;
	return nullptr;
}

void Map::initiativePhase()
{
	QVector<int> initiative;
	for (int i = 0; i < players.size(); ++i)
		initiative << BTech::d2Throw();
	for (int i = 0; i < players.size(); ++i) {	// bubble sort!
		for (int j = 0; j < players.size() - 1; ++j) {
			if (initiative[j] > initiative[j + 1]) {
				qSwap(players[j], players[j + 1]);
				qSwap(initiative[j], initiative[j + 1]);
			}
		}
	}
	for (int i = 0; i < players.size(); ++i)
		emitMessageSent(QString(char(i) + '1') + ": " + players[i]->getName(),
		                playerNameToColor[players[i]->getName()]);
	endThisPhase();
}

void Map::movementPhase()
{
	switch (getCurrentSubPhase()) {
		case GameSubPhase::None:
			if (tryToChooseMech())
				emitMechActionsNeeded(BTech::GamePhase::Movement);
			break;
		case GameSubPhase::MechChosen:
			MoveObject moveObject = getCurrentHex()->getMoveObject();
			if (getCurrentAction() == nullptr || moveObject.getAction() == BTech::MovementAction::Idle) {
				qDebug() << "\tno existing move object";
				break;
			}
			grid->getHexByCoordinate(getCurrentMech()->getCurrentPosition().getCoordinate())->removeMech();
			getCurrentMech()->move(moveObject);
			getCurrentHex()->setMech(getCurrentMech());
			emitMechInfoNeeded(getCurrentMech());
			emitMechWalkRangeNeeded(
				MovementObject(moveObject.getDest(),
				               getCurrentMech()->getMovePoints(moveObject.getAction()),
				               moveObject.getAction()));
			break;
	}
}

void Map::reactionPhase()
{
}

void Map::weaponAttackPhase()
{
	switch (getCurrentSubPhase()) {
		case GameSubPhase::None:
			if (tryToChooseMech())
				emitMechActionsNeeded(BTech::GamePhase::WeaponAttack);
			break;
		case GameSubPhase::MechChosen:
			if (getCurrentAction() == nullptr)
				break;
			if (tryToChooseEnemy())
				attackEnemy();
			break;
	}
}

void Map::physicalAttackPhase()
{
	switch (getCurrentSubPhase()) {
		case GameSubPhase::None:
			if (tryToChooseMech())
				emitMechActionsNeeded(BTech::GamePhase::PhysicalAttack);
			break;
		case GameSubPhase::MechChosen:
			if (getCurrentAction() == nullptr)
				break;
			if (tryToChooseEnemy())
				attackEnemy();
			break;
	}
}

void Map::combatPhase()
{
	switch (getCurrentSubPhase()) {
		case GameSubPhase::None:
			if (tryToChooseMech())
				emitMechActionsNeeded(BTech::GamePhase::Combat);
			break;
		case GameSubPhase::MechChosen:
			if (getCurrentAction() == nullptr)
				break;
			if (tryToChooseEnemy())
				attackEnemy();
			break;
	}
}

void Map::heatPhase()
{
	for (Player *player : players)
		for (MechEntity *mech : player->getMechs())
			mech->resolveHeat();
	setMechsMoved(true);
	endThisPhase();
}

void Map::endPhase()
{
	triggerMechTurnRecovery();
	emitMessageSent(BTech::Messages::Separator);
	endThisPhase();
}

void Map::endThisPhase()
{
	qDebug() << "End this phase\n";
	decimateMechs();

	int phaseNumber = static_cast<int>(getCurrentPhase());
	do {
		phaseNumber = (phaseNumber + 1) % (BTech::phases.size() + 1);
	} while (!Rules::getAllowedPhases().contains(static_cast<BTech::GamePhase>(phaseNumber)));

	setCurrentPhase(static_cast<BTech::GamePhase>(phaseNumber));

	if (gameOver()) {
		endGame();
		return;
	}

	qDebug() << "Phase:" << BTech::phaseStringChange[getCurrentPhase()];

	switch (getCurrentPhase()) {
		case BTech::GamePhase::Initiative:
			initiativePhase();
			break;
		case BTech::GamePhase::Heat:
			heatPhase();
			break;
		case BTech::GamePhase::End:
			endPhase();
			break;
		default:
			setCurrentPlayer(players[0]);
			while (playerEnded(getCurrentPlayer()))
				setCurrentPlayer(nextPlayer());
			emitMessageSent(getCurrentPlayer()->getName(), playerNameToColor[getCurrentPlayer()->getName()]);
			emitPlayerTurn(getCurrentPlayer());
	}
}

bool Map::tryToChooseMech()
{
	qDebug() << "Try to choose mech";
	MechEntity *mech = getCurrentHex()->getMech();
	if (mech == nullptr || mech->isMoved() || !getCurrentPlayer()->hasMech(mech)) {
		qDebug() << "\tfailure";
		return false;
	}
	setCurrentMech(mech);
	getCurrentMech()->setActive(true);
	setCurrentSubPhase(GameSubPhase::MechChosen);
	emitMechInfoNeeded(getCurrentMech());
	qDebug() << "\tchosen" << static_cast<QString>(*getCurrentMech());
	return true;
}

bool Map::tryToChooseEnemy()
{
	qDebug() << "Try to choose enemy";
	MechEntity *enemy = getCurrentHex()->getMech();
	return enemy != nullptr && !getCurrentPlayer()->hasMech(enemy) && getCurrentHex()->hasAttackObject();
}

void Map::attackEnemy()
{
	qDebug() << "Attack enemy";
	MechEntity *enemy = getCurrentHex()->getMech();
	getCurrentMech()->attack(enemy);
	emitMechRangesNotNeeded();
}

void Map::chooseAction(const Action *action)
{
	getCurrentMech()->setCurrentAction(action);
	if (getCurrentAction() != nullptr) {
		if (getCurrentAction()->getActionType() == Action::Type::Movement) {
			BTech::MovementAction movementAction =
				static_cast<const MovementAction *>(getCurrentAction())->getType();
			switch (movementAction) {
				case BTech::MovementAction::Idle:
					break;
				case BTech::MovementAction::Walk:
				case BTech::MovementAction::Run:
				case BTech::MovementAction::Jump:
					emitMechWalkRangeNeeded(MovementObject(
						getCurrentMech()->getCurrentPosition(),
						getCurrentMech()->getMovePoints(movementAction),
						movementAction));
					break;
				case BTech::MovementAction::TurnLeft:
					getCurrentMech()->turnLeft();
					getCurrentMech()->setCurrentAction(nullptr);
					break;
				case BTech::MovementAction::TurnRight:
					getCurrentMech()->turnRight();
					getCurrentMech()->setCurrentAction(nullptr);
					break;
			}
		} else {
			BTech::CombatAction combatAction =
				static_cast<const CombatAction *>(getCurrentAction())->getType();
			switch (combatAction) {
				case BTech::CombatAction::Idle:
					break;
				case BTech::CombatAction::SimpleAttack:
				case BTech::CombatAction::WeaponAttack:
					emitMechShootRangeNeeded(getCurrentMech());
					break;
				default:;
			}
		}
	}

	if (getCurrentMech() != nullptr)
		emitMechInfoNeeded(getCurrentMech());
	updateHexes();
}

void Map::setCurrentMech(MechEntity *mech)
{
	currentMech = mech;
}

void Map::setCurrentHex(Hex *hex)
{
	currentHex = hex;
}

void Map::setCurrentPhase(BTech::GamePhase phase)
{
	emitMessageSent(BTech::phaseStringChange[phase]);
	currentPhase = phase;
}

void Map::setCurrentSubPhase(Map::GameSubPhase subPhase)
{
	currentSubPhase = subPhase;
}

Map::GameSubPhase Map::getCurrentSubPhase() const
{
	return currentSubPhase;
}

void Map::setCurrentPlayer(Player *player)
{
	currentPlayer = player;
}

void Map::setCurrentAction(const Action *action)
{
	if (currentMech != nullptr)
		currentMech->setCurrentAction(action);
}

const Action * Map::getCurrentAction() const
{
	return currentMech->getCurrentAction();
}

void Map::initGrid()
{
	grid->initGrid(hexes);
}

void Map::initPlayers()
{
	qDebug() << "Players initialization...";
	setCurrentMech(nullptr);
	setCurrentPlayer(nullptr);

	//TODO these colors definitely do not belong in here
	QColor colors[] = {
		Qt::red,
		Qt::green,
		Qt::blue,
		Qt::yellow,
		Qt::magenta,
		Qt::gray,
	};
	int index = 0;
	for (Player *player : players)
		playerNameToColor[player->getName()] = colors[index++];
	qDebug() << "Completed.";
}

void Map::initUnits()
{}

void Map::resetCurrentValues()
{
	setCurrentSubPhase(GameSubPhase::None);
	setCurrentAction(nullptr);
	setCurrentMech(nullptr);
	setCurrentHex(nullptr);
}

void Map::clearMap()
{
	qDebug() << "Clearing map...";

	qDeleteAll(players);
	players.clear();
	qDebug() << "\tplayers deleted";

	qDeleteAll(hexes);
	hexes.clear();
	qDebug() << "\thexes deleted";

	qDebug("Done.");
}
