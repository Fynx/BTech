/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
Copyright (C) 2014 by Bartosz Szreder <szreder [at] mimuw [dot] edu [dot] pl>
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
#include "BTCommon/Position.h"

class Entity;
class MechEntity;

/**
 * \class GraphicsEntity
 * This class is a QGraphicsObject with an Entity attached.
 * It provides Entity with all display functions and QGraphicsScene coordinates' control and maintenance.
 * What is displayed is entirely dependent on entity's functions and state.
 */
class GraphicsEntity : public QGraphicsObject
{
Q_OBJECT;

public:
	GraphicsEntity(Entity *entity);

	void init(const CoordinateMapper *coordinateMapper);
	void setStraightSpeed(int speed);
	int getStraightSpeed() const;
	void setRotationSpeed(int angleSpeed);
	int getRotationSpeed() const;

signals:
	void movementStarted();
	void movementEnded();

protected:
	virtual void paintEntity(QPainter *painter) = 0;
	virtual void staticAdvance() = 0;
	virtual void dynamicAdvance() = 0;

	Entity *entity;

private:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	QPointF nextPoint(const QPointF &src, const QPointF &dest) const;
	int nextRotation(int src, int dest) const;

	bool isInMove() const;

	void countMidPoints();

	QList <Position> path;

	QPointF destPosition;
	int destRotation;

	static const int DEFAULT_ROTATION_SPEED = 10;
	static const int DEFAULT_STRAIGHT_SPEED = 8;

	int straightSpeed;
	int rotationSpeed;

	static const int TIMER_INTERVAL = 70;

	QTimer *timer;
	const CoordinateMapper *coordinateMapper;

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
