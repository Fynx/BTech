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

#include "BTMapEditor/UnitsManager.h"

/**
 * \class UnitsManager
 */

UnitsManager::UnitsManager(QVector <Player *> &players)
	: players(players), currentUnit_(EmptyUid)
{
	initMechList();

	playersComboBox = new QComboBox;
	connect(playersComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &UnitsManager::onPlayerChosen);
	updatePlayersComboBox();

	/* layout */

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setAlignment(Qt::AlignTop);

	layout->addWidget(playersComboBox);
	layout->addItem(mechListLayout);

	setLayout(layout);
}

Player * UnitsManager::getCurrentPlayer() const
{
	for (Player *player : players)
		if (player->getName() == playersComboBox->currentText())
			return player;
	return nullptr;
}

UID UnitsManager::currentUnit() const
{
	return currentUnit_;
}

void UnitsManager::onMapLoaded()
{
	updatePlayersComboBox();
}

void UnitsManager::refresh()
{
	updatePlayersComboBox();
}

void UnitsManager::initMechList()
{
	QSignalMapper *mapper = new QSignalMapper(this);
	QString mechName;
	mechListLayout = new QVBoxLayout;
	mechListLayout->setAlignment(Qt::AlignTop);
	for (MechBase *mech : MechModel::getMechs()) {
		mechName = static_cast<QString>(*mech);
		if (mechName.length() > 0) {
			//TODO change this ugly stupid label to the mech icon
			ClickableLabel *label = new ClickableLabel(mechName);
			connect(label, &ClickableLabel::clicked, mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
			mapper->setMapping(label, mechName);
			mechListLayout->addSpacerItem(new QSpacerItem(BTech::LITTLE_SPACER_SIZE, BTech::SMALL_SPACER_SIZE));
			mechListLayout->addWidget(label);
		}
	}
	connect(mapper, static_cast<void (QSignalMapper::*)(const QString &)>(&QSignalMapper::mapped), this, &UnitsManager::onUnitChosen);
}

void UnitsManager::updatePlayersComboBox()
{
	playersComboBox->clear();
	for (Player *player : players)
		playersComboBox->addItem(player->getName());
	playersComboBox->setEnabled(!players.isEmpty());
}

void UnitsManager::onUnitChosen(const QString &unitName)
{
	if (playersComboBox->isEnabled()) {
		currentUnit_ = MechModel::getMech(unitName)->getUid();
		emit playerChosen(getCurrentPlayer());
		emit unitChosen(currentUnit_);
	}
}

void UnitsManager::onPlayerChosen()
{
	emit playerChosen(getCurrentPlayer());
}
