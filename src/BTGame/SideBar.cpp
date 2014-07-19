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

#include "BTCommon/Action.h"
#include "BTGame/GameStrings.h"
#include "BTGame/SideBar.h"

/**
 * \class ActionLabel
 */

static const QColor ColorHighlighted    = Qt::darkYellow;
// ^ this is really funny but this is actually the only color here that works. BTech::Colors::FloralWhite;
static const QColor ColorNotHighlighted = BTech::Colors::CornflowerBlue;

/* constructor */
ActionLabel::ActionLabel(QWidget *parent, QString text, int number, const Action *action)
{
	setParent(parent);
	QString lText = QString("%1 %2").arg(number).arg(text);
	QString wText = QString();
	if (action->getActionType() == Action::Type::Combat) {
		const CombatAction *combatAction = static_cast<const CombatAction *>(action);
		if (combatAction->getWeapon() != nullptr)
			wText += static_cast<QString>(*(combatAction->getWeapon()));
		if (combatAction->getWeaponHolder() != nullptr)
		        wText += " (" + combatAction->getWeaponHolder()->getUnitName() + ")";
	}
	setText(lText + " " + wText);

	this->number = number;
	this->action = action;

	activated = false;
	setCheckable(true);
	setHighlighted(false);
	setMouseTracking(true);
}

/* destructor */
ActionLabel::~ActionLabel()
{
	delete action;
}

void ActionLabel::activate()
{
	if (!checkable)
		return;
	activated = true;
	setHighlighted(true);
}

void ActionLabel::deactivate()
{
	if (!checkable)
		return;
	activated = false;
	if (!tracked)
		setHighlighted(false);
}

void ActionLabel::click()
{
	if (activated)
		deactivate();
	else
		activate();
	emit clicked(action);
}

void ActionLabel::setCheckable(bool checkable)
{
	this->checkable = checkable;
}

bool ActionLabel::isCheckable() const
{
	return checkable;
}

void ActionLabel::setHighlighted(bool highlighted)
{
	this->highlighted = highlighted;
	if (highlighted)
		setPalette(QPalette(ColorHighlighted));
	else
		setPalette(QPalette(ColorNotHighlighted));
}

void ActionLabel::mousePressEvent(QMouseEvent *)
{
	click();
}

void ActionLabel::enterEvent(QEvent *)
{
	tracked = true;
	setHighlighted(true);
}

void ActionLabel::leaveEvent(QEvent *)
{
	tracked = false;
	if (!activated)
		setHighlighted(false);
}

/**
 * \class ActionWindow
 */

/* constructor */
ActionWindow::ActionWindow(QWidget* parent)
	: QWidget(parent)
{
	initLayout();
	setBackgroundRole(QPalette::Window);
	setPalette(Qt::lightGray);
	currentAction = nullptr;
	setMouseTracking(true);
}

void ActionWindow::keyPressEvent(QKeyEvent *event)
{
	int key = 0;
	switch (event->key()) {
		case Qt::Key_1: key = 1; break;
		case Qt::Key_2: key = 2; break;
		case Qt::Key_3: key = 3; break;
		case Qt::Key_4: key = 4; break;
		case Qt::Key_5: key = 5; break;
		case Qt::Key_6: key = 6; break;
		case Qt::Key_7: key = 7; break;
		case Qt::Key_8: key = 8; break;
		case Qt::Key_9: key = 9; break;
	}

	for (ActionLabel *label : actionList)
		if (label->number == key)
			label->click();
	QWidget::keyPressEvent(event);
}

void ActionWindow::insertActions(const BTech::GamePhase &phase, const MechEntity *mech)
{
	insertActions(mech->getActions(phase));
}

void ActionWindow::insertActions(QList <const Action *> actions)
{
	for (const Action *action : actions)
		insertAction(action);
}

void ActionWindow::insertAction(const Action *action)
{
	QString text = actionToString(action);
	int num = list.size() + 1;
	ActionLabel *l = new ActionLabel(this, text, num, action);
	l->setCheckable(isActionCheckable(action));
	connect(l, &ActionLabel::clicked, this, &ActionWindow::onActivateAction);
	list.insert(text, num);
	actionList.append(l);
	layout->addWidget(l);
}

void ActionWindow::onActivateAction(const Action *action)
{
	if (currentAction != nullptr) {
		getActionLabel(currentAction)->deactivate();
		currentAction = (currentAction == action)
			 ? nullptr
			 : action;
	} else {
		currentAction = action;
	}
	emit actionActivated(currentAction);
	if (currentAction != nullptr && !isActionCheckable(currentAction))
		currentAction = nullptr;
}

void ActionWindow::clear()
{
	list.clear();
	for (ActionLabel *label : actionList)
		delete label;
	actionList.clear();
	currentAction = nullptr;
}

QString ActionWindow::actionToString(const Action *action) const
{
	switch (action->getActionType()) {
		case Action::Type::Movement:
			return movementActionToString[static_cast<const MovementAction *>(action)->getType()];
		case Action::Type::Combat:
			return combatActionToString[static_cast<const CombatAction *>(action)->getType()];
		default:
			return QString();
	}
}

