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
#include "BTMapEditor/dialogs/EditMechStatsWidget.h"

/**
 * \class EditMechStatsWidget
 */

static const int StatusBarMessageTimeoutMs = 5000;

EditMechStatsWidget::EditMechStatsWidget(QAbstractItemView *view, QStatusBar *statusBar)
	: view(view), statusBar(statusBar)
{
	initBoxes();
	initLayout();
	initWidgetMapping();
}

void EditMechStatsWidget::save()
{
	QString name = type->text().trimmed();
	QModelIndex idx = view->selectionModel()->currentIndex();
	if (name.isEmpty()) {
		statusBar->showMessage(BTech::Strings::ErrorMechNameEmpty, StatusBarMessageTimeoutMs);
		return;
	}

	if (name != view->model()->data(idx).toString() && MechModel::hasMech(name)) {
		statusBar->showMessage(BTech::Strings::ErrorMechNameExists, StatusBarMessageTimeoutMs);
		return;
	}

	AbstractEditMechsWidget::save();
}

void EditMechStatsWidget::initBoxes()
{
	type = new QLineEdit;

	for (QSpinBox **spinBox : {&tonnage, &armorValue, &maxMovePoints, &maxJumpPoints, &baseFireRange, &heatSinksNumber} ) {
		*spinBox = new QSpinBox;
		(*spinBox)->setAlignment(Qt::AlignRight);
	}

	for (BTech::Range range : BTech::attackRanges) {
		QSpinBox *spinBox = new QSpinBox;
		rangeDamage[range] = spinBox;
		rangeDamage[range]->setAlignment(Qt::AlignRight);
	}

	type->setFixedWidth(BTech::NAME_LINE_WIDTH);
}

void EditMechStatsWidget::initLayout()
{
// 	removeMechButton = mainButtonGroup->addButton(BTech::Strings::ButtonRemoveMech, QDialogButtonBox::ActionRole);

	QFormLayout *layoutA = new QFormLayout;
	layoutA->addRow(BTech::Strings::LabelTonnage,         tonnage);
	layoutA->addRow(BTech::Strings::LabelArmorValue,      armorValue);
	layoutA->addRow(BTech::Strings::LabelMaxMovePoints,   maxMovePoints);
	layoutA->addRow(BTech::Strings::LabelMaxJumpPoints,   maxJumpPoints);
	layoutA->addRow(BTech::Strings::LabelBaseFireRange,   baseFireRange);
	layoutA->addRow(BTech::Strings::LabelHeatSinksNumber, heatSinksNumber);

	QGroupBox *groupBoxA = new QGroupBox;
	groupBoxA->setLayout(layoutA);

	QFormLayout *layoutB = new QFormLayout;
	for (BTech::Range range : BTech::attackRanges)
		layoutB->addRow(BTech::rangeStringChange[range] + " " + BTech::Strings::LabelDamage, rangeDamage[range]);

	QGroupBox *groupBoxB = new QGroupBox;
	groupBoxB->setLayout(layoutB);

	QHBoxLayout *statsLayout = new QHBoxLayout;
	statsLayout->addWidget(groupBoxA);
	statsLayout->addWidget(groupBoxB);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(type);
	layout->addItem(statsLayout);

	setLayout(layout);
}

void EditMechStatsWidget::initWidgetMapping()
{
	mapper->addMapping(type, MechModel::Mech::Name);

	mapper->addMapping(tonnage,         MechModel::Mech::Tonnage);
	mapper->addMapping(armorValue,      MechModel::Mech::ArmorValue);
	mapper->addMapping(maxMovePoints,   MechModel::Mech::MaxMovePoints);
	mapper->addMapping(maxJumpPoints,   MechModel::Mech::MaxJumpPoints);
	mapper->addMapping(baseFireRange,   MechModel::Mech::BaseFireRange);
	mapper->addMapping(heatSinksNumber, MechModel::Mech::HeatSinksNumber);

	mapper->addMapping(rangeDamage[BTech::Range::Contact], MechModel::Mech::ContactRangeDamage);
	mapper->addMapping(rangeDamage[BTech::Range::Short],   MechModel::Mech::ShortRangeDamage);
	mapper->addMapping(rangeDamage[BTech::Range::Medium],  MechModel::Mech::MediumRangeDamage);
	mapper->addMapping(rangeDamage[BTech::Range::Long],    MechModel::Mech::LongRangeDamage);
}

void EditMechStatsWidget::removeMech()
{
	const MechBase *mech = MechModel::getRow(view->selectionModel()->currentIndex().row());
	MechModel::removeMech(mech->getUid());
}
