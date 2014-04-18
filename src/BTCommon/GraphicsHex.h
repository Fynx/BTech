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

#ifndef GRAPHICS_HEX_H
#define GRAPHICS_HEX_H

#include <QtWidgets>
#include "BTCommon/Hex.h"
#include "BTCommon/Utils.h"

class GridGraphicsObject;

class GraphicsHex : public QGraphicsObject
{
Q_OBJECT;

public:
	static void setSize(int size);
	static int getSize();
	static void setGridVisible(bool visible);
	static bool isGridVisible();
	static void setCoordinatesVisible(bool visible);
	static bool areCoordinatesVisible();

	GraphicsHex(Hex *hex);
	~GraphicsHex();

	QList <GridGraphicsObject *> getGridGraphicsObjects();
	QPoint getNode(int nodeNumber) const;
	QPoint getAbsNode(int nodeNumber) const;

	void setClicked(bool clicked);
	bool isClicked() const;
	void setTracked(bool tracked);
	bool isTracked() const;
	int getTrackedArea() const;

	void init();
	void clear();
	void addGridGraphicsObjects();

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option = nullptr, QWidget *widget = nullptr);
	void paintBaseGrid(QPainter *painter);
	void paintPropertyGrid(QPainter *painter);
	void paintMouseClickDependentGrid(QPainter *painter);
	void paintMouseMoveDependentGrid(QPainter *painter);

signals:
	void activated(int);
	void mouseEntered(int);
	void mouseLeft(int);
	void newAreaTracked(int);

private:
	GridGraphicsObject *baseGridGraphicsObject;
	GridGraphicsObject *propertyGridGraphicsObject;
	GridGraphicsObject *mouseClickDependentGridGraphicsObject;
	GridGraphicsObject *mouseMoveDependentGridGraphicsObject;

	static const int DEFAULT_HEX_SIZE = 60;
	static const int MIN_HEX_SIZE     = 10;
	static const int MAX_HEX_SIZE     = 100;

	static const int BASE_GRID_GRAPHICS_OBJECT_Z_VALUE                  = 1;
	static const int PROPERTY_GRID_GRAPHICS_OBJECT_Z_VALUE              = 2;
	static const int MOUSE_CLICK_DEPENDENT_GRID_GRAPHICS_OBJECT_Z_VALUE = 3;
	static const int MOUSE_MOVE_DEPENDENT_GRID_GRAPHICS_OBJECT_Z_VALUE  = 4;

	struct Color {
		static const QColor Grid;
		static const QColor Clicked;
		static const QColor UnderMouse;
		static const QColor HasMoveObject;
		static const QColor MoveObjectActionIdle;
		static const QColor MoveObjectActionNotIdle;
		static const QColor HasAttackObject;
		static const QColor UnitMoved;
		static const QColor UnitNotMoved;
		static const QColor UnitActive;

		static const QColor CoordinatesBackground;
		static const QColor CoordinatesText;
	};

	Hex *hex;

	static const QPoint nodes[BTech::NODES_NUMBER];

	static bool gridVisible;
	static bool coordinatesVisible;

	void paintBorder(QPainter *painter, int width, const QColor &color);

	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

	bool clicked;
	bool tracked;
	int trackedArea;
};

class GridGraphicsObject : public QGraphicsObject
{

public:
	GridGraphicsObject(GraphicsHex *parent, void (GraphicsHex::*)(QPainter *painter), int zValue);

private:
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option = nullptr, QWidget *widget = nullptr);

	GraphicsHex *hex;
	void (GraphicsHex::*paintFunction)(QPainter *painter);
};

#endif // GRAPHICS_HEX_H
