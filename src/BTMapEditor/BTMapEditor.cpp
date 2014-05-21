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

#include "BTCommon/CommonSettings.h"
#include "BTCommon/DataManager.h"
#include "BTCommon/Paths.h"
#include "BTCommon/Settings.h"
#include "BTMapEditor/BTMapEditor.h"

BTMapEditor::BTMapEditor()
{
	initInfoBar();
	initMap();
	initToolBar();
	initMenu();
	initSystem();
	readSettings();
}

BTMapEditor::~BTMapEditor()
{
	writeSettings();
}

void BTMapEditor::initInfoBar()
{
	infoBar->setDisplayState(InfoBar::DisplayState::HexWindowOnly);
}

void BTMapEditor::initMap()
{
	connect(map, static_cast<void (GraphicsMap::*)(Hex *)>(&GraphicsMap::hexClicked), this, &BTMapEditor::onHexClicked);
}

void BTMapEditor::initToolBar()
{
	toolBar = new ToolBar(map);

	connect(map, &GraphicsMap::mapHasBeenLoaded, toolBar, &ToolBar::onMapLoaded);

	connect(toolBar, &ToolBar::clickModeChosen,    this, &BTMapEditor::onChooseClickMode);
	connect(toolBar, &ToolBar::playerChosen,       this, &BTMapEditor::onChoosePlayer);
	connect(toolBar, &ToolBar::unitChosen,         this, &BTMapEditor::onChooseUnit);
	connect(toolBar, &ToolBar::terrainChosen,      this, &BTMapEditor::onChooseTerrain);
	connect(toolBar, &ToolBar::playersInfoChanged, this, &BTMapEditor::updatePlayers);
	connect(toolBar, &ToolBar::hexesInfoChanged,   this, &BTMapEditor::updateHexes);

	addDockWidget(Qt::RightDockWidgetArea, toolBar);
}

void BTMapEditor::initMenu()
{
	editMenu = menuBar()->addMenu(BTech::Strings::MenuEdit);

	menuNewMapAction      = new QAction(this);
	menuSaveMapAction     = new QAction(this);
	menuSaveAsMapAction   = new QAction(this);
	menuEditWeaponsAction = new QAction(this);
	menuEditMechsAction   = new QAction(this);
	menuSaveDataAction    = new QAction(this);

	menuNewMapAction->setText(BTech::Strings::MenuActionNewMap);
	menuSaveMapAction->setText(BTech::Strings::MenuActionSaveMap);
	menuSaveAsMapAction->setText(BTech::Strings::MenuActionSaveAsMap);
	menuEditWeaponsAction->setText(BTech::Strings::MenuActionEditWeapons);
	menuEditMechsAction->setText(BTech::Strings::MenuActionEditMechs);
	menuSaveDataAction->setText(BTech::Strings::MenuActionSaveData);

	menuNewMapAction->setShortcut(QKeySequence::New);
	menuSaveMapAction->setShortcut(QKeySequence::Save);
	menuSaveAsMapAction->setShortcut(QKeySequence::SaveAs);

	connect(menuNewMapAction,      &QAction::triggered, this, &BTMapEditor::onNewMapAction);
	connect(menuSaveMapAction,     &QAction::triggered, this, &BTMapEditor::onSaveMapAction);
	connect(menuSaveAsMapAction,   &QAction::triggered, this, &BTMapEditor::onSaveAsMapAction);
	connect(menuEditWeaponsAction, &QAction::triggered, this, &BTMapEditor::onEditWeaponsAction);
	connect(menuEditMechsAction,   &QAction::triggered, this, &BTMapEditor::onEditMechsAction);
	connect(menuSaveDataAction,    &QAction::triggered, this, &BTMapEditor::onSaveData);

	fileMenu->addAction(menuNewMapAction);
	fileMenu->addAction(menuSaveMapAction);
	fileMenu->addAction(menuSaveAsMapAction);

	editMenu->addAction(menuEditWeaponsAction);
	editMenu->addAction(menuEditMechsAction);
	editMenu->addAction(menuSaveDataAction);

	sortMenu();
}

