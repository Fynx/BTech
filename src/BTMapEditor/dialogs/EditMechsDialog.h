/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
Copyright (C) 2014 by Krzysztof Adamek <squadack [at] students [dot] mimuw [dot] edu [dot] pl>
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

#ifndef EDITMECHSDIALOG_H
#define EDITMECHSDIALOG_H

#include <QtWidgets>
#include "BTCommon/Models/MechModel.h"
#include "BTMapEditor/dialogs/AbstractEditMechsWidget.h"
#include "BTMapEditor/dialogs/EditMechPartsWidget.h"
#include "BTMapEditor/dialogs/EditMechStatsWidget.h"
#include "BTMapEditor/dialogs/EditWeaponsWidget.h"
#include "BTMapEditor/dialogs/ItemComboBox.h"

/**
 * \class MechTreeProxyModel
 */

class MechTreeProxyModel : public QIdentityProxyModel
{
public:
	MechTreeProxyModel(QObject *parent = nullptr);

	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
};

/**
 * \class EditMechsDialog
 */
class EditMechsDialog : public QDialog
{
Q_OBJECT;

public:
	EditMechsDialog(QWidget *parent = nullptr);

signals:
	void mechChosen();

private:
	void initWidgets();
	void initButtons();
	void initLayout();

	QPushButton *addMechButton;
	QPushButton *removeMechButton;
	QPushButton *submitButton;
	QPushButton *discardButton;
	QPushButton *confirmButton;
	QDialogButtonBox *mainButtonGroup;

	AbstractEditMechsWidget *topLevelPlaceholder;
	EditMechPartsWidget *editMechPartsWidget;
	EditMechStatsWidget *editMechStatsWidget;
	EditWeaponsWidget *editWeaponsWidget;
	QStackedWidget *stackedWidget;

	QStatusBar *statusBar;

	QTreeView *view;

	static const int ROOT_WIDGET            = 0;
	static const int EDIT_MECH_STATS_WIDGET = 1;
	static const int EDIT_MECH_PARTS_WIDGET = 2;
	static const int EDIT_WEAPONS_WIDGET    = 3;

	const QHash <MechModel::Level, AbstractEditMechsWidget **> levelToWidget;

private slots:
	void addMech();
	void removeMech();
	void save();
	void discard();

	void adjustStackedWidget();
};

// const QHash <MechModel::Level, AbstractEditMechsWidget **> EditMechsDialog::levelToWidget

#endif // EDITMECHSDIALOG_H
