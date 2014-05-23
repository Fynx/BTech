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
	: QAbstractListModel(parent), tileDir_(tileDir)
{
	loadTiles();
}

QVariant TerrainTileModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.column() != 0)
		return QVariant();

	switch (role) {
		case Qt::DisplayRole:    return tiles_.at(index.row())->fileName();
		case Qt::DecorationRole: return tiles_.at(index.row())->frame(0);
	}

	return QVariant();
}

int TerrainTileModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	return tiles_.count();
}

void TerrainTileModel::loadTiles()
{
	tiles_.clear();

	QDir tileDir(BTech::resolvePath(tileDir_));
	QFileInfoList imgFileList = tileDir.entryInfoList({"*.png"}, QDir::Files | QDir::Readable);
	for (QFileInfo imgFile : imgFileList) {
		UID tileUid = TileManager::registerTile(imgFile);
		if (tileUid != EmptyUid)
			tiles_.append(TileManager::tile(tileUid));
	}
}
