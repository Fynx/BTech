#ifndef BTMAP_EDITOR_H
#define BTMAP_EDITOR_H

#include <QtWidgets>
#include "BTCommon/BTMapManager.h"
#include "BTCommon/GraphicsMap.h"
#include "BTMapEditor/dialogs/MapEditorDialogs.h"
#include "BTMapEditor/ToolBar.h"

/**
 * \class BTMapEditor
 * BTMapEditor is a map editor for BTech.
 */

class BTMapEditor : public BTMapManager
{
Q_OBJECT;

public:
	BTMapEditor();
	~BTMapEditor();

private:
	QAction *menuNewMapAction;
	QAction *menuSaveMapAction;
	QAction *menuSaveAsMapAction;

	QMenu *editMenu;
	QAction *menuEditWeaponsAction;
	QAction *menuEditMechsAction;
	QAction *menuSaveDataAction;

	ToolBar *toolBar;

	void initInfoBar();
	void initMap();
	void initToolBar();
	void initMenu();
	void sortMenu();
	void initSystem();

	void readSettings();
	void writeSettings();

	bool saveMap(const QString &path);

	void reloadData();

	enum class Chosen {
		Nothing,
		Mech,
		Terrain
	};

	Chosen currentlyChosen;

	Player *currentPlayer;
	UID currentMech;
	BTech::Terrain currentTerrain;
	bool clickModeActive;

private slots:
	void onNewMapAction();
	void onSaveMapAction();
	void onSaveAsMapAction();
	void onQuitAction();
	void onEditWeaponsAction();
	void onEditMechsAction();
	void onSaveData();

	void onChoosePlayer(Player *player);
	void onChooseUnit(UID unitUid);
	void onChooseTerrain(BTech::Terrain terrain);
	void onChooseClickMode(bool is);

	void onHexClicked(Hex *hex);

	void updatePlayers();
	void updateHexes();
};

#endif // BTMAP_EDITOR_H
