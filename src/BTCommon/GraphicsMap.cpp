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
#include "BTCommon/GraphicsEntity.h"
#include "BTCommon/GraphicsFactory.h"
#include "BTCommon/GraphicsHex.h"
#include "BTCommon/GraphicsMap.h"
#include "BTCommon/Grid.h"
#include "BTCommon/MechEntity.h"
#include "BTCommon/Player.h"
#include "BTCommon/TileManager.h"

GraphicsMap::GraphicsMap(Map *map)
	: map(map), valid(false)
{
	initScene();

	connect(map, &Map::hexesInitialized, this, &GraphicsMap::onHexesInitialized);
	connect(map, &Map::unitInitialized, this, &GraphicsMap::onUnitInitialized);

	connect(map, &Map::movementRangeNeeded, this, &GraphicsMap::onMovementRangeNeeded);
	connect(map, &Map::attackRangeNeeded, this, &GraphicsMap::onAttackRangeNeeded);
	connect(map, &Map::rangesNotNeeded, this, &GraphicsMap::onRangesNotNeeded);

	connect(map, &Map::playerTurn, this, &GraphicsMap::onPlayerTurn);

	connect(map, &Map::hexesNeedClearing, this, &GraphicsMap::onHexesNeedClearing);
	connect(map, &Map::hexesNeedUpdating, this, &GraphicsMap::onHexesNeedUpdating);
}

void GraphicsMap::newMap(int width, int height)
{
	if (valid)
		clearMap();
	map->newMap(width, height);
	initMap();
	valid = true;
	emit mapLoaded();
}

bool GraphicsMap::loadMap(const QString &mapFileName)
{
	if (valid)
		clearMap();
	if (!map->loadMap(mapFileName))
		return false;
	initMap();
// 	initUnits();
	valid = true;
	emit mapLoaded();
	return true;
}

bool GraphicsMap::isValid() const
{
	return valid;
}

void GraphicsMap::clearHexes()
{
	for (Hex *hex : map->getHexes())
		GraphicsFactory::get(hex)->setClicked(false);
}

void GraphicsMap::toggleGrid()
{
	GraphicsHex::setGridVisible(!isGridVisible());
	scene()->update();
}

bool GraphicsMap::isGridVisible() const
{
	return GraphicsHex::isGridVisible();
}

void GraphicsMap::toggleCoordinates()
{
	GraphicsHex::setCoordinatesVisible(!areCoordinatesVisible());
	scene()->update();
}

bool GraphicsMap::areCoordinatesVisible() const
{
	return GraphicsHex::areCoordinatesVisible();
}

QPointF GraphicsMap::mapCoordinateToScene(const Coordinate &c) const
{
	return coordinateToScenePos[c];
}

void GraphicsMap::incScale(int times)
{
	qreal newScale = finalScale;
	if (finalScale < scale)
		newScale = scale;
	newScale = qMin(newScale + scaleSpeed * times, maxScale);
	changeScale(newScale);
}

void GraphicsMap::decScale(int times)
{
	qreal newScale = finalScale;
	if (finalScale > scale)
		newScale = scale;
	newScale = qMax(newScale - scaleSpeed * times, minScale);
	changeScale(newScale);
}

void GraphicsMap::onChooseAction(const Action *action)
{
	hideAttackRange();
	hideMovementRange();
	map->chooseAction(action);
}

void GraphicsMap::onEndMove()
{
	map->endMove();
}

void GraphicsMap::initMap()
{
	walkRangeVisible = false;
	shootRangeVisible = false;

	initScaling();
	initWindowSettings();
}

void GraphicsMap::initScene()
{
	QGraphicsScene *scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
	setScene(scene);
}

