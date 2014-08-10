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

#include "BTCommon/BTMapManager.h"
#include "BTCommon/DataManager.h"
#include "BTCommon/GraphicsMap.h"
#include "BTCommon/Hex.h"
#include "BTCommon/InfoBar.h"
#include "BTCommon/MechEntity.h"
#include "BTCommon/Paths.h"

BTMapManager::BTMapManager()
{
	initBaseFunctions();
	initData();
	initWindow();
	initInfoBar();
	initCentralWindow();
	initMenu();
	initEventsHandling();
	qDebug() << "Program functions initialised.\n";
}

BTMapManager::~BTMapManager()
{}

void BTMapManager::startMapManagement()
{
	infoBar->setVisible(true);
	infoBar->show();
	menuActionToggleCoordinates->setEnabled(true);
	menuActionToggleInfoBar->setEnabled(true);
	menuActionToggleGrid->setEnabled(true);
}

Map * BTMapManager::getMap()
{
	return map;
}

GraphicsMap * BTMapManager::getGraphicsMap()
{
	return graphicsMap;
}

InfoBar * BTMapManager::getInfoBar()
{
	return infoBar;
}

QString BTMapManager::getMapFileName() const
{
	return mapFileName;
}

void BTMapManager::setMapFileName(const QString &mapFileName)
{
	this->mapFileName = mapFileName;
}

void BTMapManager::onLoadMap()
{
	mapFileName =
		QFileDialog::getOpenFileName(
			this,
			BTech::Strings::DialogOpenFile,
			BTech::resolvePath(BTech::Paths::MAPS_PATH),
			BTech::Strings::DialogBTechMapFiles);
	if (!graphicsMap->loadMap(mapFileName)) {
		mapFileName = QString();
		return;
	}
	startMapManagement();
}

void BTMapManager::onQuit()
{
	close();
}

void BTMapManager::initBaseFunctions()
{
	BTech::General::startTime();
}

void BTMapManager::initCentralWindow()
{
	map = new Map;

	graphicsMap = new GraphicsMap(map);
	setCentralWidget(graphicsMap);
	graphicsMap->stackUnder(infoBar);

	connect(graphicsMap, &GraphicsMap::hexDisplayStarted, infoBar, &InfoBar::setHex);
	connect(graphicsMap, &GraphicsMap::hexDisplayQuit,    infoBar, &InfoBar::removeHex);
	connect(graphicsMap, &GraphicsMap::hexDisplayChanged, infoBar, &InfoBar::updateHexWindow);

	connect(map, &Map::mechInfoNeeded,    this, &BTMapManager::onMechInfoNeeded);
	connect(map, &Map::mechInfoNotNeeded, this, &BTMapManager::onMechInfoNotNeeded);
}

void BTMapManager::initData()
{
	if (!DataManager::loadFromFile(BTech::resolvePath(BTech::Paths::DATA_PATH)))
		qWarning() << "Load data failed!";
	else
		qDebug() << "Data loaded.";
}

void BTMapManager::initEventsHandling()
{
	setMouseTracking(true);
}

void BTMapManager::initInfoBar()
{
	infoBar = new InfoBar;
	infoBar->resize(0, height());
	addDockWidget(Qt::LeftDockWidgetArea, infoBar);
}

void BTMapManager::initMenu()
{
	fileMenu = menuBar()->addMenu(BTech::Strings::MenuFile);
	viewMenu = menuBar()->addMenu(BTech::Strings::MenuView);

	menuActionLoadMap           = new QAction(this);
	menuActionQuit              = new QAction(this);
	menuActionToggleGrid        = new QAction(this);
	menuActionToggleCoordinates = new QAction(this);
	menuActionToggleInfoBar     = new QAction(this);
	menuActionToggleFullscreen  = new QAction(this);

	menuActionLoadMap->setText(BTech::Strings::MenuActionLoadMap);
	menuActionQuit->setText(BTech::Strings::MenuActionQuit);
	menuActionToggleGrid->setText(BTech::Strings::MenuActionToggleGrid);
	menuActionToggleCoordinates->setText(BTech::Strings::MenuActionToggleCoordinates);
	menuActionToggleInfoBar->setText(BTech::Strings::MenuActionToggleInfoBar);
	menuActionToggleFullscreen->setText(BTech::Strings::MenuActionToggleFullscreen);

	menuActionLoadMap->setShortcut(QKeySequence::Open);
	menuActionQuit->setShortcut(QKeySequence::Quit);
	menuActionToggleFullscreen->setShortcut(Qt::Key_F12);

	connect(menuActionLoadMap, &QAction::triggered, this, &BTMapManager::onLoadMap);
	connect(menuActionQuit, &QAction::triggered, this, &BTMapManager::onQuit);
	connect(menuActionToggleGrid, &QAction::toggled, this, &BTMapManager::onToggleGrid);
	connect(menuActionToggleCoordinates, &QAction::toggled, this, &BTMapManager::onToggleCoordinates);
	connect(menuActionToggleInfoBar, &QAction::toggled, this, &BTMapManager::onToggleInfoBar);
	connect(menuActionToggleFullscreen, &QAction::toggled, this, &BTMapManager::onToggleFullscreen);

	fileMenu->addAction(menuActionLoadMap);
	fileMenu->addAction(menuActionQuit);
	viewMenu->addAction(menuActionToggleGrid);
	viewMenu->addAction(menuActionToggleCoordinates);
	viewMenu->addAction(menuActionToggleInfoBar);
	viewMenu->addAction(menuActionToggleFullscreen);

	menuActionToggleGrid->setEnabled(false);
	menuActionToggleGrid->setCheckable(true);

	menuActionToggleCoordinates->setEnabled(false);
	menuActionToggleCoordinates->setCheckable(true);

	menuActionToggleInfoBar->setCheckable(true);
	menuActionToggleInfoBar->setChecked(false);

	menuActionToggleFullscreen->setCheckable(true);
	//TODO
	menuActionToggleFullscreen->setChecked(false);
}

void BTMapManager::initWindow()
{
	statusBar()->hide();
	showMaximized();
}

void BTMapManager::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = qAbs(numDegrees / 15);
	if (numDegrees > 0)
		graphicsMap->incScale(numSteps);
	else
		graphicsMap->decScale(numSteps);
	event->accept();
}

void BTMapManager::onToggleCoordinates()
{
	graphicsMap->toggleCoordinates();
}

void BTMapManager::onToggleGrid()
{
	graphicsMap->toggleGrid();
}

void BTMapManager::onToggleInfoBar()
{
	if (!infoBar->isVisible())
		infoBar->show();
	else
		infoBar->hide();
}

void BTMapManager::onToggleFullscreen()
{
	if (menuActionToggleFullscreen->isChecked())
		showFullScreen();
	else
		showMaximized();
}

void BTMapManager::onMechInfoNeeded()
{
	infoBar->setMech(map->getCurrentUnit());
}

void BTMapManager::onMechInfoNotNeeded()
{
	infoBar->removeMech();
}
