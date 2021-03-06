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

#include "BTCommon/EnumHashFunctions.h"
#include "BTCommon/Map.h"
#include "BTMapEditor/MapPropertiesManager.h"

/**
 * \class MapPropertiesManager
 */

MapPropertiesManager::MapPropertiesManager(Map *map)
	: map(map)
{
	QLabel *playersLabel = new QLabel(BTech::Strings::LabelPlayers);

	playersComboBox = new QComboBox;
	connect(playersComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &MapPropertiesManager::onPlayerChosen);

	addPlayerButton = new QPushButton(BTech::Strings::ButtonAddNewPlayer);
	connect(addPlayerButton, &QPushButton::pressed, this, &MapPropertiesManager::addNewPlayer);
	addPlayerButton->setEnabled(false);
	removePlayerButton = new QPushButton(BTech::Strings::ButtonRemovePlayer);
	connect(removePlayerButton, &QPushButton::pressed, this, &MapPropertiesManager::removePlayer);

	QLabel *playerNameLabel = new QLabel(BTech::Strings::LabelName);
	QLabel *playerColorLabel = new QLabel(BTech::Strings::LabelColor);

	//TODO fix memleak
	playerColor = new ColorRect();
	playerName = new QLineEdit;
	playerDescription = new QTextEdit;
	playerDescription->setFixedHeight(200);
	playerNameLabel->setBuddy(playerName);
	playerColorLabel->setBuddy(playerColor);

	connect(playerColor, &ColorRect::clicked, this, &MapPropertiesManager::editPlayerColor);

	confirmSavePlayerButton = new QPushButton(BTech::Strings::ButtonSavePlayerDescription);
	confirmSavePlayerButton->setFixedWidth(200);
	connect(confirmSavePlayerButton, &QPushButton::pressed, this, &MapPropertiesManager::savePlayer);
	confirmSavePlayerButton->setEnabled(false);

	QLabel *mapDescriptionLabel = new QLabel(BTech::Strings::LabelMap);

	mapDescription = new QTextEdit;
	mapDescription->setEnabled(false);

	confirmSaveMapDescriptionButton = new QPushButton(BTech::Strings::ButtonSaveMapDescription);
	confirmSaveMapDescriptionButton->setFixedWidth(DEFAULT_WIDTH);
	connect(confirmSaveMapDescriptionButton, &QPushButton::pressed, this, &MapPropertiesManager::saveMapDescription);
	confirmSaveMapDescriptionButton->setEnabled(false);

	initVersionsButton();

	/* layout */

	QHBoxLayout *playerButtonsLayout = new QHBoxLayout;
	playerButtonsLayout->addWidget(addPlayerButton);
	playerButtonsLayout->addWidget(removePlayerButton);

	QHBoxLayout *playerNameLayout = new QHBoxLayout;
	playerNameLayout->addWidget(playerNameLabel);
	playerNameLayout->addSpacerItem(new QSpacerItem(BTech::SMALL_SPACER_SIZE, BTech::SYMBOLIC_SPACER_SIZE));
	playerNameLayout->addWidget(playerName);
	playerNameLayout->addSpacerItem(new QSpacerItem(BTech::SMALL_SPACER_SIZE, BTech::SYMBOLIC_SPACER_SIZE));
	playerNameLayout->addWidget(playerColorLabel);
	playerNameLayout->addSpacerItem(new QSpacerItem(BTech::SMALL_SPACER_SIZE, BTech::SYMBOLIC_SPACER_SIZE));
	playerNameLayout->addWidget(playerColor);

	QHBoxLayout *savePlayerButtonLayout = new QHBoxLayout;
	savePlayerButtonLayout->addSpacerItem(new QSpacerItem(BTech::ENORMOUS_SPACER_SIZE, BTech::SYMBOLIC_SPACER_SIZE));
	savePlayerButtonLayout->addWidget(confirmSavePlayerButton);

	QFrame *line = new QFrame;
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);

	QHBoxLayout *saveMapDescriptionButtonLayout = new QHBoxLayout;
	saveMapDescriptionButtonLayout->addSpacerItem(new QSpacerItem(BTech::ENORMOUS_SPACER_SIZE, BTech::SYMBOLIC_SPACER_SIZE));
	saveMapDescriptionButtonLayout->addWidget(confirmSaveMapDescriptionButton);

	QHBoxLayout *versionsButtonLayout = new QHBoxLayout;
	versionsButtonLayout->addSpacerItem(new QSpacerItem(BTech::ENORMOUS_SPACER_SIZE, BTech::SYMBOLIC_SPACER_SIZE));
	versionsButtonLayout->addWidget(versionsButton);


	QVBoxLayout *layout = new QVBoxLayout;
	layout->setAlignment(Qt::AlignTop);

	layout->addWidget(playersLabel);
	layout->addWidget(playersComboBox);
	layout->addItem(playerButtonsLayout);
	layout->addItem(playerNameLayout);
	layout->addWidget(playerDescription);
	layout->addItem(savePlayerButtonLayout);
	layout->addWidget(line);
	layout->addWidget(mapDescriptionLabel);
	layout->addWidget(mapDescription);
	layout->addItem(saveMapDescriptionButtonLayout);
	layout->addItem(versionsButtonLayout);

	setLayout(layout);
	refresh();
}

