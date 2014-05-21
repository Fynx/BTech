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

#ifndef BTGAME_H
#define BTGAME_H

#include <QtWidgets>
#include "BTCommon/BTMapManager.h"
#include "BTCommon/GraphicsFactory.h"
#include "BTCommon/GraphicsMap.h"
#include "BTCommon/Player.h"
#include "BTCommon/Rules.h"
#include "BTCommon/InfoBar.h"
#include "BTCommon/Settings.h"
#include "BTGame/LogWindow.h"
#include "BTGame/SideBar.h"

/**
 * \mainpage BTGame
 * \section Description
 * BTech is a turn-based strategic game of BattleMech combat in the thirty-first century.
 * This program implements rules of BattleTech 1 st. Edition Rulebook, more widely known
 * as BattleDroids.
 * BattleDroids can be divided into three different levels of complexity:
 * - Basic BattleDroids
 * - Advanced BattleDroids
 * - Expert BattleDroids.
 * BTech translates the world of huge, heavily-armores war machines, namely BattleMechs,
 * into the computer game.
 */

class BTGame : public BTMapManager
{
Q_OBJECT;

public:
	BTGame();
	~BTGame();

private:
	QAction *menuTriggerLogWindow;

	QMenu *gameMenu;
	QAction *menuStartGameAction;
	QAction *menuSetVersion;

	SideBar *sideBar;
	LogWindow *logWindow;

	static const int DEFAULT_SIDEBAR_WIDTH = 300;

	void initBaseFunctions();
	void initWindow();
	void initLogWindow();
	void initSideBar();
	void initCentralWindow();
	void initMenu();

	void readSettings();
	void writeSettings();

	void keyPressEvent(QKeyEvent *event);

private slots:
	void onTriggerLogWindow();
	void onLoadMapAction();
	void onStartGameAction();
	void onSetVersionAction();
	void onEndGame();

	void setActionsInSideBar();
	void messageFromMapToSideBar();
	void onExtensiveInfo();
};

/**
 * \class GameVersionChoiceDialog
 */

class GameVersionChoiceDialog : public QDialog
{
Q_OBJECT;

public:
	GameVersionChoiceDialog(const QList <BTech::GameVersion> &allowedVersions);

	BTech::GameVersion getVersion() const;

private:
	QButtonGroup *group;
	QPushButton *confirmButton;
};

#endif // BTGAME_H
