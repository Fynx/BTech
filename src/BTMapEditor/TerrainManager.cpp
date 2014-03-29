#include "BTMapEditor/TerrainManager.h"

/**
 * \class TerrainManager
 */

TerrainManager::TerrainManager()
{
	initTerrainList();
}

void TerrainManager::initTerrainList()
{
	// TODO // I suppose this should be contained in a file

	QSignalMapper *mapper = new QSignalMapper(this);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setAlignment(Qt::AlignTop);
	for (BTech::Terrain terrain : BTech::terrainTypes) {
		QString terrainName = BTech::terrainStringChange[terrain];
		ClickableLabel *label = new ClickableLabel(terrainName);
		connect(label, &ClickableLabel::clicked, mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
		mapper->setMapping(label, terrainName);
		layout->addSpacerItem(new QSpacerItem(BTech::LITTLE_SPACER_SIZE, BTech::SMALL_SPACER_SIZE));
		layout->addWidget(label);
	}
	setLayout(layout);
	connect(mapper, static_cast<void (QSignalMapper::*)(const QString &)>(&QSignalMapper::mapped), this, &TerrainManager::onTerrainChosen);
}

void TerrainManager::onTerrainChosen(const QString &terrainName)
{
	emit terrainChosen(BTech::terrainStringChange[terrainName]);
}

