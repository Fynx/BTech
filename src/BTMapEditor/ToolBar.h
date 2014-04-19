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

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QtWidgets>
#include "BTCommon/Map.h"
#include "BTMapEditor/Strings.h"
#include "BTMapEditor/ClickModeManager.h"
#include "BTMapEditor/MapPropertiesManager.h"
#include "BTMapEditor/TerrainManager.h"
#include "BTMapEditor/UnitsManager.h"

/**
 * \class ToolBar
 */
class ToolBar : public QDockWidget
{
Q_OBJECT;

public:
	ToolBar(Map *map);

	void setPlayers(QVector <Player *> &players);
	void setHexes(QVector <Hex *> &hexes);

	void reloadData();

public slots:
	void refresh();
	void onMapLoaded();
	void onHexClicked(Hex *hex);

signals:
	void clickModeChosen();
	void playerChosen(Player *player);
	void unitChosen(quint32 unitSerialNumber);
	void terrainChosen(BTech::Terrain terrain);

	void playersInfoChanged();
	void hexesInfoChanged();

private:
	QTabWidget *tabs;
	MapPropertiesManager *mapPropertiesManager;
	UnitsManager *unitsManager;
	TerrainManager *terrainManager;
	ClickModeManager *clickModeManager;

	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 800;

	void initTabs();
	void initManagers(QVector <Player *> &players, QVector <Hex *> &hexes, QString &mapDescriptionRef, QList <BTech::GameVersion> &allowedVersions);
	void initWindow();
	void initWidget();
	void initLayout();

	Map *map;

private slots:
	void onTabChosen(int number);

	void removePlayer(Player *player);
	void removeMech(MechEntity *mech);
};

#endif // TOOLBAR_H
