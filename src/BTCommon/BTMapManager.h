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

#ifndef BTMAPMANAGER_H
#define BTMAPMANAGER_H

#include <QtWidgets>

class GraphicsMap;
class InfoBar;

class BTMapManager : public QMainWindow
{
Q_OBJECT;

public:
	BTMapManager();
	virtual ~BTMapManager() = 0;

protected:
	QMenu *fileMenu;
	QAction *menuLoadMapAction;
	QAction *menuQuitAction;

	QMenu *viewMenu;
	QAction *menuShowGridAction;
 	QAction *menuShowCoordsAction;
	QAction *menuShowInfoBarAction;

	GraphicsMap *map;
	InfoBar *infoBar;

	void initBaseFunctions();
	void initWindow();
	void initInfoBar();
	void initCentralWindow();
	void initMenu();
	void initEventsHandling();

	virtual void readSettings() = 0;
	virtual void writeSettings() = 0;

	void startMapManagement();

	void wheelEvent(QWheelEvent *event);

protected slots:
	virtual void onLoadMapAction();
	virtual void onQuitAction();

private slots:
	void onShowGridAction();
	void onShowCoordsAction();
	void onShowInfoBarAction();
};

#endif // BTMAPMANAGER_H