void GraphicsMap::initHexes()
{
	const int leftBorder  = +GraphicsHex::getSize();
	const int upperBorder = -GraphicsHex::getSize();

	coordinateToScenePos.clear();
	for (Hex *hex : map->getHexes()) {
		GraphicsHex *graphicsHex = GraphicsFactory::get(hex);
		const qreal x = leftBorder + hex->getCoordinate().x() * GraphicsHex::getSize() * 3 / 2;
		const qreal y = upperBorder + (hex->getCoordinate().y() * 2 + (hex->getCoordinate().x() % 2 == 0)) * GraphicsHex::getSize();
		graphicsHex->setPos(x, y);
		graphicsHex->setTile(TileManager::tile(hex));

		coordinateToScenePos.insert(hex->getCoordinate(), graphicsHex->pos());
		scene()->addItem(graphicsHex);
		for (GridGraphicsObject *object : graphicsHex->getGridGraphicsObjects()) {
			scene()->addItem(object);
			object->setPos(graphicsHex->pos());
		}

		connect(graphicsHex, &GraphicsHex::activated,      this, &GraphicsMap::onHexClicked);
		connect(graphicsHex, &GraphicsHex::mouseEntered,   this, &GraphicsMap::onHexTracked);
		connect(graphicsHex, &GraphicsHex::mouseLeft,      this, &GraphicsMap::onHexAbandoned);
		connect(graphicsHex, &GraphicsHex::newAreaTracked, this, &GraphicsMap::onHexNewAreaTracked);
	}
}

void GraphicsMap::initUnits()
{
	for (Player *player : map->getPlayers())
		for (MechEntity *mech : player->getMechs())
			initUnit(mech);
}

void GraphicsMap::initScaling()
{
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	scale = 1;
	scaleSpeed = DEFAULT_SCALE_SPEED;

	qreal mDim = (qreal)(qMax(width(), height()));
	minScale = MIN_SCALE_MULT / mDim;
	maxScale = 3;
}

void GraphicsMap::initWindowSettings()
{
	QPointF firstHex = GraphicsFactory::get(map->getHexes().first())->pos();
	QPointF lastHex  = GraphicsFactory::get(map->getHexes().last())->pos();
	scene()->setSceneRect(
		firstHex.x() - SCENE_BORDER + GraphicsHex::getSize(),
		firstHex.y() - SCENE_BORDER,
		lastHex.x() - firstHex.x() + SCENE_BORDER * 2 - GraphicsHex::getSize() * 2,
		lastHex.y() - firstHex.y() + SCENE_BORDER * 2);

	setContentsMargins(0, 0, 0, 0);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMouseTracking(true);
	setEnabled(true);
}

void GraphicsMap::initUnit(MechEntity *mech)
{
	GraphicsEntity *gEnt = GraphicsFactory::get(mech);
	gEnt->init(this);
	scene()->addItem(gEnt);

	emit hexDisplayChanged();
}

void GraphicsMap::changeScale(qreal scale)
{
	finalScale = scale;

	QTimeLine *scaleAnim = new QTimeLine(SCALE_ANIM_TIME, this);
	scaleAnim->setUpdateInterval(SCALE_ANIM_INTERVAL);
	connect(scaleAnim, &QTimeLine::valueChanged, this, &GraphicsMap::scaleView);
	connect(scaleAnim, &QTimeLine::finished, this, &GraphicsMap::scaleAnimFinished);
	scaleAnim->start();
}

void GraphicsMap::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::RightButton) {
		int newX = horizontalScrollBar()->value() - event->x() + mousePosition.x();
		horizontalScrollBar()->setValue(newX);
		int newY = verticalScrollBar()->value() - event->y() + mousePosition.y();
		verticalScrollBar()->setValue(newY);
		QCursor::setPos(mapToGlobal(mousePosition));
	}

	QGraphicsView::mouseMoveEvent(event);
}

void GraphicsMap::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::RightButton) {
		setCursor(Qt::SizeAllCursor);
		mousePosition = event->pos();
	}
	QGraphicsView::mousePressEvent(event);
}

void GraphicsMap::mouseReleaseEvent(QMouseEvent *event)
{
	if (!(event->buttons() & Qt::RightButton))
		setCursor(Qt::ArrowCursor);

	QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsMap::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = qAbs(numDegrees / 15);
	if (numDegrees > 0)
		incScale(numSteps);
	else
		decScale(numSteps);
	event->accept();
}

