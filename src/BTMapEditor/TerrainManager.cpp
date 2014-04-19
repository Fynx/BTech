/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
Copyright (C) 2014 by Bartosz Szreder <szreder [at] mimuw [dot] edu [dot] pl>
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

BTech::Terrain TerrainManager::currentTerrain() const
{
	return currentTerrain_;
}

void TerrainManager::initTerrainList()
{
	QButtonGroup *terrainGroup = new QButtonGroup(this);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setAlignment(Qt::AlignTop);
	for (BTech::Terrain terrain : BTech::terrainTypes) {
		QString terrainName = BTech::terrainStringChange[terrain];
		QRadioButton *terrainButton = new QRadioButton(terrainName);
		terrainGroup->addButton(terrainButton);
		layout->addWidget(terrainButton);
	}
	setLayout(layout);
	connect(terrainGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked), this, &TerrainManager::onTerrainChosen);

	QAbstractButton *checkedButton = terrainGroup->buttons()[0];
	checkedButton->setChecked(true);
	onTerrainChosen(checkedButton);
}

void TerrainManager::onTerrainChosen(QAbstractButton *button)
{
	currentTerrain_ = BTech::terrainStringChange[button->text()];
	emit terrainChosen(currentTerrain_);
}