Player * MapPropertiesManager::getCurrentPlayer() const
{
	return getPlayer(playersComboBox->currentText());
}

void MapPropertiesManager::refresh()
{
	playersComboBox->clear();
	for (Player *player : map->getPlayers())
		playersComboBox->addItem(player->getName());
	if (getCurrentPlayer() != nullptr) {
		playerName->setText(getCurrentPlayer()->getName());
		playerDescription->setText(getCurrentPlayer()->getDescription());
		playerColor->setColor(getCurrentPlayer()->getColor());
	}

	mapDescription->setText(map->getDescription());

	bool enabled = !map->getPlayers().empty();
	playersComboBox->setEnabled(enabled);
	removePlayerButton->setEnabled(enabled);
	if (!enabled) {
		playerName->clear();
		playerDescription->clear();
	}
	playerName->setEnabled(enabled);
	playerDescription->setEnabled(enabled);
	confirmSavePlayerButton->setEnabled(enabled);
}

void MapPropertiesManager::onMapLoaded()
{
	mapDescription->setEnabled(true);
	confirmSaveMapDescriptionButton->setEnabled(true);
	addPlayerButton->setEnabled(true);
	confirmSavePlayerButton->setEnabled(true);
	versionsButton->setEnabled(true);
	refresh();
}

void MapPropertiesManager::setCurrentPlayer(Player *player)
{
	int playerIdx = playersComboBox->findText(player->getName());
	Q_ASSERT(playerIdx != -1);
	if (playersComboBox->currentIndex() != playerIdx) {
		playersComboBox->setCurrentIndex(playerIdx);
		emit playerChosen(player);
	}
}

void MapPropertiesManager::initVersionsButton()
{
	versionsButton = new QPushButton(BTech::Strings::ButtonVersions);
	versionsButton->setFixedWidth(BTech::NAME_LINE_WIDTH);
	connect(versionsButton, &QPushButton::pressed, this, &MapPropertiesManager::editVersions);
	versionsButton->setEnabled(false);
}

Player * MapPropertiesManager::getPlayer(const QString &name) const
{
	for (Player *player : map->getPlayers())
		if (player->getName() == name)
			return player;
	return nullptr;
}

void MapPropertiesManager::onPlayerChosen()
{
	playerName->setText(getCurrentPlayer()->getName());
	playerDescription->setText(getCurrentPlayer()->getDescription());
	playerColor->setColor(getCurrentPlayer()->getColor());
	emit playerChosen(getCurrentPlayer());
}

