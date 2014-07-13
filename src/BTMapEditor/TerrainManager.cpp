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

#include "BTMapEditor/TerrainManager.h"
#include "BTCommon/Paths.h"
#include "BTCommon/TerrainTileModel.h"

/**
 * \class TerrainTileView
 */

TerrainTileView::TerrainTileView(TerrainTileModel *model, QWidget *parent)
	: QListView(parent), model_(model)
{
	setModel(model_);
}

const Tile * TerrainTileView::currentTile() const
{
	if (currentIndex() == QModelIndex())
		return nullptr;
	return model_->getTile(currentIndex().row());
}

/**
 * \class TerrainManager
 */

TerrainManager::TerrainManager()
{
	initTerrainList();
}

BTech::Terrain TerrainManager::currentTerrain() const
{
	return currentTerrain_;
}

const Tile * TerrainManager::currentTile() const
{
	TerrainTileView *tileView = qobject_cast<TerrainTileView *>(currentWidget());
	Q_ASSERT(tileView != nullptr);
	return tileView->currentTile();
}

void TerrainManager::initTerrainList()
{
	for (BTech::Terrain terrain : BTech::terrainTypes) {
		QString terrainName = BTech::terrainStringChange[terrain];
		TerrainTileModel *tileModel = new TerrainTileModel(BTech::resolvePath(BTech::Paths::Tiles::terrainTilePathMap[terrain]));
		TerrainTileView *tileView = new TerrainTileView(tileModel);
		addTab(tileView, terrainName);
		widgetTerrainMap[tileView] = terrain;
	}

	connect(this, &QTabWidget::currentChanged, this, &TerrainManager::onTerrainChosen);
	onTerrainChosen();
}

void TerrainManager::onTerrainChosen()
{
	currentTerrain_ = widgetTerrainMap[currentWidget()];
}
