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

#ifndef EDIT_MECH_STATS_WIDGET_H
#define EDIT_MECH_STATS_WIDGET_H

#include <QtWidgets>
#include "BTMapEditor/dialogs/AbstractEditMechsWidget.h"

/**
 * \class EditMechStatsWidget
 */
class EditMechStatsWidget : public AbstractEditMechsWidget
{
Q_OBJECT;

public:
	EditMechStatsWidget(QAbstractItemView *view, QStatusBar *statusBar);

	void save();

private:
	QLineEdit *type;
	QSpinBox *tonnage;
	QSpinBox *armorValue;
	QSpinBox *maxMovePoints;
	QSpinBox *maxJumpPoints;
	QSpinBox *baseFireRange;
	QSpinBox *heatSinksNumber;

	QHash <BTech::Range, QSpinBox *> rangeDamage;

	QAbstractItemView *view;

	QStatusBar *statusBar;

	void initBoxes();
	void initLayout();
	void initWidgetMapping();

private slots:
	void removeMech();
};

#endif // EDIT_MECH_STATS_WIDGET_H
