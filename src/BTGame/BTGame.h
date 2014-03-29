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
#include "BTGame/Strings.h"
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
