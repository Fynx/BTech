#include "BTCommon/GraphicsFactory.h"
#include "BTMapEditor/ClickModeManager.h"

/**
 * \class DirectionStar
 */

DirectionStar::DirectionStar()
{
	initLabels();
	clear();
	setEnabled(false);
}

void DirectionStar::setEnabled(bool enabled)
{
	clear();
	return QWidget::setEnabled(enabled);
}

void DirectionStar::clear()
{
	for (Direction direction : BTech::directions)
		directionLabel[direction]->setFrameShadow(QFrame::Plain);
}

void DirectionStar::setDirection(Direction direction)
{
	clear();
	directionLabel[direction]->setFrameShadow(QFrame::Raised);
}

void DirectionStar::initLabels()
{
	QHBoxLayout *layout = new QHBoxLayout;	// this isn't what I want, but well...
	QSignalMapper *mapper = new QSignalMapper;
	for (Direction direction : BTech::directions) {
		directionLabel[direction] = new ClickableLabel(BTech::directionStringChange[direction]);
		directionLabel[direction]->setFrameShape(QFrame::Box);
		connect(directionLabel[direction], &ClickableLabel::clicked, mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
		mapper->setMapping(directionLabel[direction], BTech::directionStringChange[direction]);
		layout->addWidget(directionLabel[direction]);
	}
	connect(mapper, static_cast<void (QSignalMapper::*)(const QString &)>(&QSignalMapper::mapped), this, &DirectionStar::onDirectionChosen);

	setLayout(layout);
}

void DirectionStar::onDirectionChosen(const QString &directionName)
{
	Direction direction = BTech::directionStringChange[directionName];
	setDirection(direction);
	emit directionChosen(direction);
}

/**
 * \class ClickModeManager
 */

ClickModeManager::ClickModeManager()
{
	currentHex = nullptr;

	initBoxes();

	QLabel *hexLabel = new QLabel(BTech::Strings::LabelHex);

	QLabel *terrainComboBoxLabel = new QLabel(BTech::Strings::LabelTerrain);
	QLabel *heightSpinBoxLabel = new QLabel(BTech::Strings::LabelHeight);

	QFrame *line = new QFrame;
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);

	QLabel *unitLabel = new QLabel(BTech::Strings::LabelUnit);

	QLabel *unitTypeComboBoxLabel = new QLabel(BTech::Strings::LabelMechType);

	QLabel *unitDirectionLabel = new QLabel(BTech::Strings::LabelDirection);
	directionStar = new DirectionStar;
	connect(directionStar, &DirectionStar::directionChosen, this, &ClickModeManager::onMechDirectionChosen);

	removeUnitButton = new QPushButton(BTech::Strings::ButtonRemoveMech);
	connect(removeUnitButton, &QPushButton::pressed, this, &ClickModeManager::onRemoveUnit);
	removeUnitButton->setEnabled(false);

	/* layout */

	QHBoxLayout *terrainComboBoxLayout = new QHBoxLayout;
	terrainComboBoxLayout->addWidget(terrainComboBoxLabel);
	terrainComboBoxLayout->addWidget(terrainComboBox);

	QHBoxLayout *heightSpinBoxLayout = new QHBoxLayout;
	heightSpinBoxLayout->addWidget(heightSpinBoxLabel);
	heightSpinBoxLayout->addWidget(heightSpinBox);

	QHBoxLayout *unitTypeComboBoxLayout = new QHBoxLayout;
	unitTypeComboBoxLayout->addWidget(unitTypeComboBoxLabel);
	unitTypeComboBoxLayout->addWidget(unitTypeComboBox);

	QHBoxLayout *unitDirectionLayout = new QHBoxLayout;
	unitDirectionLayout->addWidget(unitDirectionLabel);
	unitDirectionLayout->addWidget(directionStar);

	QHBoxLayout *removeUnitButtonLayout = new QHBoxLayout;
	removeUnitButtonLayout->addSpacerItem(new QSpacerItem(150, 1));
	removeUnitButtonLayout->addWidget(removeUnitButton);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setAlignment(Qt::AlignTop);
	layout->addWidget(hexLabel);
	layout->addItem(terrainComboBoxLayout);
	layout->addItem(heightSpinBoxLayout);
	layout->addWidget(heightSpinBox);
	layout->addWidget(line);
	layout->addWidget(unitLabel);
	layout->addItem(unitTypeComboBoxLayout);
	layout->addItem(unitDirectionLayout);
	layout->addItem(removeUnitButtonLayout);
	setLayout(layout);
}

