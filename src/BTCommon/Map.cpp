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

//TODO players colors
const QColor Map::DefaultMessageColor = Qt::white;

Map::Map()
	: grid(new Grid()), mapValid(false)
{}

Map::~Map()
{
	delete grid;
}

quint16 Map::getHeight() const
{
	return height;
}

quint16 Map::getWidth() const
{
	return width;
}

void Map::setDescription(const QString &description)
{
	this->description = description;
}

QString Map::getDescription() const
{
	return description;
}

QList <BTech::GameVersion> Map::getAllowedVersions() const
{
	return allowedVersions;
}

void Map::setAllowedVersions(const QList <BTech::GameVersion> &versions)
{
	allowedVersions = versions;
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

void Map::newMap(int width, int height)
{
	this->width  = width;
	this->height = height;

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			Hex *hex = new Hex;
			hex->setCoordinate({j + 1, i + 1});
			hex->setTerrain(BTech::Terrain::Clear);
			hex->setHeight(0);
			hexes.append(hex);
		}
	}
	emit hexesInitialized();

	setDescription(QString());
	allowedVersions = QList <BTech::GameVersion> ();
	allowedVersions.append(BTech::GameVersion::BasicBattleDroids);

	mapValid = true;

	initMap();
}

bool Map::loadMap(const QString &mapFileName)
{
	QFile file(mapFileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	QDataStream in(&file);
	in >> *this;

	initMap();
	emit hexesInitialized();

	for (Player *player : players) {
		for (MechEntity *mech : player->getMechs()) {
			const Coordinate mechCoord = mech->getCurrentPosition().getCoordinate();
			Hex *hex = grid->getHexByCoordinate(mechCoord);
			hex->setMech(mech);
			mech->setMechPosition(hex);
			initUnit(mech);
		}
	}
	initPlayers();

	mapValid = true;

	sendMessage(BTech::Messages::MapLoaded + mapFileName);
	sendMessage(BTech::Messages::Separator);

	return true;
}

bool Map::isValid() const
{
	return mapValid;
}

void Map::addMech(MechEntity *mech, Hex *hex)
{
	if (mech == nullptr || hex == nullptr)
		return;
	hex->setMech(mech);
	mech->setPosition({hex->getCoordinate(), BTech::DirectionN});
	initUnit(mech);
}

void Map::removeMech(MechEntity *mech)
{
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

void Map::updateUnitOwners()
{
	for (Player *player : players)
		for (MechEntity *mech : player->getMechs())
			mech->setOwnerName(player->getName());
}

void Map::endMove()
{
	qDebug() << "End move";
	if (getCurrentUnit() != nullptr)
		getCurrentUnit()->setMoved(true);
	clearMechs();
	emit hexesNeedClearing();
	emit mechInfoNotNeeded();
	emit mechActionsNotNeeded();
	resetCurrentValues();
	if (allPlayersEnded()) {
		endThisPhase();
	} else {
		do
			currentPlayer = nextPlayer();
		while (playerEnded(getCurrentPlayer()));
		sendMessage(getCurrentPlayer()->getName(), playerNameToColor[getCurrentPlayer()->getName()]);
		emit playerTurn();
	}
}

void Map::trigger()
{
	(this->*Map::phaseToFunction[getCurrentPhase()])();
}

void Map::chooseAction(const Action *action)
{
	getCurrentUnit()->setCurrentAction(action);
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
					currentMovement = MovementObject(
						getCurrentUnit()->getCurrentPosition(),
						getCurrentUnit()->getMovePoints(movementAction),
						movementAction
					);
					emit movementRangeNeeded();
					break;
				case BTech::MovementAction::TurnLeft:
					getCurrentUnit()->turnLeft();
					getCurrentUnit()->setCurrentAction(nullptr);
					break;
				case BTech::MovementAction::TurnRight:
					getCurrentUnit()->turnRight();
					getCurrentUnit()->setCurrentAction(nullptr);
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
					emit attackRangeNeeded();
					break;
				default:;
			}
		}
	}

	if (getCurrentUnit() != nullptr)
		emit mechInfoNeeded();
	emit hexesNeedUpdating();
}

void Map::startGame()
{
	sendExtensiveInfo(BTech::Messages::GameStarted);
	qDebug() << "sending message that game started: " << BTech::Messages::GameStarted;
	sendMessage(BTech::Messages::GameStarted);
	qDebug() << "AND WHERE THE FUCK IS THAT MESSAGE?!";
	sendMessage(BTech::Messages::Separator);
	emit gameStarted();

	setCurrentPhase(BTech::GamePhase::Initiative);
	initiativePhase();
	//TODO check if it can be done better
	emit hexesNeedUpdating();
}

