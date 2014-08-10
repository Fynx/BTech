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

#include "BTCommon/Hex.h"
#include "BTCommon/InfoBar.h"
#include "BTCommon/MechEntity.h"

/* constructor */
InfoBar::InfoBar()
{
	initSettings();
	initWidget();
	initImageWindow();
	initStatWindow();
	initHexWindow();
	initLayout();
	initTimer();
}

void InfoBar::show()
{
	tX = 0;
	timer->start(10);
}

void InfoBar::hide()
{
	tX = -MAXIMUM_WIDTH;
	timer->start(10);
}

bool InfoBar::isVisible() const
{
	return tX == 0;
}

void InfoBar::setDisplayState(const InfoBar::DisplayState &state)
{
	switch (state) {
		case DisplayState::Full:
			statWindow->setVisible(true);
			hexWindow->setVisible(true);
			break;
		case DisplayState::HexWindowOnly:
			statWindow->setVisible(false);
			hexWindow->setVisible(true);
			break;
		case DisplayState::Hidden:
			statWindow->setVisible(false);
			hexWindow->setVisible(false);
			hide();
			break;
	}
}

void InfoBar::setMech(const MechEntity *mech)
{
	curMech = mech;
	updateStatWindow();
}

void InfoBar::removeMech()
{
	curMech = nullptr;
}

void InfoBar::setHex(const Hex *hex)
{
	curHex = hex;
	updateHexWindow();
}

void InfoBar::removeHex()
{
	curHex = nullptr;
	updateHexWindow();
}

void InfoBar::update()
{
	updateStatWindow();
	updateHexWindow();
	return QDockWidget::update();
}

void InfoBar::updateStatWindow()
{
	statWindow->setText(QString());
	if (curMech != nullptr) {
		statWindow->append(BTech::Strings::LabelName + tr(": %1").arg(curMech->getName()));
		statWindow->append(BTech::Strings::LabelArmor + tr(": %1").arg(curMech->getArmorValue()));
		statWindow->append(BTech::Strings::LabelMovePoints + tr(": %1/%2").arg(curMech->getMovePoints()).arg(curMech->getMaxMovePoints()));
		statWindow->append(BTech::Strings::LabelRunPoints + tr(": %1/%2").arg(curMech->getRunPoints()).arg(curMech->getMaxRunPoints()));
		statWindow->append(BTech::Strings::LabelJumpPoints + tr(": %1/%2").arg(curMech->getJumpPoints()).arg(curMech->getMaxJumpPoints()));

		for (BTech::Range range : BTech::attackRanges)
			statWindow->append(BTech::rangeStringChange[range] + " " + BTech::Strings::LabelDamage + tr(": %1").arg(curMech->getRangeDamage(range)));
		statWindow->append(QString());

		for (Effect effect : curMech->getEffects())
			if (effect.isActive())
				statWindow->append("\t" + static_cast<QString>(effect));
	}
}

void InfoBar::updateHexWindow()
{
	hexWindow->setText(QString());
	if (curHex != nullptr) {
		hexWindow->append(BTech::Strings::LabelHex + curHex->getCoordinate().toString());
		hexWindow->append(BTech::Strings::LabelTerrain + tr(": %1").arg(BTech::terrainStringChange[curHex->getTerrain()]));
		hexWindow->append(BTech::Strings::LabelHeight + tr(": %1").arg(curHex->getHeight()));
		hexWindow->append(BTech::Strings::LabelUnits + ":");
		if (curHex->getMech() == nullptr)
			hexWindow->append(tr("\t") + BTech::Strings::LabelNone);
		else
			hexWindow->append("\t" + curHex->getMech()->getName() + "\t" + curHex->getMech()->getOwnerName());
		if (curHex->hasMoveObject())
			; //TODO
		else if (curHex->hasAttackObject())
			; //TODO
	}
}

void InfoBar::initSettings()
{
	setMouseTracking(true);
	setVisible(false);
	setEnabled(false);
	setFeatures(0);
	setMaximumSize(MAXIMUM_WIDTH, MAXIMUM_HEIGHT);
	setMinimumSize(MINIMUM_WIDTH, MINIMUM_HEIGHT);
	setContentsMargins(0, 0, 0, 0);
	removeMech();
	curHex = nullptr;
	tX = -MAXIMUM_WIDTH;
}

void InfoBar::initWidget()
{
	widget = new QWidget(this);
	QPalette wPalette;
	wPalette.setColor(QPalette::Active, QPalette::Background, QColor(0, 0, 0, 40));
	widget->setPalette(wPalette);
	widget->setMouseTracking(true);
	widget->setContentsMargins(0, 0, 0, 0);

	setWidget(widget);
}

void InfoBar::initImageWindow()
{
	imageWindow = new QLabel(this);
	imageWindow->setBaseSize(200, 200);
	imageWindow->setMouseTracking(true);
}

void InfoBar::initStatWindow()
{
	statWindow = new QTextEdit(this);
	statWindow->setReadOnly(true);
	statWindow->setPalette(QPalette(Qt::black));
	statWindow->setBackgroundRole(QPalette::Window);
	statWindow->setTextColor(Qt::green);
	statWindow->setCursorWidth(0);
	statWindow->setMouseTracking(true);
	statWindow->setBaseSize(200, 300);
}

void InfoBar::initHexWindow()
{
	hexWindow = new QTextEdit(this);
	hexWindow->setReadOnly(true);
	hexWindow->setPalette(QPalette(Qt::black));
	hexWindow->setBackgroundRole(QPalette::Window);
	hexWindow->setTextColor(Qt::green);
	hexWindow->setCursorWidth(0);
	hexWindow->setMouseTracking(true);
	hexWindow->setBaseSize(200, 300);
}

void InfoBar::initLayout()
{
	layout = new QVBoxLayout(widget);
	layout->addWidget(imageWindow);
	layout->addWidget(statWindow);
	layout->addWidget(hexWindow);
}

void InfoBar::initTimer()
{
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &InfoBar::advance);
}

void InfoBar::advance()
{
	if (x() < tX)
		move(QPoint(qMin(tX, x() + HIDE_SHOW_SPEED), y()));
	else if (x() > tX)
		move(QPoint(qMax(tX, x() - HIDE_SHOW_SPEED), y()));
	else
		timer->stop();
}