bool ActionWindow::isActionCheckable(const Action *action) const
{
	switch (action->getActionType()) {
		case Action::Type::Movement:
			return movementActionCheckable[static_cast<const MovementAction *>(action)->getType()];
		case Action::Type::Combat:
			return combatActionCheckable[static_cast<const CombatAction *>(action)->getType()];
		default:
			return false;
	}
}

const QHash <BTech::MovementAction, QString> ActionWindow::movementActionToString {
	{BTech::MovementAction::Idle,         BTech::Strings::None      },
	{BTech::MovementAction::Walk,         BTech::Strings::Walk      },
	{BTech::MovementAction::Run,          BTech::Strings::Run       },
	{BTech::MovementAction::Jump,         BTech::Strings::Jump      },
	{BTech::MovementAction::TurnRight,    BTech::Strings::TurnRight },
	{BTech::MovementAction::TurnLeft,     BTech::Strings::TurnLeft  },
};

const QHash <BTech::CombatAction, QString> ActionWindow::combatActionToString {
	{BTech::CombatAction::SimpleAttack, BTech::Strings::Attack},
	{BTech::CombatAction::Kick,         BTech::Strings::Kick},
	{BTech::CombatAction::Punch,        BTech::Strings::Punch},
	{BTech::CombatAction::Push,         BTech::Strings::Push}
};

const QHash <BTech::MovementAction, bool> ActionWindow::movementActionCheckable {
	{ BTech::MovementAction::Idle,         false },
	{ BTech::MovementAction::Walk,         true  },
	{ BTech::MovementAction::Run,          true  },
	{ BTech::MovementAction::Jump,         true  },
	{ BTech::MovementAction::TurnRight,    false },
	{ BTech::MovementAction::TurnLeft,     false },
};

const QHash <BTech::CombatAction, bool> ActionWindow::combatActionCheckable {
	{ BTech::CombatAction::SimpleAttack, true },
	{ BTech::CombatAction::WeaponAttack, true },
	{ BTech::CombatAction::Kick,         true },
	{ BTech::CombatAction::Punch,        true },
};

void ActionWindow::initLayout()
{
	layout = new QVBoxLayout(this);
	layout->setGeometry(QRect(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT));
	//layout->setSizeConstraint(QLayout::SetFixedSize);
	setLayout(layout);
}

ActionLabel * ActionWindow::getActionLabel(const Action *action)
{
	for (ActionLabel *label : actionList)
		if (label->action == action)
			return label;
	return nullptr;
}

/**
 * \class SideBar
 */

const QColor SideBar::DefaultLogColor = Qt::white;

/* constructor */
SideBar::SideBar()
{
	initWindow();
	initWidget();
	initImageWindow();
	initActionWindow();
	initLogWindow();
	initEndMoveButton();
	initLayout();
	disable();
}

ActionWindow * SideBar::getActionWindow()
{
	return actionWindow;
}

void SideBar::keyPressEvent(QKeyEvent *event)
{
	actionWindow->keyPressEvent(event);
	QWidget::keyPressEvent(event);
}

void SideBar::enable()
{
	endMoveButton->show();
}

void SideBar::disable()
{
	endMoveButton->hide();
}

void SideBar::insertMessage(const QString &arg, const QColor &color)
{
	logWindow->setTextColor(color);
	log << arg;
	logWindow->append(arg);
}

void SideBar::initWindow()
{
	setMouseTracking(true);
	setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	setMaximumSize(MAX_WIDTH, MAX_HEIGHT);
	setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
	setContentsMargins(0, 0, 0, 0);
}

void SideBar::initWidget()
{
	setWidget(new QWidget(this));
	widget()->setMouseTracking(true);
}

void SideBar::initImageWindow()
{
	imageWindow = new QLabel(this);
	imageWindow->resize(200, 200);
	imageWindow->setMouseTracking(true);
	image = nullptr;
}

void SideBar::initActionWindow()
{
	actionWindow = new ActionWindow(this);
}

void SideBar::initLogWindow()
{
	logWindow = new QTextEdit(this);
	logWindow->setReadOnly(true);
	logWindow->setBackgroundRole(QPalette::Window);
	logWindow->setPalette(QPalette(Qt::black));
	logWindow->setAutoFillBackground(true);
	logWindow->setTextColor(Qt::green);
	logWindow->setCursorWidth(0);
	logWindow->setMouseTracking(true);
	logWindow->setBaseSize(300, 200);
}

void SideBar::initEndMoveButton()
{
	endMoveButton = new QPushButton(tr("End move"), this);
	endMoveButton->setAutoDefault(false);
	endMoveButton->setDefault(false);
	endMoveButton->setMouseTracking(true);
	connect(endMoveButton, &QPushButton::clicked, this, &SideBar::endMoveButtonPressed);
	endMoveButton->setShortcut(tr("E"));
}

void SideBar::initLayout()
{
	layout = new QVBoxLayout();
	layout->addWidget(imageWindow);
	layout->addWidget(actionWindow);
	layout->addWidget(logWindow);
	layout->addWidget(endMoveButton);
	widget()->setLayout(layout);
	widget()->setContentsMargins(0, 0, 0, 0);
}
