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

#ifndef UNITS_MANAGER_H
#define UNITS_MANAGER_H

#include <QtWidgets>
#include "BTMapEditor/ManagersUtils.h"

/**
 * \class UnitsManager
 */
class UnitsManager : public QWidget
{
Q_OBJECT;

public:
	UnitsManager(QVector <Player *> &players);

	Player * getCurrentPlayer() const;

public slots:
	void setCurrentPlayer(Player *player);
	UID currentUnit() const;
	void onMapLoaded();
	void refresh();

signals:
	void unitChosen(UID unitUid);
	void playerChosen(Player *player);

private:
	const QVector <Player *> &players;

	void initMechList();

	QComboBox *playersComboBox;
	QVBoxLayout *mechListLayout;

	void updatePlayersComboBox();

	UID currentUnit_;

private slots:
	void onUnitChosen(const QString &unitName);
	void onPlayerChosen();
};

#endif // UNITS_MANAGER_H