void Map::endGame()
{
	qDebug() << "end game";

	sendExtensiveInfo(BTech::Messages::GameOver);
	sendMessage(BTech::Messages::GameOver);

	Player *winner = getWinner();
	if (winner == nullptr) {
		sendMessage(BTech::Messages::NoWinner);
	} else {
		QString winner = getWinner()->getName();
		sendMessage(BTech::Messages::WinMessage);
		sendMessage(winner, playerNameToColor[winner]);
	}

	emit gameEnded();
	clearMap();
}

QVector <Player *> & Map::getPlayers()
{
	return players;
}

QVector <Hex *> & Map::getHexes()
{
	return hexes;
}

const Grid * Map::getGrid() const
{
	return grid;
}

BTech::GamePhase Map::getCurrentPhase() const
{
	return phase;
}

Hex * Map::getCurrentHex() const
{
	return currentHex;
}

MechEntity * Map::getCurrentUnit() const
{
	return currentUnit;
}

Player * Map::getCurrentPlayer() const
{
	return currentPlayer;
}

const Action * Map::getCurrentAction() const
{
	return currentUnit->getCurrentAction();
}

MovementObject Map::getCurrentMovement() const
{
	return currentMovement;
}

void Map::setCurrentHex(Hex *hex)
{
	currentHex = hex;
}

Map::Message Map::getMessage() const
{
	return message;
}

void Map::sendMessage(const Map::Message &message)
{
	this->message = message;
	emit messageSent();
}

void Map::sendMessage(const QString &text, const QColor &color)
{
	return sendMessage({text, color});
}

Map::Message Map::getExtensiveInfo() const
{
	return extInfo;
}

void Map::sendExtensiveInfo(const Map::Message &message)
{
	extInfo = message;
	emit extensiveInfoSent();
}

void Map::sendExtensiveInfo(const QString &text, const QColor &color)
{
	return sendExtensiveInfo({text, color});
}

QDataStream & operator << (QDataStream &out, const Map &map)
{
	qDebug() << map;

	out << map.description << map.allowedVersions;

	out << Rules::getVersion();

	out << map.width << map.height;
	for (Hex *hex : map.hexes)
		out << *hex;

	out << static_cast<quint16>(map.players.size());
	for (Player *player : map.players)
		out << *player;

	TileManager::saveTileDictionary(out, map.hexes);

	return out;
}

