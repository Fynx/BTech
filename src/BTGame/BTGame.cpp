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

#include "BTCommon/CommonSettings.h"
#include "BTGame/BTGame.h"
#include "BTGame/GameStrings.h"

/* constructor */
BTGame::BTGame()
{
	initLogWindow();
	initSideBar();
	initCentralWindow();
	initMenu();
	readSettings();
}

BTGame::~BTGame()
{
	writeSettings();
}

void BTGame::initLogWindow()
{
	logWindow = new LogWindow(this);
	addDockWidget(Qt::RightDockWidgetArea, logWindow);
	logWindow->hide();
}

void BTGame::initSideBar()
{
	sideBar = new SideBar;
	sideBar->resize(DEFAULT_SIDEBAR_WIDTH, height());
	addDockWidget(Qt::RightDockWidgetArea, sideBar);
}

void BTGame::initCentralWindow()
{
	getGraphicsMap()->stackUnder(sideBar);

	connect(getMap(), &Map::mechActionsNeeded, this, &BTGame::setActionsInSideBar);
	connect(getMap(), &Map::mechActionsNotNeeded, sideBar->getActionWindow(), &ActionWindow::clear);
	connect(getMap(), &Map::messageSent, this, &BTGame::messageFromMapToSideBar);
	connect(getMap(), &Map::extensiveInfoSent, this, &BTGame::onExtensiveInfo);

	connect(sideBar, &SideBar::endMoveButtonPressed, getGraphicsMap(), &GraphicsMap::onEndMove);
	connect(sideBar->getActionWindow(), &ActionWindow::actionActivated, getGraphicsMap(), &GraphicsMap::onChooseAction);

	connect(getMap(), &Map::gameStarted, this, &BTGame::onGameStarted);
	connect(getMap(), &Map::gameEnded,   this, &BTGame::onGameEnded);

	connect(getGraphicsMap(), &GraphicsMap::mapCleared, sideBar->getActionWindow(), &ActionWindow::clear);
	connect(getGraphicsMap(), &GraphicsMap::mapCleared, sideBar, &SideBar::disable);
}

void BTGame::initMenu()
{
	menuTriggerLogWindow = new QAction(this);
	menuTriggerLogWindow->setEnabled(true);
	menuTriggerLogWindow->setText(BTech::Strings::ActionTriggerLogWindow);
	menuTriggerLogWindow->setShortcut(tr("Ctrl+L"));
	connect(menuTriggerLogWindow, &QAction::triggered, this, &BTGame::onTriggerLogWindow);
	viewMenu->addAction(menuTriggerLogWindow);

	gameMenu = menuBar()->addMenu(BTech::Strings::MenuGame);

	menuStartGameAction = new QAction(this);
	menuStartGameAction->setEnabled(false);
	menuStartGameAction->setText(BTech::Strings::ActionStartGame);
	menuStartGameAction->setShortcut(tr("Ctrl+F"));
	connect(menuStartGameAction, &QAction::triggered, this, &BTGame::onStartGameAction);
	gameMenu->addAction(menuStartGameAction);

	menuSetVersion = new QAction(this);
	menuSetVersion->setEnabled(false);
	menuSetVersion->setText(BTech::Strings::ActionSetVersion);
	menuSetVersion->setShortcut(tr("Ctrl+V"));
	connect(menuSetVersion, &QAction::triggered, this, &BTGame::onSetVersionAction);
	gameMenu->addAction(menuSetVersion);
}

void BTGame::readSettings()
{
	restoreGeometry(Settings::value(BTech::Settings::Window::Geometry).toByteArray());
}

void BTGame::writeSettings()
{
	Settings::setValue(BTech::Settings::Window::Geometry, saveGeometry());
	Settings::sync();
}

void BTGame::keyPressEvent(QKeyEvent *event)
{
	sideBar->keyPressEvent(event);
	return QWidget::keyPressEvent(event);
}

void BTGame::onTriggerLogWindow()
{
	logWindow->setVisible(!logWindow->isVisible());
}

void BTGame::onLoadMapAction()
{
	BTMapManager::onLoadMapAction();
	if (getGraphicsMap()->isValid()) {
		menuStartGameAction->setEnabled(true);
		menuSetVersion->setEnabled(true);
	}
}

void BTGame::onStartGameAction()
{
	sideBar->enable();
	menuStartGameAction->setEnabled(false);
	getMap()->startGame();
}

void BTGame::onSetVersionAction()
{
	GameVersionChoiceDialog dialog(getMap()->getAllowedVersions());
	if (dialog.exec())
		Rules::setVersion(dialog.getVersion());
}

void BTGame::onGameStarted()
{}

void BTGame::onGameEnded()
{
	getInfoBar()->hide();
	sideBar->disable();
}

void BTGame::setActionsInSideBar()
{
	sideBar->getActionWindow()->insertActions(getMap()->getCurrentPhase(), getMap()->getCurrentUnit());
}

void BTGame::messageFromMapToSideBar()
{
	auto message = getMap()->getMessage();
	sideBar->insertMessage(message.text, message.color);
}

void BTGame::onExtensiveInfo()
{
	QColor color    = getMap()->getExtensiveInfo().color;
	QString message = getMap()->getExtensiveInfo().text;
	qDebug() << BTech::General::bashColorString(message, color).toStdString().c_str();

	logWindow->print({getMap()->getExtensiveInfo().text, getMap()->getExtensiveInfo().color});
}

/**
 * \class GameVersionChoiceDialog
 */

GameVersionChoiceDialog::GameVersionChoiceDialog(const QList <BTech::GameVersion> &allowedVersions)
	: QDialog()
{
	setFixedSize(200, 300);

	QVBoxLayout *checkBoxLayout = new QVBoxLayout;
	checkBoxLayout->setAlignment(Qt::AlignTop);

	QVector <QCheckBox *> checkBox;
	group = new QButtonGroup;
	group->setExclusive(true);
	for (BTech::GameVersion version : allowedVersions) {
		checkBox << new QCheckBox(BTech::gameVersionStringChange[version]);
		group->addButton(checkBox.last());
		checkBoxLayout->addWidget(checkBox.last());
	}
	checkBox.first()->setChecked(true);

	QVBoxLayout *layout = new QVBoxLayout;

	layout->addItem(checkBoxLayout);

	confirmButton = new QPushButton(BTech::Strings::ButtonConfirm);
	connect(confirmButton, &QPushButton::pressed, this, &GameVersionChoiceDialog::accept);
	layout->addWidget(confirmButton);

	setLayout(layout);
}

BTech::GameVersion GameVersionChoiceDialog::getVersion() const
{
	return BTech::gameVersionStringChange[group->checkedButton()->text()];
}
