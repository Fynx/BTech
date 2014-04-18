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

#include "BTCommon/EnumSerialization.h"
#include "BTMapEditor/dialogs/EditMechsDialog.h"

static const int StatusBarMessageTimeoutMs = 5000;

/**
 * \class MechTreeProxyModel
 */

MechTreeProxyModel::MechTreeProxyModel(QObject *parent)
	: QIdentityProxyModel(parent)
{
	setSourceModel(&MechModel::getInstance());
}

int MechTreeProxyModel::columnCount(const QModelIndex &parent) const
{
	return qMin(1, sourceModel()->columnCount(parent));
}

QVariant MechTreeProxyModel::data(const QModelIndex &index, int role) const
{
	if (index.column() != 0)
		return QVariant();
	return sourceModel()->data(index, role);
}

/**
 * \class EditMechsDialog
 */

EditMechsDialog::EditMechsDialog(QWidget *parent)
	: QDialog(parent),
	  levelToWidget( {
		{ MechModel::Level::Root,      reinterpret_cast<AbstractEditMechsWidget **>(&topLevelPlaceholder) },
		{ MechModel::Level::Mech,      reinterpret_cast<AbstractEditMechsWidget **>(&editMechStatsWidget) },
		{ MechModel::Level::MechPart,  reinterpret_cast<AbstractEditMechsWidget **>(&editMechPartsWidget) },
		{ MechModel::Level::Equipment, reinterpret_cast<AbstractEditMechsWidget **>(&editWeaponsWidget)   },
	} )
{
	setWindowTitle(BTech::Strings::EditMechsDialog);

	initButtons();
	initWidgets();
	initLayout();
}

void EditMechsDialog::initWidgets()
{
	MechTreeProxyModel *proxyModel = new MechTreeProxyModel(this);
	view = new QTreeView(this);
	view->setModel(proxyModel);
	view->setHeaderHidden(true);
	view->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &EditMechsDialog::adjustStackedWidget);

	statusBar = new QStatusBar;

	topLevelPlaceholder = new AbstractEditMechsWidget();
	editMechStatsWidget = new EditMechStatsWidget(view, statusBar);
	editMechPartsWidget = new EditMechPartsWidget(view);
	editMechPartsWidget->setModel(proxyModel);
	editWeaponsWidget = new EditWeaponsWidget(view);

	stackedWidget = new QStackedWidget(this);
	stackedWidget->addWidget(topLevelPlaceholder);
	stackedWidget->addWidget(editMechStatsWidget);
	stackedWidget->addWidget(editMechPartsWidget);
	stackedWidget->addWidget(editWeaponsWidget);
}

void EditMechsDialog::initButtons()
{
	mainButtonGroup = new QDialogButtonBox(this);
	addMechButton = mainButtonGroup->addButton(BTech::Strings::ButtonAddMech, QDialogButtonBox::ActionRole);
	removeMechButton = mainButtonGroup->addButton(BTech::Strings::ButtonRemoveMech, QDialogButtonBox::ActionRole);
	submitButton = mainButtonGroup->addButton(BTech::Strings::ButtonSaveMech, QDialogButtonBox::ApplyRole);
	discardButton = mainButtonGroup->addButton(BTech::Strings::ButtonDiscardChanges, QDialogButtonBox::ResetRole);

	connect(addMechButton,    &QPushButton::pressed, this, &EditMechsDialog::addMech);
	connect(removeMechButton, &QPushButton::pressed, this, &EditMechsDialog::removeMech);
	connect(submitButton,     &QPushButton::pressed, this, &EditMechsDialog::save);
	connect(discardButton,    &QPushButton::pressed, this, &EditMechsDialog::discard);
}

void EditMechsDialog::initLayout()
{
	// TODO later

	QHBoxLayout *upperPartLayout = new QHBoxLayout;
	upperPartLayout->addWidget(view);
	upperPartLayout->addWidget(stackedWidget);

	statusBar->setSizeGripEnabled(false);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(upperPartLayout);
	layout->addWidget(mainButtonGroup);
	layout->addWidget(statusBar);

	layout->setSizeConstraint(QLayout::SetFixedSize);
	setLayout(layout);
}

void EditMechsDialog::addMech()
{
	MechModel::addNewMech();
}

void EditMechsDialog::removeMech()
{
	QModelIndex index = view->selectionModel()->currentIndex();

	if (MechModel::getIndexType(index) == MechModel::Level::Root)
		return;

	while (MechModel::getIndexType(index) != MechModel::Level::Mech)
		index = index.parent();

	// TODO this makes removeMech function in MechModel idle. The funny part is that MechModel::removeMech actually uses the following one.
	MechModel::getInstance().removeRows(index.row(), 1, QModelIndex());
}

void EditMechsDialog::save()
{
	AbstractEditMechsWidget *currentWidget = qobject_cast<AbstractEditMechsWidget *>(stackedWidget->currentWidget());
	Q_ASSERT(currentWidget != nullptr);
	currentWidget->save();
}

void EditMechsDialog::discard()
{
	AbstractEditMechsWidget *currentWidget = qobject_cast<AbstractEditMechsWidget *>(stackedWidget->currentWidget());
	Q_ASSERT(currentWidget != nullptr);
	currentWidget->discard();
}

void EditMechsDialog::adjustStackedWidget()
{
	QModelIndex index = view->selectionModel()->currentIndex();
// 	qDebug() << __PRETTY_FUNCTION__ << index << view->selectionModel() << view->model();
	AbstractEditMechsWidget *currentWidget = *levelToWidget[MechModel::getIndexType(index)];

	stackedWidget->setCurrentWidget(currentWidget);
	currentWidget->setCurrentIndex(index);
}
