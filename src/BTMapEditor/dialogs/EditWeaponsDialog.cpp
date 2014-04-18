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
#include "BTMapEditor/dialogs/EditWeaponsDialog.h"

static const int StatusBarMessageTimeoutMs = 5000;

/**
 * \class ButtonGroup
 */

ButtonGroup::ButtonGroup(QWidget *parent)
	: QWidget(parent)
{
	group = new QButtonGroup(this);
	connect(group, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &ButtonGroup::clicked);
}

int ButtonGroup::getCurrentId() const
{
	return group->checkedId();
}

void ButtonGroup::check(int id)
{
	group->button(id)->click();
}

void ButtonGroup::addButton(QAbstractButton *button, int id)
{
	group->addButton(button, id);
	connect(button, &QRadioButton::toggled, this, &ButtonGroup::emitToggled);
}

void ButtonGroup::emitToggled()
{
	emit toggled(getCurrentId());
}

/**
 * \class EditWeaponsDialog
 */

EditWeaponsDialog::EditWeaponsDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(BTech::Strings::EditWeaponsDialog);

	initSpinBoxes();
	initButtons();
	initLayout();
	initWidgetMapping();
}

void EditWeaponsDialog::initButtons()
{
	buttonBox = new QDialogButtonBox;
	addWeaponButton = buttonBox->addButton(BTech::Strings::ButtonAddWeapon, QDialogButtonBox::ApplyRole);
	saveWeaponButton = buttonBox->addButton(BTech::Strings::ButtonSaveWeapon, QDialogButtonBox::ApplyRole);
	removeWeaponButton = buttonBox->addButton(BTech::Strings::ButtonRemoveWeapon, QDialogButtonBox::ActionRole);
	resetButton = buttonBox->addButton(QDialogButtonBox::Reset);
	closeButton = buttonBox->addButton(QDialogButtonBox::Close);

	connect(addWeaponButton,    &QPushButton::pressed, this, &EditWeaponsDialog::addWeapon);
	connect(removeWeaponButton, &QPushButton::pressed, this, &EditWeaponsDialog::removeWeapon);
	connect(saveWeaponButton,   &QPushButton::pressed, this, &EditWeaponsDialog::saveWeapon);
	connect(closeButton,        &QPushButton::pressed, this, &QDialog::accept);

	typeEnergy = new QRadioButton(BTech::Strings::LabelWeaponTypeEnergy);
	typeEnergy->setChecked(true);
	typeBallistic = new QRadioButton(BTech::Strings::LabelWeaponTypeBallistic);
	typeMissile = new QRadioButton(BTech::Strings::LabelWeaponTypeMissile);

	weaponTypeGroup = new ButtonGroup;
	weaponTypeGroup->addButton(typeEnergy,    toUnderlying(BTech::WeaponType::Energy));
	weaponTypeGroup->addButton(typeBallistic, toUnderlying(BTech::WeaponType::Ballistic));
	weaponTypeGroup->addButton(typeMissile,   toUnderlying(BTech::WeaponType::Missile));

	connect(weaponTypeGroup, &ButtonGroup::toggled, this, &EditWeaponsDialog::adjustEditability);
}

