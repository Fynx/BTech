#ifndef COLORS_H
#define COLORS_H

#include <QtWidgets>

namespace BTech {

	namespace Colors {
		const QColor CobaltGreen     = QColor( 61, 145,  64);
		const QColor CornflowerBlue  = QColor(100, 149, 237);
		const QColor Crimson         = QColor(220,  20,  60);
		const QColor DarkCyan        = QColor(  0, 100,   0);
		const QColor FloralWhite     = QColor(255, 250, 240);
		const QColor LightGolden     = QColor(250, 250, 210);
		const QColor LightYellow     = QColor(139, 139, 122);
		const QColor MediumTurquoise = QColor( 72, 209, 204);
		const QColor MidnightBlue    = QColor( 25,  25, 112);
		const QColor PeachPuff       = QColor(238, 203, 173);
	}

	QString colorToString(const QColor &color);
}

#endif // COLORS_H