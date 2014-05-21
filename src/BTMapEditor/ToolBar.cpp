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

#include "BTCommon/Map.h"
#include "BTMapEditor/ClickModeManager.h"
#include "BTMapEditor/MapEditorStrings.h"
#include "BTMapEditor/MapPropertiesManager.h"
#include "BTMapEditor/TerrainManager.h"
#include "BTMapEditor/ToolBar.h"
#include "BTMapEditor/UnitsManager.h"

/**
 * \class ToolBar
 */

ToolBar::ToolBar(Map *map)
{
	this->map = map;
	initManagers(map->getPlayers(), map->getHexes(), map->getDescriptionReference(), map->getAllowedVersionsRef());
	initTabs();
	initWindow();
	initWidget();
	initLayout();
}

void ToolBar::setPlayers(QVector <Player *> &players)
{
	mapPropertiesManager->setPlayers(players);
}

void ToolBar::setHexes(QVector <Hex *> &hexes)
{}

void ToolBar::reloadData()
{
	// TODO called by the main window, when data has changed - meaning reload data about mechs and weapons in the graphical interface
}

void ToolBar::refresh()
{
	mapPropertiesManager->refresh();
	unitsManager->refresh();
}

void ToolBar::onMapLoaded()
{
	mapPropertiesManager->onMapLoaded();
	unitsManager->onMapLoaded();
}

void ToolBar::onHexClicked(Hex *hex)
{
	clickModeManager->onHexClicked(hex);
}

void ToolBar::initTabs()
{
	tabs = new QTabWidget;
	tabs->addTab(mapPropertiesManager, BTech::Strings::LabelMapProperties);
	tabs->addTab(unitsManager,         BTech::Strings::LabelUnits);
	tabs->addTab(terrainManager,       BTech::Strings::LabelTerrain);
	tabs->addTab(clickModeManager,     BTech::Strings::LabelClickMode);

	connect(tabs, &QTabWidget::currentChanged, this, &ToolBar::onTabChosen);
}

void ToolBar::initManagers(QVector <Player *> &players, QVector <Hex *> &hexes, QString &mapDescriptionRef, QList <BTech::GameVersion> &allowedVersions)
{
	mapPropertiesManager = new MapPropertiesManager(players, mapDescriptionRef, allowedVersions);
	connect(mapPropertiesManager, &MapPropertiesManager::playerChosen,        this, &ToolBar::playerChosen);
	connect(mapPropertiesManager, &MapPropertiesManager::playerNeedsRemoving, this, &ToolBar::removePlayer);
	connect(mapPropertiesManager, &MapPropertiesManager::playerAdded,         this, &ToolBar::refresh);
	connect(mapPropertiesManager, &MapPropertiesManager::playerRemoved,       this, &ToolBar::refresh);
	connect(mapPropertiesManager, &MapPropertiesManager::playerAdded,         this, &ToolBar::playersInfoChanged);
	connect(mapPropertiesManager, &MapPropertiesManager::playerRemoved,       this, &ToolBar::playersInfoChanged);
	connect(mapPropertiesManager, &MapPropertiesManager::playerInfoChanged,   this, &ToolBar::playersInfoChanged);

	unitsManager = new UnitsManager(players);
	connect(unitsManager,         &UnitsManager::playerChosen,              this,         &ToolBar::playerChosen);
	connect(unitsManager,         &UnitsManager::unitChosen,                this,         &ToolBar::unitChosen);
	connect(mapPropertiesManager, &MapPropertiesManager::playerInfoChanged, unitsManager, &UnitsManager::refresh);

	terrainManager = new TerrainManager;
	connect(terrainManager, &TerrainManager::terrainChosen, this, &ToolBar::terrainChosen);

	clickModeManager = new ClickModeManager;
	connect(clickModeManager, &ClickModeManager::hexInfoChanged,    this, &ToolBar::hexesInfoChanged);
	connect(clickModeManager, &ClickModeManager::mechNeedsRemoving, this, &ToolBar::removeMech);
}

void ToolBar::initWindow()
{
	setMouseTracking(true);
	setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	setBaseSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	setContentsMargins(0, 0, 0, 0);
}

void ToolBar::initWidget()
{
	setWidget(new QWidget(this));
	widget()->setMouseTracking(true);
}

void ToolBar::initLayout()
{
	QVBoxLayout *layout = new QVBoxLayout;

	layout->addWidget(tabs);

	widget()->setContentsMargins(0, 0, 0, 0);
	widget()->setLayout(layout);
}

void ToolBar::onTabChosen(int)
{
	if (tabs->currentWidget() == unitsManager)
		emit unitChosen(unitsManager->currentUnit());
	else if (tabs->currentWidget() == terrainManager)
		emit terrainChosen(terrainManager->currentTerrain());
	else
		emit clickModeChosen();
}

void ToolBar::removePlayer(Player *player)
{
	map->removePlayer(player);
}

void ToolBar::removeMech(MechEntity *mech)
{
	for (Player *player : map->getPlayers())
		for (MechEntity *cur : player->getMechs())
			if (mech == cur)
				player->removeMech(mech);
	emit hexesInfoChanged();
}
