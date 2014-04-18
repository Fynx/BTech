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

#ifndef CLICK_MODE_MANAGER_H
#define CLICK_MODE_MANAGER_H

#include <QtWidgets>
#include "BTMapEditor/ManagersUtils.h"
#include "BTMapEditor/Utils.h"

/**
 * \class DirectionStar
 */
class DirectionStar : public QWidget
{
Q_OBJECT;

public:
	DirectionStar();

	void setEnabled(bool enabled);
	void clear();
	void setDirection(Direction direction);

signals:
	void directionChosen(Direction direction);

private:
	ClickableLabel *directionLabel[Direction::NUMBER];

	void initLabels();

private slots:
	void onDirectionChosen(const QString &directionName);

};

/**
 * \class ClickModeManager
 */
class ClickModeManager : public QWidget
{
Q_OBJECT;

public:
	ClickModeManager();

public slots:
	void onHexClicked(Hex *hex);

signals:
	void mechTypeSelected(quint32 serialNumber);

	void hexInfoChanged();
	void mechInfoChanged();
	void mechNeedsRemoving(MechEntity *mech);
	void mechRemoved();

private:
	QComboBox *terrainComboBox;
	QSpinBox *heightSpinBox;
	QComboBox *unitTypeComboBox;
	DirectionStar *directionStar;
	QPushButton *removeUnitButton;

	void initBoxes();
	void initTerrainComboBox();
	void initHeightSpinBox();
	void initUnitTypeComboBox();

	BTech::Terrain getCurrentTerrain() const;
	QString getCurrentMechType() const;
	quint32 getCurrentMechTypeSerialNumber() const;

	Hex *currentHex;

	void refresh();

private slots:
	void onTerrainChosen();
	void changeHexHeight(int height);
	void onMechTypeSelected();
	void onMechDirectionChosen(Direction direction);
	void onRemoveUnit();
};

#endif // CLICK_MODE_MANAGER_H
