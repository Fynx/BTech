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
		const QString WEAPONS_PATH  = "data/weapons/weapons.bin";
		const QString MECHS_PATH    = "data/mechs/mechs.bin";
		const QString MAPS_PATH     = "data/maps";
		const QString DATA_PATH     = "data/data.bin";
	}

	QString resolvePath(const QString &path)
	{
		static const QString BASE_PATH = "../";
		if (QDir::isAbsolutePath(path))
			return path;
		return QCoreApplication::applicationDirPath() + '/' + BASE_PATH + path;
	}
}