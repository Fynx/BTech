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

#ifndef EDITMECHPARTSWIDGET_H
#define EDITMECHPARTSWIDGET_H

#include <QtWidgets>
#include "BTMapEditor/dialogs/AbstractEditMechsWidget.h"
#include "BTMapEditor/dialogs/ItemComboBox.h"

/**
 * \class EditMechPartsWidget
 */

class EditMechPartsWidget : public AbstractEditMechsWidget
{
Q_OBJECT;

public:
	EditMechPartsWidget(QAbstractItemView *view);

	void setCurrentIndex(const QModelIndex &index);
	void setModel(QAbstractItemModel *model);

private:
	QSpinBox *maxArmorValue;
	QSpinBox *maxInternalValue;

	QPushButton *addWeaponButton;
	QPushButton *removeWeaponButton;
	QPushButton *moveWeaponUpButton;
	QPushButton *moveWeaponDownButton;

	QAbstractItemView *view;

	QListView *attachedWeapons;
	QListView *allWeapons;

	void initBoxes();
	void initButtons();
	void initListView();
	void initLayout();
	void initWidgetMapping();

	QList <int> getSelectedIndexes() const;

private slots:
	void addWeapons();
	void removeWeapons();
	void moveWeaponsUp();
	void moveWeaponsDown();
};

#endif // EDITMECHPARTSWIDGET_H
