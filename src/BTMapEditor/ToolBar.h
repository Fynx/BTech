#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QtWidgets>
#include "BTCommon/Map.h"
#include "BTMapEditor/Strings.h"
#include "BTMapEditor/ClickModeManager.h"
#include "BTMapEditor/MapPropertiesManager.h"
#include "BTMapEditor/TerrainManager.h"
#include "BTMapEditor/UnitsManager.h"

/**
 * \class ToolBar
 */
class ToolBar : public QDockWidget
{
Q_OBJECT;

public:
	ToolBar(Map *map);

	void setPlayers(QVector <Player *> &players);
	void setHexes(QVector <Hex *> &hexes);

	void reloadData();

public slots:
	void refresh();
	void onMapLoaded();
	void onHexClicked(Hex *hex);

signals:
	void clickModeActivated(bool is);

	void playerChosen(Player *player);
	void unitChosen(quint32 unitSerialNumber);
	void terrainChosen(BTech::Terrain terrain);

	void playersInfoChanged();
	void hexesInfoChanged();

private:
	QTabWidget *tabs;
	MapPropertiesManager *mapPropertiesManager;
	UnitsManager *unitsManager;
	TerrainManager *terrainManager;
	ClickModeManager *clickModeManager;

	static const int MAP_PROPERTIES_OVERLAP_INDEX = 0;
	static const int UNITS_OVERLAP_INDEX = 1;
	static const int TERRAIN_OVERLAP_INDEX = 2;
	static const int CLICK_MODE_OVERLAP_INDEX = 3;

	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 800;

	void initTabs();
	void initManagers(QVector <Player *> &players, QVector <Hex *> &hexes, QString &mapDescriptionRef, QList <BTech::GameVersion> &allowedVersions);
	void initWindow();
	void initWidget();
	void initLayout();

	Map *map;

private slots:
	void onTabChosen(int number);

	void removePlayer(Player *player);
	void removeMech(MechEntity *mech);
};

#endif // TOOLBAR_H