void GraphicsMap::resizeEvent(QResizeEvent *event)
{
	initScaling();
}

void GraphicsMap::drawFriendlyMechs(const Player *player)
{
	for (MechEntity *mech : player->getMechs())
		mech->setFriendly(true);
}

void GraphicsMap::drawShootRange(const MechEntity *mech)
{
	Hex *mechHex = map->getGrid()->getHexByCoordinate(mech->getCurrentCoordinate());
	QList <Hex *> vArc = map->getGrid()->getShootRange(mechHex, mech->getTorsoDirection() + mech->getCurrentDirection());
	for (Hex *dest : vArc)
		dest->setAttackObject(map->getGrid()->getAttackObject(mech, dest->getMech()));
}

void GraphicsMap::drawWalkRange(const MovementObject &movement)
{
	QList <MoveObject> wRange = map->getGrid()->getWalkRange(movement);
	for (MoveObject &object : wRange)
		map->getGrid()->getHexByCoordinate(object.getDest().getCoordinate())->setMoveObject(object);
}

void GraphicsMap::hideAll()
{
	hideMovementRange();
	hideAttackRange();
	for (Hex *hex : map->getHexes())
		hex->clear();
	scene()->update();
}

void GraphicsMap::hideAttackRange()
{
	shootRangeVisible = false;
	for (Hex *hex : map->getHexes())
		hex->removeAttackObject();

	scene()->update();
}

void GraphicsMap::hideMovementRange()
{
	if (!walkRangeVisible)
		return;
	walkRangeVisible = false;
	for (Hex *hex : map->getHexes())
		hex->removeMoveObject();

	scene()->update();
}

void GraphicsMap::showAttackRange(const MechEntity *mech)
{
	if (mech == nullptr)
		return;

	drawShootRange(mech);
	shootRangeVisible = true;

	scene()->update();
}

void GraphicsMap::showMovementRange(const MovementObject &movement)
{
	hideMovementRange();
	drawWalkRange(movement);
	walkRangeVisible = true;

	scene()->update();
}

void GraphicsMap::onPlayerTurn()
{
	drawFriendlyMechs(map->getCurrentPlayer());
}

void GraphicsMap::onHexesNeedClearing()
{
	hideAll();
	scene()->update();
}

void GraphicsMap::onHexesNeedUpdating()
{
	scene()->update();
}

void GraphicsMap::clearMap()
{
	map->clearMap();
	valid = false;
	GraphicsFactory::clear();
	emit mapCleared();
}

void GraphicsMap::onHexesInitialized()
{
	initHexes();
}

void GraphicsMap::onUnitInitialized()
{
	initUnit(map->getCurrentUnit());
}

void GraphicsMap::onHexClicked(Hex *hex)
{
	emit hexClicked(hex);
	map->setCurrentHex(hex);
	map->trigger();
}

void GraphicsMap::onHexTracked(Hex *hex)
{
	emit hexDisplayStarted(hex);
}

void GraphicsMap::onHexAbandoned()
{
	emit hexDisplayQuit();
}

void GraphicsMap::onHexNewAreaTracked()
{
	emit hexDisplayChanged();
}

void GraphicsMap::onAttackRangeNeeded()
{
	showAttackRange(map->getCurrentUnit());
}

void GraphicsMap::onMovementRangeNeeded()
{
	showMovementRange(map->getCurrentMovement());
}

void GraphicsMap::onRangesNotNeeded()
{
	hideMovementRange();
	hideAttackRange();
}

void GraphicsMap::scaleView()
{
	qreal initScale = scale;
	if (scale > finalScale)
		scale = qMax(finalScale, scale - scaleSpeed * SCALE_ANIM_INTERVAL / SCALE_ANIM_TIME);
	else if (scale < finalScale)
		scale = qMin(finalScale, scale + scaleSpeed * SCALE_ANIM_INTERVAL / SCALE_ANIM_TIME);
	else
		return;

	QGraphicsView::scale(scale / initScale, scale / initScale);
	update();
}

void GraphicsMap::scaleAnimFinished()
{
	delete sender();
}
