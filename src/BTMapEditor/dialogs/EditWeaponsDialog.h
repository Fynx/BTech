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

#ifndef EDIT_WEAPONS_DIALOG_H
#define EDIT_WEAPONS_DIALOG_H

#include <QtWidgets>
#include "BTCommon/WeaponBase.h"
#include "BTMapEditor/ManagersUtils.h"

/**
 * \class ButtonGroup
 */

class ButtonGroup : public QWidget
{
Q_OBJECT;
Q_PROPERTY(int option READ getCurrentId WRITE check USER true);

public:
	ButtonGroup(QWidget *parent = nullptr);

	int getCurrentId() const;
	void check(int id);
	void addButton(QAbstractButton *button, int id);

signals:
	void clicked();
	void toggled(int);

private:
	QButtonGroup *group;

private slots:
	void emitToggled();
};

/**
 * \class EditWeaponsDialog
 */
class EditWeaponsDialog : public QDialog
{
Q_OBJECT;

public:
	EditWeaponsDialog(QWidget *parent = nullptr);

private:
	QLineEdit *nameEdit;

	QRadioButton *typeEnergy, *typeBallistic, *typeMissile;
	ButtonGroup *weaponTypeGroup;

	QSpinBox *minRange;
	QHash <BTech::Range, QSpinBox *> maxRange;
	QList <QSpinBox *> maxRangeList;
	QSpinBox *heat;
	QSpinBox *damage;
	QSpinBox *tonnage;
	QSpinBox *criticalSpaces;
	QSpinBox *ammoPerTon;
	QSpinBox *missilesPerShot;

	QListView *weaponList;

	QDialogButtonBox *buttonBox;
	QPushButton *addWeaponButton;
	QPushButton *closeButton;
	QPushButton *resetButton;
	QPushButton *removeWeaponButton;
	QPushButton *saveWeaponButton;

	QStatusBar *statusBar;

	QDataWidgetMapper *mapper;

	void initButtons();
	void initLayoutEditPart(QBoxLayout *layout);
	void initLayoutViewPart(QBoxLayout *layout);
	void initLayout();
	void initSpinBoxes();
	void initWidgetMapping();

	void adjustRangeThreshold(QSpinBox *shorter, QSpinBox *longer, bool up);

	static const int MAX_SHORT_RANGE_SPIN_BOX  = 0;
	static const int MAX_MEDIUM_RANGE_SPIN_BOX = 1;
	static const int MAX_LONG_RANGE_SPIN_BOX   = 2;

private slots:
	void addWeapon();
	void removeWeapon();
	void saveWeapon();

	void adjustRangeThresholds(quint8 spinBoxNumber);
	void adjustEditability(quint8 radioButtonNumber);
};

#endif // EDIT_WEAPONS_DIALOG_H
