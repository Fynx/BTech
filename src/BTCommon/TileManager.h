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

#include <QtCore>

class Hex;
class Tile;

inline uint qHash(const QPoint &p, uint seed = 0)
{
	return qHash(QPair <int, int>(p.x(), p.y()), seed);
}

class TileManager {

public:
	static const unsigned int FRAME_CHANGE_MSEC = 100;

	static unsigned int currentFrame();
	static QString fileName(const Tile *tile);
	static void loadTileDictionary(QDataStream &in, const QVector <Hex *> &hexes);
	static const Tile * registerTile(const QFileInfo &tileFile);
	static void saveTileDictionary(QDataStream &out, const QVector <Hex *> &hexes);
	static const Tile * tile(const Hex *hex);
	static const Tile * tile(QPoint hexCoord);

private:
	TileManager();
	TileManager(const TileManager &) = delete;
	void operator = (const TileManager &) = delete;

	static TileManager & getInstance();
	const Tile * loadTileImage(const QString &filePath);

	QHash <QString, const Tile *> pathToTile;
	QHash <const Tile *, QFileInfo> tileToFileInfo;
	QVector <Tile *> tiles;
	QHash <QPoint, const Tile *> coordToTile;
};

#endif
