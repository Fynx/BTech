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

#ifndef EDIT_WEAPONS_WIDGET_H
#define EDIT_WEAPONS_WIDGET_H

#include <QtWidgets>
#include "BTMapEditor/dialogs/AbstractEditMechsWidget.h"

class EditWeaponsWidget : public AbstractEditMechsWidget
{
Q_OBJECT;

public:
	EditWeaponsWidget(QAbstractItemView *view);

private:
	QAbstractItemView *view;

	/* TODO turn it into a read-only data display
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
	*/

	void initLayout();
	void initWidgetMapping();
};

#endif // EDIT_WEAPONS_WIDGET_H