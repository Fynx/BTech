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

#ifndef GRAPHICSENTITY_H
#define GRAPHICSENTITY_H

#include <QtWidgets>
#include "BTCommon/Objects.h"
#include "BTCommon/MechEntity.h" // TODO this should be eradicated from here, along with class GraphicsMech

/**
 * \class GraphicsEntity
 * This class is a QGraphicsObject with an Entity attached.
 * It provides Entity with all display functions and QGraphicsScene coordinates' control and maintenance.
 * What is displayed is entirely dependent on entity's functions and state.
 * To make this class properly, PathFinder must be set with static function setPathFinder.
 */
class GraphicsEntity : public QGraphicsObject
{
Q_OBJECT;

public:
	static void setPathFinder(PathFinder *pathFinder);

	GraphicsEntity(Entity *entity = nullptr);

	void setEntity(Entity *entity);
	void setStraightSpeed(int speed);
	int getStraightSpeed() const;
	void setRotationSpeed(int angleSpeed);
	int getRotationSpeed() const;

public slots:
	void init(QPoint position);

signals:
	void movementStarted();
	void movementEnded();

protected:
	virtual void paintEntity(QPainter *painter) = 0;
	virtual void staticAdvance() = 0;
	virtual void dynamicAdvance() = 0;

	Entity *entity;

private:
	static const PathFinder *pathFinder;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	QPoint nextPoint(const QPoint &src, const QPoint &dest) const;
	int nextRotation(int src, int dest) const;

	bool isInMove() const;

	void countMidPoints();

	QList <Position> path;

	QPoint destPoint;
	int destRotation;

	static const int DEFAULT_ROTATION_SPEED = 10;
	static const int DEFAULT_STRAIGHT_SPEED = 8;

	int straightSpeed;
	int rotationSpeed;

	static const int TIMER_INTERVAL = 70;

	QTimer *timer;

private slots:
	void startMovement();
	void advance();

	void adjustCoordinates();
	void adjustRotation();
};

class GraphicsMech : public GraphicsEntity
{
Q_OBJECT;

public:
	GraphicsMech(MechEntity *entity);

private:
	virtual QRectF boundingRect() const;

	virtual void paintEntity(QPainter *painter);
	virtual void staticAdvance();
	virtual void dynamicAdvance();
};

#endif // GRAPHICSENTITY_H
