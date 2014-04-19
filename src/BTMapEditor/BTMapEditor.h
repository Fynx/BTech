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

#ifndef BTMAP_EDITOR_H
#define BTMAP_EDITOR_H

#include <QtWidgets>
#include "BTCommon/BTMapManager.h"
#include "BTCommon/GraphicsMap.h"
#include "BTMapEditor/dialogs/MapEditorDialogs.h"
#include "BTMapEditor/ToolBar.h"

/**
 * \class BTMapEditor
 * BTMapEditor is a map editor for BTech.
 */

class BTMapEditor : public BTMapManager
{
Q_OBJECT;

public:
	BTMapEditor();
	~BTMapEditor();

private:
	QAction *menuNewMapAction;
	QAction *menuSaveMapAction;
	QAction *menuSaveAsMapAction;

	QMenu *editMenu;
	QAction *menuEditWeaponsAction;
	QAction *menuEditMechsAction;
	QAction *menuSaveDataAction;

	ToolBar *toolBar;

	void initInfoBar();
	void initMap();
	void initToolBar();
	void initMenu();
	void sortMenu();
	void initSystem();

	void readSettings();
	void writeSettings();

	bool saveMap(const QString &path);

	void reloadData();

	enum class Mode : quint8 {
		Click,
		Unit,
		Terrain
	};

	Mode currentMode;

	Player *currentPlayer;
	UID currentMech;
	BTech::Terrain currentTerrain;

private slots:
	void onNewMapAction();
	void onSaveMapAction();
	void onSaveAsMapAction();
	void onQuitAction();
	void onEditWeaponsAction();
	void onEditMechsAction();
	void onSaveData();

	void onChooseClickMode();
	void onChoosePlayer(Player *player);
	void onChooseUnit(UID unitUid);
	void onChooseTerrain(BTech::Terrain terrain);

	void onHexClicked(Hex *hex);

	void updatePlayers();
	void updateHexes();
};

#endif // BTMAP_EDITOR_H
