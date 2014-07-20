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
class Map;

class BTMapManager : public QMainWindow
{
Q_OBJECT;

public:
	BTMapManager();
	virtual ~BTMapManager() = 0;

protected:
	void startMapManagement();

	Map * getMap();
	GraphicsMap * getGraphicsMap();
	InfoBar * getInfoBar();

	QString getMapFileName() const;
	void setMapFileName(const QString &mapFileName);

	QMenu *fileMenu;
	QAction *menuLoadMapAction;
	QAction *menuQuitAction;

	QMenu *viewMenu;
	QAction *menuShowGridAction;
 	QAction *menuShowCoordsAction;
	QAction *menuShowInfoBarAction;

	virtual void readSettings() = 0;
	virtual void writeSettings() = 0;

protected slots:
	virtual void onLoadMapAction();
	virtual void onQuitAction();

private:
	void initBaseFunctions();
	void initCentralWindow();
	void initData();
	void initEventsHandling();
	void initInfoBar();
	void initMenu();
	void initWindow();

	void wheelEvent(QWheelEvent *event);

	Map *map;
	GraphicsMap *graphicsMap;
	InfoBar *infoBar;
	QString mapFileName;

private slots:
	void onShowCoordsAction();
	void onShowGridAction();
	void onShowInfoBarAction();

	void onMechInfoNeeded();
	void onMechInfoNotNeeded();
};

#endif // BTMAPMANAGER_H
