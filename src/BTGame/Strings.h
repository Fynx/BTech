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