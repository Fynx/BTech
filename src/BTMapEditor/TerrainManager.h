#ifndef TERRAIN_MANAGER_H
#define TERRAIN_MANAGER_H

#include <QtWidgets>
#include "BTMapEditor/ManagersUtils.h"

/**
 * \class TerrainManager
 */
class TerrainManager : public QWidget
{
Q_OBJECT;

public:
	TerrainManager();

signals:
	void terrainChosen(BTech::Terrain terrain);

private:
	void initTerrainList();

private slots:
	void onTerrainChosen(const QString &terrainName);
};

#endif // TERRAIN_MANAGER_H
