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