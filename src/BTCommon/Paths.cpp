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

#include "BTCommon/Paths.h"

namespace BTech {
	namespace Paths {
		const QString DATA_DIR_PATH = "data/";

		const QString DATA_PATH     = DATA_DIR_PATH + "data.bin";
		const QString MAPS_PATH     = DATA_DIR_PATH + "maps";
		const QString MECHS_PATH    = DATA_DIR_PATH + "mechs/mechs.bin";
		const QString WEAPONS_PATH  = DATA_DIR_PATH + "weapons/weapons.bin";

		namespace Tiles {
			const QString TILES_PATH                = DATA_DIR_PATH + "tiles/";

			const QString CLEAR_TERRAIN_PATH        = TILES_PATH + "clear/";
			const QString LIGHT_WOODS_TERRAIN_PATH  = TILES_PATH + "light_woods/";
			const QString HEAVY_WOODS_TERRAIN_PATH  = TILES_PATH + "heavy_woods/";
			const QString ROUGH_TERRAIN_PATH        = TILES_PATH + "rough/";
			const QString WATER_TERRAIN_PATH        = TILES_PATH + "water/";

			const BiHash <Terrain, QString> terrainTilePathMap {
				{Terrain::Clear,      CLEAR_TERRAIN_PATH},
				{Terrain::LightWoods, LIGHT_WOODS_TERRAIN_PATH},
				{Terrain::HeavyWoods, HEAVY_WOODS_TERRAIN_PATH},
				{Terrain::Rough,      ROUGH_TERRAIN_PATH},
				{Terrain::Water,      WATER_TERRAIN_PATH}
			};
		}
	}

	QString resolvePath(const QString &path)
	{
		static const QString BASE_PATH = "../";
		if (QDir::isAbsolutePath(path))
			return path;
		return QCoreApplication::applicationDirPath() + '/' + BASE_PATH + path;
	}
}
