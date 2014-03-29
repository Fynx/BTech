#include "BTMapEditor/dialogs/EditMechPartsWidget.h"

/**
 * \class EditMechPartsWidget
 */

EditMechPartsWidget::EditMechPartsWidget(QAbstractItemView *view)
	: view(view)
{
	initBoxes();
	initButtons();
	initListView();
	initLayout();
	initWidgetMapping();
}

void EditMechPartsWidget::setCurrentIndex(const QModelIndex &index)
{
	AbstractEditMechsWidget::setCurrentIndex(index);
	attachedWeapons->setRootIndex(index);
}

void EditMechPartsWidget::setModel(QAbstractItemModel *model)
{
	attachedWeapons->setModel(model);
}

void EditMechPartsWidget::initBoxes()
{
	maxArmorValue = new QSpinBox;
	maxInternalValue = new QSpinBox;

	maxArmorValue->setFixedWidth(BTech::SPIN_BOX_WIDTH);
	maxInternalValue->setFixedWidth(BTech::SPIN_BOX_WIDTH);

	maxArmorValue->setMinimum(MechPartBase::MINIMAL_ARMOR_VALUE);
	maxInternalValue->setMinimum(MechPartBase::MINIMAL_INTERNAL_VALUE);
}

void EditMechPartsWidget::initButtons()
{
	addWeaponButton      = new QPushButton(QIcon(BTech::Paths::ICON_LIST_ADD), QString());
	removeWeaponButton   = new QPushButton(QIcon(BTech::Paths::ICON_LIST_REMOVE), QString());
	moveWeaponUpButton   = new QPushButton(QIcon(BTech::Paths::ICON_ARROW_UP), QString());
	moveWeaponDownButton = new QPushButton(QIcon(BTech::Paths::ICON_ARROW_DOWN), QString());

	connect(addWeaponButton,      &QPushButton::pressed, this, &EditMechPartsWidget::addWeapons);
	connect(removeWeaponButton,   &QPushButton::pressed, this, &EditMechPartsWidget::removeWeapons);
	connect(moveWeaponUpButton,   &QPushButton::pressed, this, &EditMechPartsWidget::moveWeaponsUp);
	connect(moveWeaponDownButton, &QPushButton::pressed, this, &EditMechPartsWidget::moveWeaponsDown);
}

void EditMechPartsWidget::initListView()
{
	attachedWeapons = new QListView;
	attachedWeapons->setModel(&MechModel::getInstance());
	attachedWeapons->setModelColumn(WeaponModel::Name);
	attachedWeapons->setSelectionBehavior(QAbstractItemView::SelectRows);
	attachedWeapons->setSelectionMode(QAbstractItemView::ExtendedSelection);

	allWeapons = new QListView;
	allWeapons->setModel(&WeaponModel::getInstance());
	allWeapons->setModelColumn(WeaponModel::Name);
	allWeapons->setSelectionBehavior(QAbstractItemView::SelectRows);
	allWeapons->setSelectionMode(QAbstractItemView::SingleSelection);	// TODO DON'T LIKE IT
}

void EditMechPartsWidget::initLayout()
{
	QFormLayout *statsLayout = new QFormLayout;

	statsLayout->addRow(new QLabel(BTech::Strings::LabelMaxArmorValue),    maxArmorValue);
	statsLayout->addRow(new QLabel(BTech::Strings::LabelMaxInternalValue), maxInternalValue);

	QGroupBox *statsBox = new QGroupBox(BTech::Strings::LabelArmorValues);
	statsBox->setLayout(statsLayout);

	QVBoxLayout *attachedWeaponsLayout = new QVBoxLayout;
	attachedWeaponsLayout->addWidget(new QLabel(BTech::Strings::LabelWeaponsUsed));
	attachedWeaponsLayout->addWidget(attachedWeapons);

	QVBoxLayout *arrowsLayout = new QVBoxLayout;
	arrowsLayout->addWidget(moveWeaponUpButton);
	arrowsLayout->addWidget(addWeaponButton);
	arrowsLayout->addWidget(removeWeaponButton);
	arrowsLayout->addWidget(moveWeaponDownButton);

	QVBoxLayout *allWeaponsLayout = new QVBoxLayout;
	allWeaponsLayout->addWidget(new QLabel(BTech::Strings::LabelAvailableWeapons));
	allWeaponsLayout->addWidget(allWeapons);

	QHBoxLayout *layout = new QHBoxLayout;

	layout->addWidget(statsBox);
	layout->addLayout(attachedWeaponsLayout);
	layout->addLayout(arrowsLayout);
	layout->addLayout(allWeaponsLayout);

	setLayout(layout);
}

void EditMechPartsWidget::initWidgetMapping()
{
	mapper->addMapping(maxArmorValue,    MechModel::MechPart::MaxArmorValue);
	mapper->addMapping(maxInternalValue, MechModel::MechPart::MaxInternalValue);
}

/// TODO!!! Test all below

QList <int> EditMechPartsWidget::getSelectedIndexes() const
{
	QList <int> result;
	for (const QModelIndex &index : attachedWeapons->selectionModel()->selectedRows())
		result.append(index.row());
	qSort(result.begin(), result.end());

	return result;
}

void EditMechPartsWidget::addWeapons()
{
	for (const QModelIndex &index : allWeapons->selectionModel()->selectedRows())
		MechModel::addWeaponToMechPart(currentIndex(), WeaponModel::getWeapon(index));
}

void EditMechPartsWidget::removeWeapons()
{
	QList <int> indexes = getSelectedIndexes();
	for (int i = indexes.size() - 1; i >= 0; --i)
		MechModel::removeWeaponFromMechPart(currentIndex(), indexes[i]);
}

void EditMechPartsWidget::moveWeaponsUp()
{
	/*QList <int> indexes = getSelectedIndexes();
	for (int i = indexes.size() - 1; i >= 0; --i)
		MechModel::moveWeaponInMechUp(currentIndex(), indexes[i]);
	updateWeaponList();*/
}

void EditMechPartsWidget::moveWeaponsDown()
{
	/*for (int index : getSelectedIndexes())
		MechModel::moveWeaponInMechDown(currentIndex(), index);
	updateWeaponList();*/
}