void EditWeaponsDialog::initLayoutEditPart(QBoxLayout* layout)
{
	nameEdit = new QLineEdit;

	QFormLayout *nameLayout = new QFormLayout;
	nameLayout->setLabelAlignment(Qt::AlignRight);
	nameLayout->addRow(new QLabel(BTech::Strings::LabelName), nameEdit);

	QVBoxLayout *weaponTypeBoxLayout = new QVBoxLayout;
	weaponTypeBoxLayout->addWidget(typeEnergy);
	weaponTypeBoxLayout->addWidget(typeBallistic);
	weaponTypeBoxLayout->addWidget(typeMissile);

	QGroupBox *weaponTypeBox = new QGroupBox(BTech::Strings::LabelWeaponType);
	weaponTypeBox->setLayout(weaponTypeBoxLayout);

	QFormLayout *weaponParametersBoxLayout = new QFormLayout;
	weaponParametersBoxLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
	weaponParametersBoxLayout->setLabelAlignment(Qt::AlignRight);

	weaponParametersBoxLayout->addRow(BTech::Strings::LabelHeat,               heat);
	weaponParametersBoxLayout->addRow(BTech::Strings::LabelDamage,             damage);
	weaponParametersBoxLayout->addRow(BTech::Strings::LabelTonnage,            tonnage);
	weaponParametersBoxLayout->addRow(BTech::Strings::LabelCriticalSpaces,     criticalSpaces);
	weaponParametersBoxLayout->addRow(BTech::Strings::LabelAmmoShotsPerTon,    ammoPerTon);
	weaponParametersBoxLayout->addRow(BTech::Strings::LabelMissilesNumberShot, missilesPerShot);

	QGroupBox *weaponParametersBox = new QGroupBox(BTech::Strings::LabelWeaponParameters);
	weaponParametersBox->setLayout(weaponParametersBoxLayout);

	QFormLayout *rangeThresholdBoxLayout = new QFormLayout;
	rangeThresholdBoxLayout->setFormAlignment(Qt::AlignRight);
	rangeThresholdBoxLayout->setLabelAlignment(Qt::AlignRight);

	rangeThresholdBoxLayout->addRow(BTech::Strings::LabelMinimal + " " + BTech::Strings::LabelRange, minRange);
	for (BTech::Range range : BTech::shootRanges)
		rangeThresholdBoxLayout->addRow(BTech::rangeStringChange[range], maxRange[range]);

	QGroupBox *rangeThresholdBox = new QGroupBox(BTech::Strings::LabelRangeThresholds);
	rangeThresholdBox->setLayout(rangeThresholdBoxLayout);

	QVBoxLayout *weaponEditLayout = new QVBoxLayout;
	weaponEditLayout->addWidget(new QLabel(BTech::Strings::LabelWeaponName));
	weaponEditLayout->addWidget(nameEdit);
	weaponEditLayout->addWidget(weaponTypeBox);
	weaponEditLayout->addWidget(weaponParametersBox);
	weaponEditLayout->addWidget(rangeThresholdBox);

	layout->addLayout(weaponEditLayout);
}

void EditWeaponsDialog::initLayoutViewPart(QBoxLayout* layout)
{
	weaponList = new QListView;
	weaponList->setModel(&WeaponModel::getInstance());
	weaponList->setModelColumn(WeaponModel::Name);
	weaponList->setSelectionMode(QAbstractItemView::SingleSelection);

	QVBoxLayout *weaponListLayout = new QVBoxLayout;
	weaponListLayout->addWidget(new QLabel(BTech::Strings::LabelAvailableWeapons));
	weaponListLayout->addWidget(weaponList);

	layout->addLayout(weaponListLayout);
}

void EditWeaponsDialog::initLayout()
{
	QHBoxLayout *topLayout = new QHBoxLayout;
	initLayoutEditPart(topLayout);
	initLayoutViewPart(topLayout);

	statusBar = new QStatusBar(this);
	statusBar->setSizeGripEnabled(false);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(topLayout);
	layout->addWidget(buttonBox);
	layout->addWidget(statusBar);

	layout->setSizeConstraint(QLayout::SetFixedSize);
	setLayout(layout);
}

