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