QDataStream & operator >> (QDataStream &in, Map &map)
{
	in >> map.description >> map.allowedVersions;

	BTech::GameVersion version;
	in >> version;
	Rules::setVersion(version);

	in >> map.width >> map.height;

	qDebug() << map;

	for (int i = 0; i < map.width * map.height; ++i) {
		Hex *hex = new Hex;
		in >> *hex;
		map.hexes.append(hex);
	}

	map.grid->initGrid(map.hexes);
	qDebug() << "Hexes loaded.";

	quint16 playersSize;
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

QDebug operator << (QDebug out, const Map &map)
{
	out << "description:      " << map.getDescription() << "\n";
	out << "width:            " << map.getWidth() << "\n";
	out << "height:           " << map.getHeight() << "\n";
	out << "allowed versions: \n";
	for (BTech::GameVersion version : map.getAllowedVersions())
		out << "\t" << BTech::gameVersionStringChange[version] << "\n";

	return out;
}

void Map::initGrid()
{
	grid->initGrid(hexes);
}

void Map::initMap()
{
	phase = BTech::GamePhase::None;
	resetCurrentValues();
	currentPlayer = nullptr;
}

void Map::initPlayers()
{
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
}

void Map::initUnits()
{
	for (Player *player : players)
		for (MechEntity *unit : player->getMechs())
			initUnit(unit);
}

void Map::initUnit(MechEntity *unit)
{
	currentUnit = unit;

	connect(unit, &MechEntity::stateInfoSent, this, &Map::mechStateInfoReceived);
	connect(unit, &MechEntity::infoSent, this, &Map::mechInfoReceived);
	connect(unit, &MechEntity::extensiveInfoSent, this, &Map::mechExtensiveInfoReceived);

	emit unitInitialized();
}

//TODO I sense a Something here
void Map::resetCurrentValues()
{
	subphase      = GameSubphase::None;
	currentAction = nullptr;
	currentUnit   = nullptr;
	currentHex    = nullptr;
}

void Map::setCurrentPhase(BTech::GamePhase phase)
{
	sendMessage(BTech::phaseStringChange[phase]);
	this->phase = phase;
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
			currentUnit = mech;
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

void Map::noPhase()
{
	emit rangesNotNeeded();
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
		sendMessage(QString(char(i) + '1') + ": " + players[i]->getName(),
		            playerNameToColor[players[i]->getName()]);
	endThisPhase();
}

void Map::movementPhase()
{
	switch (subphase) {
		case GameSubphase::None:
			if (tryToChooseMech())
				emit mechActionsNeeded();
			break;
		case GameSubphase::MechChosen:
			MoveObject moveObject = getCurrentHex()->getMoveObject();
			if (getCurrentAction() == nullptr || moveObject.getAction() == BTech::MovementAction::Idle) {
				qDebug() << "\tno existing move object";
				break;
			}
			grid->getHexByCoordinate(getCurrentUnit()->getCurrentPosition().getCoordinate())->removeMech();
			getCurrentUnit()->move(moveObject);
			getCurrentHex()->setMech(getCurrentUnit());
			emit mechInfoNeeded();
			currentMovement =
				MovementObject(moveObject.getDest(),
				               getCurrentUnit()->getMovePoints(moveObject.getAction()),
				               moveObject.getAction());
			emit movementRangeNeeded();
			break;
	}
}

void Map::reactionPhase()
{
}

void Map::weaponAttackPhase()
{
	switch (subphase) {
		case GameSubphase::None:
			if (tryToChooseMech())
				emit mechActionsNeeded();
			break;
		case GameSubphase::MechChosen:
			if (getCurrentAction() == nullptr)
				break;
			if (tryToChooseEnemy())
				attackEnemy();
			break;
	}
}

void Map::physicalAttackPhase()
{
	switch (subphase) {
		case GameSubphase::None:
			if (tryToChooseMech())
				emit mechActionsNeeded();
			break;
		case GameSubphase::MechChosen:
			if (getCurrentAction() == nullptr)
				break;
			if (tryToChooseEnemy())
				attackEnemy();
			break;
	}
}

void Map::combatPhase()
{
	switch (subphase) {
		case GameSubphase::None:
			if (tryToChooseMech())
				emit mechActionsNeeded();
			break;
		case GameSubphase::MechChosen:
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
	sendMessage(BTech::Messages::Separator);
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
			currentPlayer = players[0];
			while (playerEnded(getCurrentPlayer()))
				currentPlayer = nextPlayer();
			sendMessage(getCurrentPlayer()->getName(),
			            playerNameToColor[getCurrentPlayer()->getName()]);
			emit playerTurn();
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
	currentUnit = mech;
	getCurrentUnit()->setActive(true);
	subphase = GameSubphase::MechChosen;
	emit mechInfoNeeded();
	qDebug() << "\tchosen" << static_cast<QString>(*getCurrentUnit());
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
	getCurrentUnit()->attack(enemy);
	emit rangesNotNeeded();
}

void Map::mechInfoReceived()
{
	MechEntity *mech = static_cast<MechEntity *>(sender());
	if (mech->getInfo().isEmpty())
		sendMessage(QString(*mech), playerNameToColor[mech->getOwnerName()]);
	else
		sendMessage(mech->getName() + ": " + mech->getInfo(),
		            playerNameToColor[mech->getOwnerName()]);
}

void Map::mechExtensiveInfoReceived()
{
	MechEntity *mech = static_cast<MechEntity *>(sender());
	if (mech->getInfo().isEmpty())
		sendExtensiveInfo(QString(*mech), playerNameToColor[mech->getOwnerName()]); /** signature */
	else
		sendExtensiveInfo(mech->getInfo());
}

void Map::mechStateInfoReceived()
{
	sendMessage(getCurrentUnit()->getOwnerName() + ": " + getCurrentUnit()->getName() + ": " + getCurrentUnit()->getInfo(),
	            playerNameToColor[getCurrentUnit()->getOwnerName()]);
}

const QHash <BTech::GamePhase, void (Map::*)()> Map::phaseToFunction {
	{BTech::GamePhase::None,           &Map::noPhase},
	{BTech::GamePhase::Initiative,     &Map::initiativePhase},
	{BTech::GamePhase::Movement,       &Map::movementPhase},
	{BTech::GamePhase::Reaction,       &Map::reactionPhase},
	{BTech::GamePhase::WeaponAttack,   &Map::weaponAttackPhase},
	{BTech::GamePhase::PhysicalAttack, &Map::physicalAttackPhase},
	{BTech::GamePhase::Combat,         &Map::combatPhase},
	{BTech::GamePhase::Heat,           &Map::heatPhase},
	{BTech::GamePhase::End,            &Map::endPhase},
};
