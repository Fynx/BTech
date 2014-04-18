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

#include <QtWidgets>
#include "BTMapEditor/BTMapEditor.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setOrganizationName("BTech");
	app.setApplicationName("BTMapEditor");
	BTMapEditor editor;
	editor.show();
	return app.exec();
}
