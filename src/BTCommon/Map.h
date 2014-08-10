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

#ifndef MAP_H
#define MAP_H

#include <QtWidgets>
#include "BTCommon/Action.h"
#include "BTCommon/MoveObject.h"
#include "BTCommon/Utils.h"

class Grid;
class Hex;
class MechEntity;
class Player;

class Map : public QObject
{
Q_OBJECT;

public:
	Map();
	~Map();

	/** Map shape cannot be changed after the map is created. */
	//TODO MapShape / Grid
	quint16 getHeight() const;
	quint16 getWidth() const;

	QString getDescription() const;
	void setDescription(const QString &description);

	QList <BTech::GameVersion> getAllowedVersions() const;
	void setAllowedVersions(const QList <BTech::GameVersion> &allowedVersions);

	void clearMap();
	void newMap(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
	bool loadMap(const QString &mapFileName);
	bool isValid() const;

	void addMech(MechEntity *mech, Hex *hex);
	void removeMech(MechEntity *mech);

	void addPlayer(Player *player);
	void removePlayer(Player *player);

	void updateUnitOwners();

	void endMove();
	void trigger();

	void chooseAction(const Action &action);

	void startGame();
	void endGame();

	QVector <Player *> & getPlayers();
	Player * getPlayer(const QString &name) const;
	QVector <Hex *> & getHexes();

	const Grid * getGrid() const;

	/** API for signals */

	BTech::GamePhase getCurrentPhase() const;
	Hex * getCurrentHex() const;
	MechEntity * getCurrentUnit() const;
	Player * getCurrentPlayer() const;
	Action getCurrentAction() const;
	MovementObject getCurrentMovement() const;

	void setCurrentHex(Hex *hex);

	//TODO this should Not be here
	static const QColor DefaultMessageColor;

	//TODO push it somewhere
	struct Message {
		Message(const QString &text = QString(), const QColor &color = DefaultMessageColor)
			: text(text), color(color) {}
		QString text;
		QColor color;
	};

	Message getMessage() const;
	void sendMessage(const Message &message);
	void sendMessage(const QString &text, const QColor &color = DefaultMessageColor);
	Message getExtensiveInfo() const;
	void sendExtensiveInfo(const Message &message);
	void sendExtensiveInfo(const QString &text, const QColor &color = DefaultMessageColor);

	friend QDataStream & operator << (QDataStream &out, const Map &map);
	friend QDataStream & operator >> (QDataStream &in, Map &map);
	friend QDebug operator << (QDebug out, const Map &map);

signals:
	void gameStarted();
	void gameEnded();

	void hexesInitialized();
	void unitInitialized();

	void mechAdded();
	void mechInfoNeeded();
	void mechInfoNotNeeded();
	void mechActionsNeeded();
	void mechActionsNotNeeded();
	void movementRangeNeeded();
	void attackRangeNeeded();
	void rangesNotNeeded();
	void playerTurn();

	//TODO these two put inside: for all hexes: clear - signal -> GraphicsHex::clear
	void hexesNeedClearing();
	void hexesNeedUpdating();

	void messageSent();
	void extensiveInfoSent();

private:
	void initGrid();
	void initMap();
	void initPlayers();
	void initUnits();

	void initUnit(MechEntity *mech);

	void resetCurrentValues();

	void setCurrentPhase(BTech::GamePhase phase);

	void setMechsMoved(bool moved);
	void clearMechs();
	void cleanMechs();
	void decimateMechs();
	void triggerMechTurnRecovery();

	Player * nextPlayer() const;
	bool playerEnded(Player *player) const;
	bool allPlayersEnded() const;
	bool gameOver();
	Player * getWinner() const;

	void noPhase();
	void initiativePhase();
	void movementPhase();
	void reactionPhase();
	void weaponAttackPhase();
	void physicalAttackPhase();
	void combatPhase();
	void heatPhase();
	void endPhase();
	void endThisPhase();

	bool tryToChooseMech();
	bool tryToChooseEnemy();
	void attackEnemy();

	static const qint16 DEFAULT_WIDTH  = 40;
	static const qint16 DEFAULT_HEIGHT = 40;

	quint16 width;
	quint16 height;

	QString description;
	QList <BTech::GameVersion> allowedVersions;

	QVector <Hex *> hexes;
	Grid *grid;

	bool mapValid;

	/**
	 * \enum GameSubPhase
	 * This is an auxiliary type specyfying the state in the game graph.
	 */
	enum class GameSubphase : quint8 {
		None,
		MechChosen
	};

	BTech::GamePhase phase;
	GameSubphase subphase;

	static const QHash <BTech::GamePhase, void (Map::*)()> phaseToFunction;

	Message message;
	Message extInfo;

	QVector <Player *> players;

	Action         currentAction;
	MovementObject currentMovement;
	Player         *currentPlayer;
	MechEntity     *currentUnit;
	Hex            *currentHex;

private slots:
	void mechInfoReceived();
	void mechExtensiveInfoReceived();
	void mechStateInfoReceived();
};

#endif // MAP_H
