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

#ifndef TERRAIN_MANAGER_H
#define TERRAIN_MANAGER_H

#include <QtWidgets>
#include "BTMapEditor/ManagersUtils.h"

class Tile;
class TerrainTileModel;

/**
 * \class TerrainTileView
 */
class TerrainTileView : public QListView {
Q_OBJECT;

public:
	TerrainTileView(TerrainTileModel *model, QWidget *parent = nullptr);

	const Tile * getCurrentTile() const;

private:
	TerrainTileModel *model;
};

/**
 * \class TerrainManager
 */
class TerrainManager : public QTabWidget
{
Q_OBJECT;

public:
	TerrainManager();

	BTech::Terrain getCurrentTerrain() const;
	const Tile * getCurrentTile() const;

private:
	void initTerrainList();

	BTech::Terrain currentTerrain;
	QHash <QWidget *, BTech::Terrain> widgetTerrainMap;

private slots:
	void onTerrainChosen();
};

#endif // TERRAIN_MANAGER_H
