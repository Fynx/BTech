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

#ifndef MAP_H
#define MAP_H

#include <QtWidgets>
#include "BTCommon/CommonStrings.h"
#include "BTCommon/Grid.h"
#include "BTCommon/Hex.h"
#include "BTCommon/InfoBar.h"
#include "BTCommon/MechEntity.h"
#include "BTCommon/Player.h"
#include "BTCommon/Rules.h"
#include "BTCommon/Utils.h"

/**
 * \class Map
 * This class provides functions and enums required by the game system. Before the game can be started, calling loadMap() is essential.
 * It does not provide the tools for displaying the map or object, due to the fact that it organises only the game structures and objects.
 * Also, instance of this class cannot be created, because of pure virtual non-QObject functions that require implementation.
 */
class Map
{

public:
	Map();

	void setDescription(const QString &description);
	QString getDescription() const;
	QString & getDescriptionReference();
	QList <BTech::GameVersion> & getAllowedVersionsRef();

	void createNewMap(int width, int height);
	bool loadMap(const QString &mapFileName);
	bool isLoaded() const;
	void setMapFileName(const QString &path);
	QString getMapFileName() const;

	void addPlayer(Player *player);
	void removePlayer(Player *player);
	void updatePlayers();

	void startGame();
	void endGame();

	void updateHexes();

	QVector <Player *> & getPlayers();
	QVector <Hex *> & getHexes();

	MechEntity * getCurrentMech() const;
	Hex * getCurrentHex() const;
	Player * getCurrentPlayer() const;
	BTech::GamePhase getCurrentPhase() const;

	friend QDataStream & operator << (QDataStream &out, const Map &map);
	friend QDataStream & operator >> (QDataStream &in, Map &map);

protected:
	bool mapLoaded;
	QString mapFileName;
	QVector <Hex *> hexes;

	qint16 hexWidth;
	qint16 hexHeight;

	QVector <Player *> players;
	void countInitiative();
	void setMechsMoved(bool moved);
	void clearMechs();
	void cleanMechs();
	void decimateMechs();
	void triggerMechTurnRecovery();

	/**
	 * \enum GameSubPhase
	 * This is an auxiliary type specyfying the state in the game graph.
	 */
	enum class GameSubPhase : quint8 {
		None,
		MechChosen
	};

	BTech::GamePhase currentPhase;
	GameSubPhase currentSubPhase;
	Player *currentPlayer;
	MechEntity *currentMech;
	Hex *currentHex;
	Hex *hexOnMouse;

	Player * nextPlayer() const;
	void endMove();
	bool playerEnded(Player *player) const;
	bool allPlayersEnded() const;
	bool gameOver();
	Player * getWinner() const;

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

	void chooseAction(const Action *action);

	void setCurrentMech(MechEntity *mech);
	void setCurrentHex(Hex *hex);
	void setCurrentPhase(BTech::GamePhase phase);
	void setCurrentSubPhase(GameSubPhase subPhase);
	GameSubPhase getCurrentSubPhase() const;
	void setCurrentPlayer(Player *player);
	void setCurrentAction(const Action *action);
	const Action * getCurrentAction() const;

	virtual void emitGameStarted() = 0;
	virtual void emitGameEnded() = 0;
	virtual void emitMechInfoNeeded(const MechEntity *mech) = 0;
	virtual void emitMechInfoNotNeeded() = 0;
	virtual void emitMechActionsNeeded(BTech::GamePhase phase) = 0;
	virtual void emitMechActionsNotNeeded() = 0;
	virtual void emitMechWalkRangeNeeded(const MovementObject &movement) = 0;
	virtual void emitMechShootRangeNeeded(const MechEntity *mech) = 0;
	virtual void emitMechRangesNotNeeded() = 0;
	virtual void emitPlayerTurn(const Player *player) = 0;
	virtual void emitHexesNeedClearing() = 0;
	virtual void emitHexesNeedUpdating() = 0;
	virtual void emitMessageSent(const QString &message, const QColor &color = DefaultMessageColor) = 0;

	virtual void clearMap();

	//TODO this should Not be here
	QHash <QString, QColor> playerNameToColor;

	static const QColor DefaultMessageColor;

private:
	QString description;
	QList <BTech::GameVersion> allowedVersions;

	void initPlayers();

	void resetCurrentValues();

	static const qint16 DEFAULT_HEX_WIDTH = 40;
	static const qint16 DEFAULT_HEX_HEIGHT = 40;
};

#endif // MAP_H