void BTMapEditor::sortMenu()
{
	fileMenu->clear();
	fileMenu->addAction(menuNewMapAction);
	fileMenu->addAction(menuLoadMapAction);
	fileMenu->addAction(menuSaveMapAction);
	fileMenu->addAction(menuSaveAsMapAction);
	fileMenu->addSeparator();
	fileMenu->addAction(menuQuitAction);
}

void BTMapEditor::initSystem()
{
	currentMech = EmptyUid;
	currentMode = Mode::Click;
	currentPlayer = nullptr;
	currentTerrain = BTech::Terrain::Clear;
}

void BTMapEditor::readSettings()
{
	restoreGeometry(Settings::value(BTech::Settings::Window::Geometry).toByteArray());
}

void BTMapEditor::writeSettings()
{
	Settings::setValue(BTech::Settings::Window::Geometry, saveGeometry());
	Settings::sync();
}

bool BTMapEditor::saveMap(const QString &path)
{
	if (path.isEmpty())
		return false;
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly))
		return false;
	QDataStream out(&file);
	map->setMapFileName(path);
	out << *map;
	file.close();

	return true;
}

void BTMapEditor::reloadData()
{
	toolBar->reloadData();
}

void BTMapEditor::onNewMapAction()
{
	NewMapDialog dialog(this);
	if (dialog.exec()) {
		map->createNewMap(dialog.getMapWidth(), dialog.getMapHeight());
		BTMapManager::startMapManagement();
	}
}

void BTMapEditor::onSaveMapAction()
{
	if (map->getMapFileName().isEmpty())
		return onSaveAsMapAction();
	saveMap(map->getMapFileName());
}

void BTMapEditor::onSaveAsMapAction()
{
	QString path = QFileDialog::getSaveFileName(this, BTech::Strings::DialogSaveMap, BTech::resolvePath(BTech::Paths::MAPS_PATH), BTech::Strings::DialogBTechMapFiles);
	if (saveMap(path))
		map->setMapFileName(path);
}

void BTMapEditor::onQuitAction()
{
	if (!WeaponModel::getInstance().isChanged() && !MechModel::getInstance().isChanged()) {
		BTMapManager::onQuitAction();
		return;
	}

	QMessageBox dialog(QMessageBox::NoIcon,
	                   BTech::Strings::DialogQuitWithoutSaving,
	                   BTech::Strings::QuitWithoutSavingWarning,
	                   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
	                   this);

	dialog.exec();

	switch (dialog.buttonRole(dialog.clickedButton())) {
		case QMessageBox::YesRole: onSaveData();
		case QMessageBox::NoRole:  BTMapManager::onQuitAction();
		default:                   return;
	}
}

void BTMapEditor::onEditWeaponsAction()
{
	EditWeaponsDialog dialog(this);
	dialog.exec();
	reloadData();
}

void BTMapEditor::onEditMechsAction()
{
	EditMechsDialog dialog(this);
	dialog.exec();
	reloadData();
}

void BTMapEditor::onSaveData()
{
	if (!DataManager::saveToFile())
		qWarning() << "Save data failed!";
	else
		qDebug() << "Data saved.";
}

void BTMapEditor::onChooseClickMode()
{
	currentMode = Mode::Click;
}

void BTMapEditor::onChoosePlayer(Player *player)
{
	currentPlayer = player;
}

void BTMapEditor::onChooseUnit(UID unitUid)
{
	currentMode = Mode::Unit;
	currentMech = unitUid;
}

void BTMapEditor::onChooseTerrain(BTech::Terrain terrain)
{
	currentMode = Mode::Terrain;
	currentTerrain = terrain;
}

void BTMapEditor::onHexClicked(Hex *hex)
{
	map->clearHexes();
	GraphicsFactory::get(hex)->setClicked(true);
	switch (currentMode) {
		case Mode::Unit:
			if (currentMech != EmptyUid)
				map->addMechToHex(new MechEntity(currentMech), hex, currentPlayer);
			break; // responsibility for the new allocated MechEntity goes to GraphicsMap.
		case Mode::Terrain:
			hex->setTerrain(currentTerrain);
			break;
		default:;
	}

	toolBar->onHexClicked(hex);
}

void BTMapEditor::updatePlayers()
{
	map->updatePlayers();
}

void BTMapEditor::updateHexes()
{
	map->updateHexes();
}
