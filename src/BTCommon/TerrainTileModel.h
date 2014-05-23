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

#ifndef TERRAIN_TILE_MODEL_H
#define TERRAIN_TILE_MODEL_H

#include "BTCommon/Tile.h"

class TerrainTileModel : public QAbstractListModel {

public:
	TerrainTileModel(const QString &tileDir, QObject *parent = nullptr);

	QVariant data(const QModelIndex &index, int role) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
	void loadTiles();

	const QString tileDir_;
	QVector <const Tile *> tiles_;
};

#endif
