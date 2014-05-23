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

#ifndef TILE_MANAGER_H
#define TILE_MANAGER_H

#include "BTCommon/Uid.h"

class Hex;
class Tile;

class TileManager {

public:
	static const unsigned int FRAME_CHANGE_MSEC = 100;

	static unsigned int currentFrame();
	static QString fileName(UID uid);
	static void loadTileDictionary(QDataStream &in, const QVector <Hex *> &hexes);
	static UID registerTile(const QFileInfo &tileFile);
	static void saveTileDictionary(QDataStream &out, const QVector <Hex *> &hexes);
	static const Tile * tile(UID uid);
	static const Tile * tile(QPair <int, int> hexCoord);

private:
	TileManager();
	TileManager(const TileManager &) = delete;
	void operator = (const TileManager &) = delete;

	static TileManager & instance();
	bool loadTileImage(const QString &filePath, UID tileUid);

	UID nextUid_; //TODO use class Serial instead
	QHash <QString, UID> pathToUid;
	QHash <UID, QFileInfo> uidToFileInfo;
	QHash <UID, Tile *> tileMap;
	QHash <QPair <int, int>, UID> coordToTile;
};

#endif
