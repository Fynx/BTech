/*
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

#include "BTCommon/Paths.h"
#include "BTCommon/TileManager.h"
#include "BTCommon/TerrainTileModel.h"

TerrainTileModel::TerrainTileModel(const QString &tileDir, QObject *parent)
	: QAbstractListModel(parent), tileDirPath(tileDir)
{
	loadTiles();
}

QVariant TerrainTileModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.column() != 0)
		return QVariant();

	switch (role) {
		case Qt::DisplayRole:    return tiles.at(index.row())->getFileName();
		case Qt::DecorationRole: return tiles.at(index.row())->getFrame(0);
	}

	return QVariant();
}

const Tile * TerrainTileModel::getTile(int idx) const
{
	return tiles[idx];
}

int TerrainTileModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	return tiles.count();
}

void TerrainTileModel::loadTiles()
{
	tiles.clear();

	QDir tileDir(BTech::resolvePath(tileDirPath));
	QFileInfoList imgFileList = tileDir.entryInfoList({"*.png"}, QDir::Files | QDir::Readable);
	for (QFileInfo imgFile : imgFileList) {
		const Tile *tile = TileManager::registerTile(imgFile);
		if (tile != nullptr)
			tiles.append(tile);
	}
}
