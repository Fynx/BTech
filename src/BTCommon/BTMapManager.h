#ifndef BTMAPMANAGER_H
#define BTMAPMANAGER_H

#include <QtWidgets>
#include "BTCommon/GraphicsMap.h"

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
