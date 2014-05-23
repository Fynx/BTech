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

QString TileManager::fileName(UID uid)
{
	return instance().uidToFileInfo.value(uid, QFileInfo()).fileName();
}

void TileManager::loadTileDictionary(QDataStream &in, const QVector <Hex *> &hexes)
{
	TileManager &manager = instance();

	manager.coordToTile.clear();

	QHash <UID, QString> tileDict;
	QHash <UID, UID> uidRemap;
	in >> tileDict >> manager.coordToTile;
	for (QHash <UID, QString>::const_iterator i = tileDict.constBegin(); i != tileDict.constEnd(); ++i)
		uidRemap[i.key()] = registerTile(i.value());

	for (UID &uid : manager.coordToTile)
		uid = uidRemap.value(uid, EmptyUid);
}

UID TileManager::registerTile(const QFileInfo &tileFile)
{
	TileManager &manager = instance();

	const QString filePath = tileFile.filePath();
	if (manager.pathToUid.contains(filePath))
		return manager.pathToUid[filePath];

	UID newTileUid = manager.nextUid_++;
	while (manager.tileMap.contains(newTileUid))
		newTileUid = manager.nextUid_++;

	if (manager.loadTileImage(filePath, newTileUid))
		return newTileUid;
	else
		return EmptyUid;
}

void TileManager::saveTileDictionary(QDataStream &out, const QVector <Hex *> &hexes)
{
	TileManager &manager = instance();

	QHash <UID, QString> tileDict;
	manager.coordToTile.clear();
	for (const Hex *hex : hexes) {
		const GraphicsHex *graphicsHex = GraphicsFactory::get(hex);
		const Tile *tile = graphicsHex->getTile();
		if (tile != nullptr) {
			const UID tileUid = tile->uid();
			QPoint hexCoord = hex->getPoint();

			manager.coordToTile[qMakePair(hexCoord.x(), hexCoord.y())] = tileUid;
			tileDict[tileUid] = manager.uidToFileInfo[tileUid].filePath();
		}
	}

	out << tileDict << manager.coordToTile << manager.nextUid_;
}

const Tile * TileManager::tile(UID uid)
{
	return instance().tileMap.value(uid, nullptr);
}

const Tile * TileManager::tile(QPair <int, int> hexCoord)
{
	return tile(instance().coordToTile.value(hexCoord, EmptyUid));
}

TileManager::TileManager()
	: nextUid_(MinUid)
{
}

TileManager & TileManager::instance()
{
	static TileManager instance_;
	return instance_;
}

bool TileManager::loadTileImage(const QString &filePath, UID tileUid)
{
	QImage image(filePath);
	if (!image.isNull() && image.height() == Tile::TileSize && image.width() % Tile::TileSize == 0) {
		pathToUid[filePath] = tileUid;
		tileMap.insert(tileUid, new Tile(image, tileUid));
		uidToFileInfo[tileUid] = filePath;
		return true;
	}

	return false;
}