void MapPropertiesManager::addNewPlayer()
{
	if (map->getPlayers().size() == MAX_PLAYERS_SIZE)
		return;

	QList <QString> existingNames;
	for (Player *player : map->getPlayers())
		existingNames.append(player->getName());
	QString newName = BTech::General::indexString(BTech::Strings::UnnamedPlayer, existingNames);

	Player *player = new Player;
	player->setName(newName);

	QColor randomColor =
		QColor(BTech::randomInt(256),
		       BTech::randomInt(256),
		       BTech::randomInt(256));
	player->setColor(randomColor);

	map->getPlayers().append(player);
	refresh();
	emit playerAdded();
}

void MapPropertiesManager::removePlayer()
{
	emit playerNeedsRemoving(getCurrentPlayer());
	emit playerRemoved();
}

void MapPropertiesManager::savePlayer()
{
	getCurrentPlayer()->setDescription(playerDescription->document()->toPlainText());
	getCurrentPlayer()->setName(playerName->text());
	getCurrentPlayer()->setColor(playerColor->getColor());
	refresh();
	emit playerInfoChanged();
}

void MapPropertiesManager::editPlayerColor()
{
	QColorDialog dialog;
	playerColor->setColor(dialog.getColor());
}

void MapPropertiesManager::saveMapDescription()
{
	map->setDescription(mapDescription->document()->toPlainText());
	refresh();
}

void MapPropertiesManager::editVersions()
{
	GameVersionDialog dialog;
	for (BTech::GameVersion gameVersion : BTech::gameVersions)
		dialog.setChecked(gameVersion, map->getAllowedVersions().contains(gameVersion));
	if (dialog.exec())
		map->setAllowedVersions(dialog.getAllowedVersions());
}

/**
 * \class ColorRect
 */

ColorRect::ColorRect(const QColor &color)
{
	setAutoFillBackground(true);
	setColor(color);
	setText("      ");
}

QColor ColorRect::getColor() const
{
	return color;
}

void ColorRect::setColor(const QColor &color)
{
	this->color = color;
	QPalette palette;
	palette.setColor(QPalette::Background, color);
	setPalette(palette);
}

void ColorRect::mousePressEvent(QMouseEvent *event)
{
	emit clicked();
}

/**
 * \class GameVersionDialog
 */

GameVersionDialog::GameVersionDialog(QWidget *parent)
	: QDialog(parent)
{
	setFixedSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	QVBoxLayout *checkBoxLayout = new QVBoxLayout;
	checkBoxLayout->setAlignment(Qt::AlignTop);
	for (BTech::GameVersion gameVersion : BTech::gameVersions) {
		QCheckBox *newCheckBox = new QCheckBox(BTech::gameVersionStringChange[gameVersion]);
		newCheckBox->setChecked(true);
		checkBoxLayout->addWidget(newCheckBox);
		checkBox[gameVersion] = newCheckBox;
	}

	QVBoxLayout *layout = new QVBoxLayout;

	layout->addItem(checkBoxLayout);

	errorLabel = new QLabel(BTech::Strings::GameVersionDialogWarning);
	errorLabel->setWordWrap(true);
	errorLabel->setVisible(false);
	layout->addWidget(errorLabel);

	confirmButton = new QPushButton(BTech::Strings::ButtonConfirm);
	connect(confirmButton, &QPushButton::pressed, this, &GameVersionDialog::tryConfirm);
	layout->addWidget(confirmButton);

	setLayout(layout);
}

QList <BTech::GameVersion> GameVersionDialog::getAllowedVersions() const
{
	QList <BTech::GameVersion> list;
	for (BTech::GameVersion version : BTech::gameVersions)
		if (checkBox[version]->isChecked())
			list.append(version);
	return list;
}

void GameVersionDialog::setChecked(BTech::GameVersion version, bool check)
{
	checkBox[version]->setChecked(check);
}

bool GameVersionDialog::isChecked(BTech::GameVersion version) const
{
	return checkBox[version]->isChecked();
}

void GameVersionDialog::tryConfirm()
{
	bool checked = false;
	for (BTech::GameVersion version : BTech::gameVersions)
		checked |= checkBox[version]->isChecked();
	if (checked)
		QDialog::accept();
	else
		errorLabel->setVisible(true);
}
