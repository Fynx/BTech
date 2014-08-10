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

#include "BTCommon/EnumHashFunctions.h"
#include "BTCommon/GraphicsFactory.h"
#include "BTCommon/GraphicsHex.h"
#include "BTCommon/MechEntity.h"
#include "BTCommon/Tile.h"

bool GraphicsHex::gridVisible = false;
bool GraphicsHex::coordinatesVisible = false;

// TODO yeah, the potato aspect of hexness
const QPointF GraphicsHex::nodes[BTech::NODES_NUMBER] {
	{ DEFAULT_HEX_SIZE / 2, -DEFAULT_HEX_SIZE},
	{ DEFAULT_HEX_SIZE,      0},
	{ DEFAULT_HEX_SIZE / 2,  DEFAULT_HEX_SIZE},
	{-DEFAULT_HEX_SIZE / 2,  DEFAULT_HEX_SIZE},
	{-DEFAULT_HEX_SIZE,      0},
	{-DEFAULT_HEX_SIZE / 2, -DEFAULT_HEX_SIZE},
};

const QColor GraphicsHex::Color::Grid                    = Qt::black;
const QColor GraphicsHex::Color::Clicked                 = Qt::gray;
const QColor GraphicsHex::Color::UnderMouse              = Qt::green; // TODO I want something else
const QColor GraphicsHex::Color::HasMoveObject           = Qt::yellow;
const QColor GraphicsHex::Color::MoveObjectActionNotIdle = BTech::Colors::LightGolden;
const QColor GraphicsHex::Color::MoveObjectActionIdle    = BTech::Colors::Crimson;
const QColor GraphicsHex::Color::HasAttackObject         = Qt::red;
const QColor GraphicsHex::Color::UnitMoved               = Qt::gray;
const QColor GraphicsHex::Color::UnitNotMoved            = Qt::green;
const QColor GraphicsHex::Color::UnitActive              = Qt::darkGreen;

const QColor GraphicsHex::Color::CoordinatesBackground   = Qt::black;
const QColor GraphicsHex::Color::CoordinatesText         = Qt::green;

/* static member */
void GraphicsHex::setGridVisible(bool visible)
{
	gridVisible = visible;
}

/* static member */
bool GraphicsHex::isGridVisible()
{
	return gridVisible;
}

/* static member */
void GraphicsHex::setCoordinatesVisible(bool visible)
{
	coordinatesVisible = visible;
}

/* static memmber */
bool GraphicsHex::areCoordinatesVisible()
{
	return coordinatesVisible;
}

/* static member */
void GraphicsHex::setSize(int newSize)
{
	// Methinks this function is baad and unnecessary
}

/* static member */
int GraphicsHex::getSize()
{
	return DEFAULT_HEX_SIZE;
}

GraphicsHex::GraphicsHex(Hex *hex)
	: hex(hex), tile(nullptr)
{
	setParent(hex);
	init();
	setEnabled(true);
	setAcceptHoverEvents(true);
	addGridGraphicsObjects();
}

GraphicsHex::~GraphicsHex()
{
	qDeleteAll(getGridGraphicsObjects());
	GraphicsFactory::erase(this);
}

QList <GridGraphicsObject *> GraphicsHex::getGridGraphicsObjects()
{
	return { graphicsObjects.base,
	         graphicsObjects.property,
	         graphicsObjects.mouseClick,
	         graphicsObjects.mouseMove };
}

QPointF GraphicsHex::getNode(int nodeNumber) const
{
	return nodes[(nodeNumber + BTech::NODES_NUMBER) % BTech::NODES_NUMBER];
}

QPointF GraphicsHex::getAbsNode(int nodeNumber) const
{
	return getNode(nodeNumber) + pos().toPoint();
}

void GraphicsHex::setClicked(bool clicked)
{
	this->clicked = clicked;
	QGraphicsItem::update();
}

bool GraphicsHex::isClicked() const
{
	return clicked;
}

void GraphicsHex::setTracked(bool tracked)
{
	this->tracked = tracked;
	update();
}

bool GraphicsHex::isTracked() const
{
	return tracked;
}

int GraphicsHex::getTrackedArea() const
{
	return trackedArea;
}

void GraphicsHex::init()
{
	setClicked(false);
	setTracked(false);
}

void GraphicsHex::clear()
{
	hex->clear();
	setClicked(false);
	setTracked(false);
	QGraphicsItem::update();
}

void GraphicsHex::addGridGraphicsObjects()
{
	graphicsObjects.base =
		new GridGraphicsObject(this,
		                       &GraphicsHex::paintBaseGrid,
		                       graphicsObjects.BaseZValue);
	graphicsObjects.property =
		new GridGraphicsObject(this,
		                       &GraphicsHex::paintPropertyGrid,
		                       graphicsObjects.PropertyZValue);
	graphicsObjects.mouseClick =
		new GridGraphicsObject(this,
		                       &GraphicsHex::paintMouseClickDependentGrid,
		                       graphicsObjects.MouseClickZValue);
	graphicsObjects.mouseMove =
		new GridGraphicsObject(this,
		                       &GraphicsHex::paintMouseMoveDependentGrid,
		                       graphicsObjects.MouseMoveZValue);
}

QRectF GraphicsHex::boundingRect() const
{
	return QRectF(-getSize(),     -getSize(),
	               getSize() * 2,  getSize() * 2);
}

