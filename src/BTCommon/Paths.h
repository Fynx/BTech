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

#ifndef PATHS_H
#define PATHS_H

#include <QtCore>

#include "BTCommon/Position.h"

namespace BTech {
	namespace Paths {
		extern const QString DATA_DIR_PATH;

		extern const QString DATA_PATH;
		extern const QString MAPS_PATH;
		extern const QString MECHS_PATH;
		extern const QString WEAPONS_PATH;

		namespace Tiles {
			extern const QString TILES_PATH;

			extern const QString CLEAR_TERRAIN_PATH;
			extern const QString LIGHT_WOODS_TERRAIN_PATH;
			extern const QString HEAVY_WOODS_TERRAIN_PATH;
			extern const QString ROUGH_TERRAIN_PATH;
			extern const QString WATER_TERRAIN_PATH;

			extern const BiHash <Terrain, QString> terrainTilePathMap;
		}
	}

	QString resolvePath(const QString &path);
}

#endif // PATHS_H