void ClickModeManager::onHexClicked(Hex *hex)
{
	terrainComboBox->setEnabled(true);
	heightSpinBox->setEnabled(true);
	currentHex = hex;
	refresh();
}

void ClickModeManager::initBoxes()
{
	terrainComboBox = new QComboBox;
	terrainComboBox->setFixedWidth(150);
	for (BTech::Terrain terrain : BTech::terrainTypes)
		terrainComboBox->addItem(BTech::terrainStringChange[terrain]);
	connect(terrainComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &ClickModeManager::onTerrainChosen);
	terrainComboBox->setEnabled(false);

	heightSpinBox = new QSpinBox;
	heightSpinBox->setFixedWidth(100);
	heightSpinBox->setRange(-5, 5);
	connect(heightSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ClickModeManager::changeHexHeight);
	heightSpinBox->setEnabled(false);

	unitTypeComboBox = new QComboBox;
	unitTypeComboBox->setFixedWidth(150);
	unitTypeComboBox->setEnabled(false);
	for (const MechBase *mech : MechModel::getMechs())
		unitTypeComboBox->addItem(static_cast<QString>(*mech));
	connect(unitTypeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &ClickModeManager::onMechTypeSelected);
}

BTech::Terrain ClickModeManager::getCurrentTerrain() const
{
	return BTech::terrainStringChange[terrainComboBox->currentText()];
}

QString ClickModeManager::getCurrentMechType() const
{
	return unitTypeComboBox->currentText();
}

quint32 ClickModeManager::getCurrentMechTypeSerialNumber() const
{
	return MechModel::getMech(getCurrentMechType())->getUid();
}

void ClickModeManager::refresh()
{
	for (int i = 0; i < terrainComboBox->count(); ++i)
		if (terrainComboBox->itemText(i) == BTech::terrainStringChange[currentHex->getTerrain()])
			terrainComboBox->setCurrentIndex(i);
	heightSpinBox->setValue(currentHex->getHeight());

	bool hasMech = currentHex->getMech() != nullptr;

	unitTypeComboBox->setEnabled(hasMech);
	directionStar->setEnabled(hasMech);
	removeUnitButton->setEnabled(hasMech);
	if (hasMech) {
		for (int i = 0; i < unitTypeComboBox->count(); ++i)
			if (unitTypeComboBox->itemText(i) == currentHex->getMech()->getType())
				unitTypeComboBox->setCurrentIndex(i);
		directionStar->setDirection(currentHex->getMech()->getCurrentDirection());
	}
}

void ClickModeManager::onTerrainChosen()
{
	currentHex->setTerrain(getCurrentTerrain());
	emit hexInfoChanged();
}

void ClickModeManager::changeHexHeight(int height)
{
	currentHex->setHeight(height);
	emit hexInfoChanged();
}

void ClickModeManager::onMechTypeSelected()
{
	currentHex->getMech()->setBase(MechModel::getMech(getCurrentMechTypeSerialNumber()));
	emit mechTypeSelected(getCurrentMechTypeSerialNumber());
}

void ClickModeManager::onMechDirectionChosen(Direction direction)
{
	currentHex->getMech()->setDirection(direction);
	emit mechInfoChanged();
}

void ClickModeManager::onRemoveUnit()
{
	emit mechNeedsRemoving(currentHex->getMech());
	refresh();
	emit mechRemoved();
}
