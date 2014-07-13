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

#include <QtGui>

#include "BTCommon/GraphicsFactory.h"
#include "BTCommon/Tile.h"
#include "BTCommon/TileManager.h"

unsigned int TileManager::currentFrame()
{
	return 0;
}

QString TileManager::fileName(const Tile *tile)
{
	return instance().tileToFileInfo.value(tile, QFileInfo()).fileName();
}

void TileManager::loadTileDictionary(QDataStream &in, const QVector <Hex *> &hexes)
{
	TileManager &manager = instance();

	QHash <UID, QString> tileDict;
	QHash <UID, const Tile *> idToTile;
	QHash <QPoint, UID> coordToTileUid;
	in >> tileDict >> coordToTileUid;

	for (QHash <UID, QString>::const_iterator i = tileDict.constBegin(); i != tileDict.constEnd(); ++i) {
		const Tile *tile = registerTile(i.value());
		if (tile != nullptr)
			idToTile[i.key()] = tile;
	}

	manager.coordToTile.clear();
	for (QHash <QPoint, UID>::const_iterator i = coordToTileUid.constBegin(); i != coordToTileUid.constEnd(); ++i)
		manager.coordToTile[i.key()] = idToTile[i.value()];
}

const Tile * TileManager::registerTile(const QFileInfo &tileFile)
{
	TileManager &manager = instance();

	const QString filePath = tileFile.filePath();
	if (manager.pathToTile.contains(filePath))
		return manager.pathToTile[filePath];

	return manager.loadTileImage(filePath);
}

void TileManager::saveTileDictionary(QDataStream &out, const QVector <Hex *> &hexes)
{
	TileManager &manager = instance();

	UID tileCnt = EmptyUid;
	QHash <const Tile *, UID> tileToId;
	QHash <UID, QString> tileDict;

	manager.coordToTile.clear();
	for (const Hex *hex : hexes) {
		const GraphicsHex *graphicsHex = GraphicsFactory::get(hex);
		const Tile *tile = graphicsHex->getTile();
		if (tile != nullptr) {
			QPoint hexCoord = hex->getPoint();
			UID tileUid;
			if (tileToId.contains(tile)) {
				tileUid = tileToId[tile];
			} else {
				tileUid = ++tileCnt;
				tileToId[tile] = tileUid;
			}

			manager.coordToTile[hexCoord] = tile;
			tileDict[tileUid] = manager.tileToFileInfo[tile].filePath();
		}
	}

	QHash <QPoint, UID> coordToTileUid;
	for (QHash <QPoint, const Tile *>::const_iterator i = manager.coordToTile.constBegin(); i != manager.coordToTile.constEnd(); ++i)
		coordToTileUid[i.key()] = tileToId[i.value()];
	out << tileDict << coordToTileUid;
}

const Tile * TileManager::tile(const Hex *hex)
{
	return instance().tile(hex->getPoint());
}

const Tile * TileManager::tile(QPoint hexCoord)
{
	return instance().coordToTile.value(hexCoord, nullptr);
}

TileManager::TileManager()
{
}

TileManager & TileManager::instance()
{
	static TileManager instance_;
	return instance_;
}

const Tile * TileManager::loadTileImage(const QString &filePath)
{
	QImage image(filePath);
	if (!image.isNull() && image.height() == Tile::TileSize && image.width() % Tile::TileSize == 0) {
		Tile *result = new Tile(image);
		tiles.append(result);
		pathToTile[filePath] = result;
		tileToFileInfo[result] = filePath;
		return result;
	}

	return nullptr;
}