void EditWeaponsDialog::initSpinBoxes()
{
	minRange = new QSpinBox;
	minRange->setAlignment(Qt::AlignRight);
	minRange->setFixedWidth(BTech::SPIN_BOX_WIDTH);

	for (BTech::Range range : BTech::shootRanges) {
		QSpinBox *rangeBox = new QSpinBox;
		rangeBox->setAlignment(Qt::AlignRight);
		rangeBox->setFixedWidth(BTech::SPIN_BOX_WIDTH);

		rangeBox->setRange(BTech::minimalMaxRange[range], BTech::maximalMaxRange[range]);

		maxRange[range] = rangeBox;
	}

	maxRangeList.append(maxRange[BTech::Range::Short]);
	maxRangeList.append(maxRange[BTech::Range::Medium]);
	maxRangeList.append(maxRange[BTech::Range::Long]);

	QSignalMapper *maxRangeMapper = new QSignalMapper(this);
	for (QSpinBox *spinBox : maxRangeList) {
		connect(spinBox,        static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			maxRangeMapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
		maxRangeMapper->setMapping(spinBox, maxRangeList.indexOf(spinBox));
		connect(maxRangeMapper, static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped),
			this,           &EditWeaponsDialog::adjustRangeThresholds);
	}

	heat               = new QSpinBox;
	damage             = new QSpinBox;
	tonnage            = new QSpinBox;
	criticalSpaces     = new QSpinBox;
	ammoPerTon         = new QSpinBox;
	missilesPerShot    = new QSpinBox;

	heat->setFixedWidth(BTech::SPIN_BOX_WIDTH);
	damage->setFixedWidth(BTech::SPIN_BOX_WIDTH);
	tonnage->setFixedWidth(BTech::SPIN_BOX_WIDTH);
	criticalSpaces->setFixedWidth(BTech::SPIN_BOX_WIDTH);
	ammoPerTon->setFixedWidth(BTech::SPIN_BOX_WIDTH);
	missilesPerShot->setFixedWidth(BTech::SPIN_BOX_WIDTH);
	ammoPerTon->setMaximum(Weapon::MAX_AMMO_SHOTS_PER_TON);

	heat->setAlignment(Qt::AlignRight);
	damage->setAlignment(Qt::AlignRight);
	tonnage->setAlignment(Qt::AlignRight);
	criticalSpaces->setAlignment(Qt::AlignRight);
	ammoPerTon->setAlignment(Qt::AlignRight);
	missilesPerShot->setAlignment(Qt::AlignRight);

	missilesPerShot->setEnabled(false);
	ammoPerTon->setEnabled(false);

	missilesPerShot->setValue(BTech::DEFAULT_MISSILES_PER_SHOT);
	ammoPerTon->setValue(BTech::DEFAULT_AMMO_PER_TON);
}

void EditWeaponsDialog::initWidgetMapping()
{
	mapper = new QDataWidgetMapper(this);
	mapper->setModel(&WeaponModel::getInstance());
	mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

	mapper->addMapping(nameEdit, WeaponModel::Name);
	mapper->addMapping(weaponTypeGroup, WeaponModel::Type);
	mapper->addMapping(minRange, WeaponModel::MinimalRange);
	mapper->addMapping(maxRange[BTech::Range::Short],  WeaponModel::MaximalShortRange);
	mapper->addMapping(maxRange[BTech::Range::Medium], WeaponModel::MaximalMediumRange);
	mapper->addMapping(maxRange[BTech::Range::Long],   WeaponModel::MaximalLongRange);
	mapper->addMapping(heat, WeaponModel::Heat);
	mapper->addMapping(damage, WeaponModel::Damage);
	mapper->addMapping(tonnage, WeaponModel::Tonnage);
	mapper->addMapping(criticalSpaces, WeaponModel::CriticalSpaces);
	mapper->addMapping(ammoPerTon, WeaponModel::AmmoPerTon);
	mapper->addMapping(missilesPerShot, WeaponModel::MissilesPerShot);

	connect(weaponList->selectionModel(), &QItemSelectionModel::currentRowChanged, mapper, &QDataWidgetMapper::setCurrentModelIndex);
	connect(resetButton, &QPushButton::clicked, mapper, &QDataWidgetMapper::revert);
}

