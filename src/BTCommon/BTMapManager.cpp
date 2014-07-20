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
	menuShowCoordsAction->setEnabled(true);
	menuShowInfoBarAction->setEnabled(true);
	menuShowGridAction->setEnabled(true);
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

void BTMapManager::onLoadMapAction()
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

void BTMapManager::onQuitAction()
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

	menuLoadMapAction     = new QAction(this);
	menuQuitAction        = new QAction(this);
	menuShowGridAction    = new QAction(this);
	menuShowCoordsAction  = new QAction(this);
	menuShowInfoBarAction = new QAction(this);

	menuLoadMapAction->setText(BTech::Strings::MenuActionLoadMap);
	menuQuitAction->setText(BTech::Strings::MenuActionQuit);
	menuShowGridAction->setText(BTech::Strings::MenuActionToggleGrid);
	menuShowCoordsAction->setText(BTech::Strings::MenuActionToggleCoordinates);
	menuShowInfoBarAction->setText(BTech::Strings::MenuActionToggleInfoBar);

	menuLoadMapAction->setShortcut(QKeySequence::Open);
	menuQuitAction->setShortcut(QKeySequence::Quit);

	connect(menuLoadMapAction,     &QAction::triggered, this, &BTMapManager::onLoadMapAction);
	connect(menuQuitAction,        &QAction::triggered, this, &BTMapManager::onQuitAction);
	connect(menuShowGridAction,    &QAction::toggled,   this, &BTMapManager::onShowGridAction);
	connect(menuShowCoordsAction,  &QAction::toggled,   this, &BTMapManager::onShowCoordsAction);
	connect(menuShowInfoBarAction, &QAction::toggled,   this, &BTMapManager::onShowInfoBarAction);

	fileMenu->addAction(menuLoadMapAction);
	fileMenu->addAction(menuQuitAction);
	viewMenu->addAction(menuShowGridAction);
	viewMenu->addAction(menuShowCoordsAction);
	viewMenu->addAction(menuShowInfoBarAction);

	menuShowGridAction->setEnabled(false);
	menuShowGridAction->setCheckable(true);

	menuShowCoordsAction->setEnabled(false);
	menuShowCoordsAction->setCheckable(true);

	menuShowInfoBarAction->setCheckable(true);
	menuShowInfoBarAction->setChecked(false);
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

void BTMapManager::onShowCoordsAction()
{
	graphicsMap->toggleCoordinates();
}

void BTMapManager::onShowGridAction()
{
	graphicsMap->toggleGrid();
}

void BTMapManager::onShowInfoBarAction()
{
	if (!infoBar->isVisible())
		infoBar->show();
	else
		infoBar->hide();
}

void BTMapManager::onMechInfoNeeded()
{
	infoBar->setMech(map->getCurrentUnit());
}

void BTMapManager::onMechInfoNotNeeded()
{
	infoBar->removeMech();
}
