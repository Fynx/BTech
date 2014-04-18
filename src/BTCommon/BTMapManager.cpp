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

#include "BTCommon/BTMapManager.h"
#include "BTCommon/DataManager.h"
#include "BTCommon/Paths.h"

BTMapManager::BTMapManager()
{
	initBaseFunctions();
	initWindow();
	initInfoBar();
	initCentralWindow();
	initMenu();
	initEventsHandling();
	qDebug() << "Program functions initialised.\n";
}

BTMapManager::~BTMapManager()
{}

void BTMapManager::initBaseFunctions()
{
	if (!DataManager::loadFromFile(BTech::Paths::DATA_PATH))
		qWarning() << "Load data failed!";
	else
		qDebug() << "Data loaded.";

	BTech::General::startTime();
}

void BTMapManager::initWindow()
{
	statusBar()->hide();
	resize(1024, 800);
	//showFullScreen();
	//showMaximized();
}

void BTMapManager::initInfoBar()
{
	infoBar = new InfoBar;
	infoBar->resize(0, height());
	addDockWidget(Qt::LeftDockWidgetArea, infoBar);
}

void BTMapManager::initCentralWindow()
{
	map = new GraphicsMap;
	setCentralWidget(map);
	map->stackUnder(infoBar);

	connect(map, &GraphicsMap::hexDisplayStarted, infoBar, &InfoBar::setHex);
	connect(map, &GraphicsMap::hexDisplayQuit,    infoBar, &InfoBar::removeHex);
	connect(map, &GraphicsMap::hexDisplayChanged, infoBar, &InfoBar::updateHexWindow);
	connect(map, &GraphicsMap::mechInfoNeeded,    infoBar, &InfoBar::setMech);
	connect(map, &GraphicsMap::mechInfoNotNeeded, infoBar, &InfoBar::removeMech);
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

void BTMapManager::initEventsHandling()
{
	setMouseTracking(true);
}

void BTMapManager::startMapManagement()
{
	infoBar->setVisible(true);
	infoBar->show();
	menuShowInfoBarAction->setEnabled(true);
	menuShowGridAction->setEnabled(true);
}

void BTMapManager::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = qAbs(numDegrees / 15);
	if (numDegrees > 0)
		map->incScale(numSteps);
	else
		map->decScale(numSteps);
	event->accept();
}

void BTMapManager::onLoadMapAction()
{
	if (!map->loadMap(QFileDialog::getOpenFileName(this,
	                                               BTech::Strings::DialogOpenFile,
	                                               BTech::Paths::MAPS_PATH,
	                                               BTech::Strings::DialogBTechMapFiles)))
		return;
	startMapManagement();
}

void BTMapManager::onQuitAction()
{
	close();
}

void BTMapManager::onShowGridAction()
{
	if (!map->isGridVisible())
		menuShowCoordsAction->setEnabled(true);
	else {
		menuShowCoordsAction->setChecked(false);
		menuShowCoordsAction->setEnabled(false);
	}
	map->toggleGrid();
}

void BTMapManager::onShowCoordsAction()
{
	map->toggleCoordinates();
}

void BTMapManager::onShowInfoBarAction()
{
	if (!infoBar->isVisible())
		infoBar->show();
	else
		infoBar->hide();
}
