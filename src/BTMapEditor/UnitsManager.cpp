#include "BTMapEditor/UnitsManager.h"

/**
 * \class UnitsManager
 */

UnitsManager::UnitsManager(QVector <Player *> &players)
	: players(players)
{
	initMechList();

	playersComboBox = new QComboBox;
	connect(playersComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &UnitsManager::onPlayerChosen);
	updatePlayersComboBox();

	/* layout */

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setAlignment(Qt::AlignTop);

	layout->addWidget(playersComboBox);
	layout->addItem(mechListLayout);

	setLayout(layout);
}

Player * UnitsManager::getCurrentPlayer() const
{
	for (Player *player : players)
		if (player->getName() == playersComboBox->currentText())
			return player;
	return nullptr;
}

void UnitsManager::onMapLoaded()
{
	updatePlayersComboBox();
}

void UnitsManager::refresh()
{
	updatePlayersComboBox();
}

void UnitsManager::initMechList()
{
	QSignalMapper *mapper = new QSignalMapper(this);
	QString mechName;
	mechListLayout = new QVBoxLayout;
	mechListLayout->setAlignment(Qt::AlignTop);
	for (MechBase *mech : MechModel::getMechs()) {
		mechName = static_cast<QString>(*mech);
		if (mechName.length() > 0) {
			//TODO change this ugly stupid label to the mech icon
			ClickableLabel *label = new ClickableLabel(mechName);
			connect(label, &ClickableLabel::clicked, mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
			mapper->setMapping(label, mechName);
			mechListLayout->addSpacerItem(new QSpacerItem(BTech::LITTLE_SPACER_SIZE, BTech::SMALL_SPACER_SIZE));
			mechListLayout->addWidget(label);
		}
	}
	connect(mapper, static_cast<void (QSignalMapper::*)(const QString &)>(&QSignalMapper::mapped), this, &UnitsManager::onUnitChosen);
}

void UnitsManager::updatePlayersComboBox()
{
	playersComboBox->clear();
	for (Player *player : players)
		playersComboBox->addItem(player->getName());
	playersComboBox->setEnabled(!players.isEmpty());
}

void UnitsManager::onUnitChosen(const QString &unitName)
{
	if (playersComboBox->isEnabled()) {
		emit playerChosen(getCurrentPlayer());
		emit unitChosen(MechModel::getMech(unitName)->getUid());
	}
}

void UnitsManager::onPlayerChosen()
{
	emit playerChosen(getCurrentPlayer());
}