QPainterPath GraphicsHex::shape() const
{
	QPainterPath path;
	QPolygonF polygon;
	for (int i = 0; i < BTech::NODES_NUMBER; ++i)
		polygon << getNode(i);
	path.addPolygon(polygon);
	return path;
}

const Tile * GraphicsHex::getTile() const
{
	return this->tile;
}

void GraphicsHex::setTile(const Tile *tile)
{
	this->tile = tile;
}

void GraphicsHex::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setBrush(BTech::terrainColorMap[hex->getTerrain()]);
	painter->setPen(Qt::NoPen);
	painter->drawPath(shape());

	if (tile != nullptr)
		painter->drawImage(QRect(-DEFAULT_HEX_SIZE, -DEFAULT_HEX_SIZE, 2 * DEFAULT_HEX_SIZE, 2 * DEFAULT_HEX_SIZE), tile->getCurrentFrame());

	if (areCoordinatesVisible()) {
		painter->setBrush(Color::CoordinatesBackground);
		painter->setPen(Color::CoordinatesBackground);
		painter->drawRect(QRect(-getSize() / 2, getSize() / 4, getSize(), getSize() / 2));
		painter->setPen(Color::CoordinatesText);
		painter->drawText(-getSize() / 2, getSize() / 4, getSize(), getSize() / 2, Qt::AlignCenter,
			QString("%1, %2").arg(hex->getCoordinate().x()).arg(hex->getCoordinate().y()));
	}
}

void GraphicsHex::paintBaseGrid(QPainter *painter)
{
	if (isGridVisible())
		paintBorder(painter, 3, Color::Grid);
}

void GraphicsHex::paintPropertyGrid(QPainter *painter)
{
	if (hex->hasMoveObject())
		paintBorder(painter, 2, Color::HasMoveObject);
	if (hex->isAttackable())
		paintBorder(painter, 2, Color::HasAttackObject);
	if (hex->getMech() != nullptr) {
		if (hex->getMech()->isFriendly()) {
			if (hex->getMech()->isMoved())
				paintBorder(painter, 2, Color::UnitMoved);
			else
				paintBorder(painter, 2, Color::UnitNotMoved);
		}

		if (hex->getMech()->isActive())
			paintBorder(painter, 3, Color::UnitActive);
	}
}

void GraphicsHex::paintMouseClickDependentGrid(QPainter *painter)
{
	if (isClicked())
		paintBorder(painter, 3, Color::Clicked);
}

void GraphicsHex::paintMouseMoveDependentGrid(QPainter *painter)
{
	if (isTracked()) {
		paintBorder(painter, 3, Color::UnderMouse);
		if (hex->hasMoveObject()) {
			QPolygonF polygon;
			polygon << QPointF(0, 0) << getNode(trackedArea - 1) << getNode(trackedArea);
			QPainterPath path;
			path.addPolygon(polygon);
			painter->setPen(Qt::NoPen);
			if (hex->getMoveObject().getActionType() == BTech::ActionType::Idle)
				painter->setBrush(Color::MoveObjectActionIdle);
			else
				painter->setBrush(Color::MoveObjectActionNotIdle);
			painter->drawPath(path);
		}
	}
}

void GraphicsHex::paintBorder(QPainter *painter, int width, const QColor &color)
{
	QPen pen;
	pen.setWidth(width);
	pen.setColor(color);
	painter->setPen(pen);

	QPointF node[2];
	for (int i = 0; i < BTech::NODES_NUMBER; ++i) {
		node[0] = getNode(i);
		node[1] = getNode(i + 1);
		painter->drawLine(node[0].x(), node[0].y(), node[1].x(), node[1].y());
	}
}

void GraphicsHex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit activated(hex);
	event->accept();
}

void GraphicsHex::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	setTracked(true);
	emit mouseEntered(hex);
	event->accept();
}

void GraphicsHex::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	setTracked(false);
	trackedArea = -1;
	emit mouseLeft(hex);
	event->accept();
}

void GraphicsHex::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	qreal x = event->pos().x();
	qreal y = event->pos().y();
	event->accept();
	if (!hex->hasMoveObject() || qAbs(y) < 2)
		return;
	BTech::Matrix auxCur, auxPrev;
	int i = 0;
	do {
		++i;
		auxPrev = auxCur;
		auxCur = BTech::Matrix(x, y, getNode(i).x(), getNode(i).y());
	} while (i < 2 || auxPrev.det() < 0 || auxCur.det() >= 0);
	int prevHexTracked = trackedArea;
	trackedArea = i % BTech::NODES_NUMBER;
	if (trackedArea != prevHexTracked) {
		hex->setMoveObject(trackedArea);
		emit newAreaTracked(hex);
	}
	QGraphicsItem::update();
}

/**
 * \class GridGraphicsObject
 */

GridGraphicsObject::GridGraphicsObject(GraphicsHex *parent, void (GraphicsHex::*paintFunction)(QPainter *painter), int zValue)
	: hex(parent), paintFunction(paintFunction)
{
	setParent(parent);
	setZValue(zValue);
}

QRectF GridGraphicsObject::boundingRect() const
{
	return hex->boundingRect();
}

QPainterPath GridGraphicsObject::shape() const
{
	return hex->shape();
}

void GridGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	return (hex->*paintFunction)(painter);
}
