/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
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

#ifndef BTGAME_STRINGS_H
#define BTGAME_STRINGS_H

#include <QtCore>

namespace BTech {
	namespace Strings {
		const QString MenuGame = QObject::tr("&Game");

		const QString ActionStartGame        = QObject::tr("Start game");
		const QString ActionSetVersion       = QObject::tr("Set version");
		const QString ActionTriggerLogWindow = QObject::tr("Trigger log window");

		const QString ButtonConfirm = QObject::tr("Confirm");
	}

	namespace HTML {
		const QString FontColor = QObject::tr("<FONT COLOR=%1>%2</FONT>");
		const QString NewLine   = QObject::tr("<br>");
	}
}

#endif // BTGAME_STRINGS_H