void EditWeaponsDialog::adjustRangeThreshold(QSpinBox *shorter, QSpinBox *longer, bool up)
{
	int valueShorter = shorter->value();
	int valueLonger  = longer->value();
	if (up)
		longer->setValue(qMax(valueShorter + 1, valueLonger));
	else
		shorter->setValue(qMin(valueLonger - 1, valueShorter));
}

void EditWeaponsDialog::addWeapon()
{
	WeaponModel::addNewWeapon();
}

void EditWeaponsDialog::removeWeapon()
{
	QModelIndex idx = weaponList->selectionModel()->currentIndex();
	UID weaponUid = WeaponModel::getRow(idx.row())->getUid();
	WeaponModel::removeWeapon(weaponUid);
}

void EditWeaponsDialog::saveWeapon()
{
	QString name = nameEdit->text().trimmed();
	QModelIndex idx = weaponList->selectionModel()->currentIndex();
	if (name.isEmpty()) {
		statusBar->showMessage(BTech::Strings::ErrorWeaponNameEmpty, StatusBarMessageTimeoutMs);
		return;
	}

	if (name != weaponList->model()->data(idx).toString() && WeaponModel::hasWeapon(name)) {
		statusBar->showMessage(BTech::Strings::ErrorWeaponNameExists, StatusBarMessageTimeoutMs);
		return;
	}
	mapper->submit();
}

void EditWeaponsDialog::adjustRangeThresholds(quint8 spinBoxNumber)
{
	switch (spinBoxNumber) {
		case MAX_SHORT_RANGE_SPIN_BOX:
			adjustRangeThreshold(maxRangeList[MAX_SHORT_RANGE_SPIN_BOX],  maxRangeList[MAX_MEDIUM_RANGE_SPIN_BOX], true);
			adjustRangeThreshold(maxRangeList[MAX_MEDIUM_RANGE_SPIN_BOX], maxRangeList[MAX_LONG_RANGE_SPIN_BOX],   true);
			break;
		case MAX_MEDIUM_RANGE_SPIN_BOX:
			adjustRangeThreshold(maxRangeList[MAX_SHORT_RANGE_SPIN_BOX],  maxRangeList[MAX_MEDIUM_RANGE_SPIN_BOX], false);
			adjustRangeThreshold(maxRangeList[MAX_MEDIUM_RANGE_SPIN_BOX], maxRangeList[MAX_LONG_RANGE_SPIN_BOX],   true);
			break;
		case MAX_LONG_RANGE_SPIN_BOX:
			adjustRangeThreshold(maxRangeList[MAX_MEDIUM_RANGE_SPIN_BOX], maxRangeList[MAX_LONG_RANGE_SPIN_BOX],   false);
			adjustRangeThreshold(maxRangeList[MAX_SHORT_RANGE_SPIN_BOX],  maxRangeList[MAX_MEDIUM_RANGE_SPIN_BOX], false);
			break;
		default:
			qCritical() << "Error: EditWeaponsDialog::adjustRangeThresholds called with a wrong parameter";
	}
}

void EditWeaponsDialog::adjustEditability(quint8 radioButtonNumber)
{
	switch (radioButtonNumber) {
		case toUnderlying(BTech::WeaponType::Energy):
			ammoPerTon->setEnabled(false);
			missilesPerShot->setEnabled(false);
			ammoPerTon->setValue(BTech::DEFAULT_AMMO_PER_TON);
			missilesPerShot->setValue(BTech::DEFAULT_MISSILES_PER_SHOT);
			break;
		case toUnderlying(BTech::WeaponType::Ballistic):
			ammoPerTon->setEnabled(true);
			missilesPerShot->setEnabled(false);
			missilesPerShot->setValue(BTech::DEFAULT_MISSILES_PER_SHOT);
			break;
		case toUnderlying(BTech::WeaponType::Missile):
			ammoPerTon->setEnabled(true);
			missilesPerShot->setEnabled(true);
			break;
	}
}
