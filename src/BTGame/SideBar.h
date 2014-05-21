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

#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QtWidgets>
#include "BTCommon/MechEntity.h"

/**
 * \class ActionLabel
 * A clickable label that creates an interface for the player to activate action of the given unit.
 */
class ActionLabel : public QLabel
{
Q_OBJECT;

public:
	ActionLabel(QWidget *parent, QString text, int num, const Action *action);
	~ActionLabel();

	int number;
	const Action *action;

	void activate();
	void deactivate();
	void click();

	void setCheckable(bool checkable);
	bool isCheckable() const;

signals:
	void clicked(const Action *action);

private:
	bool checkable;
	bool activated;
	bool tracked;
	bool highlighted;

	void setHighlighted(bool highlighted);

	void mousePressEvent(QMouseEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
};

/**
 * \class ActionWindow
 * A window with actions that can be chosen by a player.
 */
class ActionWindow : public QWidget
{
Q_OBJECT;

public:
	ActionWindow(QWidget *parent);

	void keyPressEvent(QKeyEvent *event);

public slots:
	void insertActions(const BTech::GamePhase &phase, const MechEntity *mech);
	void insertActions(QList <const Action *> actions);
	void insertAction(const Action *action);
	void onActivateAction(const Action *action);
	void clear();

signals:
	void actionActivated(const Action *action);

private:
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 200;

	QString actionToString(const Action *action) const;
	bool isActionCheckable(const Action *action) const;

	static const QHash <BTech::MovementAction, QString> movementActionToString;
	static const QHash <BTech::CombatAction, QString> combatActionToString;
	static const QHash <BTech::MovementAction, bool> movementActionCheckable;
	static const QHash <BTech::CombatAction, bool> combatActionCheckable;

	void initLayout();

	QVBoxLayout *layout;
	QMap <QString, int> list;
	QList <ActionLabel *> actionList;

	const Action *currentAction;

	ActionLabel * getActionLabel(const Action *action);
};

/**
 * \class SideBar
 * A QDockWidget containing an image of an activated unit, ActionWindow with list of its actions and logWindow.
 */
class SideBar : public QDockWidget
{
Q_OBJECT;

public:
	SideBar();

	ActionWindow * getActionWindow();

	void keyPressEvent(QKeyEvent *event);

public slots:
	void enable();
	void disable();
	void insertMessage(const QString &message, const QColor &color = DefaultLogColor);

signals:
 	void endMoveButtonPressed();

private:
	static const QColor DefaultLogColor;

	static const int MAX_WIDTH = 300;
	static const int MAX_HEIGHT = 2000;
	static const int MIN_WIDTH = 300;
	static const int MIN_HEIGHT = 600;

	QImage *image;
	QVector <QString> log;

	QVBoxLayout *layout;
	QLabel *imageWindow;
	ActionWindow *actionWindow;
	QTextEdit *logWindow;
	QPushButton *endMoveButton;

	void initWindow();
	void initWidget();
	void initImageWindow();
	void initActionWindow();
	void initLogWindow();
	void initEndMoveButton();
	void initLayout();
};

#endif // SIDEBAR_H

