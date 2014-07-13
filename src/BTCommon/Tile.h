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

#ifndef TILE_H
#define TILE_H

#include <QtGui>

#include "BTCommon/Uid.h"

class Tile {

public:
	static const int TileSize = 64;

	Tile() = default;
	Tile(QImage image);

	QImage getCurrentFrame() const;
	QString getFileName() const;
	QImage getFrame(unsigned int frame) const;
	int getFrameCount() const;
	UID getUid() const;

private:
	QVector <QImage> frames;
};

#endif
