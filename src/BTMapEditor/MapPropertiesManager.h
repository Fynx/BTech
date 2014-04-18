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

#ifndef MAP_PROPERTIES_MANAGER_H
#define MAP_PROPERTIES_MANAGER_H

#include <QtWidgets>
#include "BTCommon/Rules.h"
#include "BTMapEditor/ManagersUtils.h"

/**
 * \class MapPropertiesManager
 */
class MapPropertiesManager : public QWidget
{
Q_OBJECT;

public:
	MapPropertiesManager(QVector <Player *> &players, QString &mapDescription, QList <BTech::GameVersion> &allowedVersions);

	void setPlayers(QVector <Player *> &players);
	void refresh();

public slots:
	void onMapLoaded();

signals:
	void playerChosen(Player *player);
	void playerAdded();
	void playerInfoChanged();
	void playerNeedsRemoving(Player *player);
	void playerRemoved();

private:
	static const int MAX_PLAYERS_SIZE = 8;

	static const int DEFAULT_WIDTH = 200;

	QVector <Player *> &players;
	QString &mapDescriptionRef;
	QList <BTech::GameVersion> &allowedVersions;

	QComboBox *playersComboBox;
	QPushButton *addPlayerButton;
	QPushButton *removePlayerButton;
	QLineEdit *playerName;
	QTextEdit *playerDescription;
	QPushButton *confirmSavePlayerButton;

	QTextEdit *mapDescription;
	QPushButton *confirmSaveMapDescriptionButton;

	QPushButton *versionsButton;

	void initVersionsButton();

	Player * getPlayer(const QString &name) const;
	Player * getCurrentPlayer() const;

private slots:
	void onPlayerChosen();
	void addNewPlayer();
	void removePlayer();
	void savePlayer();
	void saveMapDescription();
	void editVersions();
};

/**
 * \class GameVersionDialog
 */

class GameVersionDialog : public QDialog
{
Q_OBJECT;

public:
	GameVersionDialog(QWidget *parent = nullptr);

	QList <BTech::GameVersion> getAllowedVersions() const;
	void setChecked(BTech::GameVersion version, bool check);
	bool isChecked(BTech::GameVersion version) const;

private:
	static const int DEFAULT_WIDTH  = 200;
	static const int DEFAULT_HEIGHT = 300;

	QHash <BTech::GameVersion, QCheckBox *> checkBox;
	QPushButton *confirmButton;
	QLabel *errorLabel;

private slots:
	void tryConfirm();
};

#endif // MAP_PROPERTIES_MANAGER_H
