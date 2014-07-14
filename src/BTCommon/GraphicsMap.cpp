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

const QHash <BTech::GamePhase, void (GraphicsMap::*)()> GraphicsMap::phaseToFunction {
	{BTech::GamePhase::None,           &GraphicsMap::noPhase},
	{BTech::GamePhase::Initiative,     &GraphicsMap::initiativePhase},
	{BTech::GamePhase::Movement,       &GraphicsMap::movementPhase},
	{BTech::GamePhase::Reaction,       &GraphicsMap::reactionPhase},
	{BTech::GamePhase::WeaponAttack,   &GraphicsMap::weaponAttackPhase},
	{BTech::GamePhase::PhysicalAttack, &GraphicsMap::physicalAttackPhase},
	{BTech::GamePhase::Combat,         &GraphicsMap::combatPhase},
	{BTech::GamePhase::Heat,           &GraphicsMap::heatPhase},
	{BTech::GamePhase::End,            &GraphicsMap::endPhase},
};

void GraphicsMap::createNewMap(int width, int height)
{
	if (mapLoaded)
		clearMap();
	Map::createNewMap(width, height);
	initMap();
	mapLoaded = true;
	emit mapHasBeenLoaded();
}

bool GraphicsMap::loadMap(const QString &mapFileName)
{
	if (mapLoaded)
		clearMap();
	if (!Map::loadMap(mapFileName))
		return false;
	initMap();
	mapLoaded = true;
	emit mapHasBeenLoaded();
	return true;
}

void GraphicsMap::clearHexes()
{
	for (Hex *hex : hexes)
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
	GraphicsHex::setCoordinatesVisible(!areCoordinatesVisible() && isGridVisible());
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

void GraphicsMap::addMechToHex(MechEntity *mech, Hex *hex, Player *player)
{
	if (hex->getMech() != nullptr) {
		delete mech;
		return;
	}
	mech->setPosition({hex->getCoordinate(), BTech::DirectionN});

	player->addMech(mech);
	hex->setMech(mech);
	initUnit(mech);

	emit hexDisplayChanged();
}

QPair <QString, QColor> GraphicsMap::getMessage() const
{
	return {message, messageColor};
}

QPair <QString, QColor> GraphicsMap::getExtInfo() const
{
	return {extensiveInfo, extensiveInfoColor};
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
	hideShootRange();
	hideWalkRange();
	chooseAction(action);
}

void GraphicsMap::onEndMove()
{
	endMove();
}

void GraphicsMap::initMap()
{
	walkRangeVisible = false;
	shootRangeVisible = false;

	initScene();
	initHexes();
	initUnits();
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
	const int leftBorder  = GraphicsHex::getSize();
	const int upperBorder = -GraphicsHex::getSize();

	coordinateToScenePos.clear();
	for (Hex *hex : hexes) {
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
	for (Player *player : players)
		for (MechEntity *mech : player->getMechs())
			initUnit(mech);
}

void GraphicsMap::initUnit(MechEntity *mech)
{
	GraphicsEntity *gEnt = GraphicsFactory::get(mech);
	gEnt->init(this);
	scene()->addItem(gEnt);

	connect(mech, &MechEntity::stateInfoSent, this, &GraphicsMap::mechStateInfoReceived);
	connect(mech, &MechEntity::infoSent, this, &GraphicsMap::mechInfoReceived);
	connect(mech, &MechEntity::extensiveInfoSent, this, &GraphicsMap::mechExtensiveInfoReceived);

	emit mechAdded(mech);
}

void GraphicsMap::initScaling()
{
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	scale = 1;
	scaleSpeed = DEFAULT_SCALE_SPEED;

	qreal mDim = (qreal)(qMax(getMapWidth(), getMapHeight()));
	minScale = MIN_SCALE_MULT / mDim;
	maxScale = 3;
}

void GraphicsMap::initWindowSettings()
{
	QPointF firstHex = GraphicsFactory::get(hexes.first())->pos();
	QPointF lastHex  = GraphicsFactory::get(hexes.last())->pos();
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
	Hex *mechHex = getGrid()->getHexByCoordinate(mech->getCurrentCoordinate());
	QList <Hex *> vArc = getGrid()->getShootRange(mechHex, mech->getTorsoDirection() + mech->getCurrentDirection());
	for (Hex *dest : vArc)
		dest->setAttackObject(getGrid()->getAttackObject(mech, dest->getMech()));
}

void GraphicsMap::drawWalkRange(const MovementObject &movement)
{
	QList <MoveObject> wRange = getGrid()->getWalkRange(movement);
	for (MoveObject &object : wRange)
		getGrid()->getHexByCoordinate(object.getDest().getCoordinate())->setMoveObject(object);
}

void GraphicsMap::hideAll()
{
	hideWalkRange();
	hideShootRange();
	for (Hex *hex : hexes)
		hex->clear();
}

void GraphicsMap::hideShootRange()
{
	shootRangeVisible = false;
	for (Hex *hex : hexes)
		hex->removeAttackObject();
}

void GraphicsMap::hideWalkRange()
{
	if (!walkRangeVisible)
		return;
	walkRangeVisible = false;
	for (Hex *hex : hexes)
		hex->removeMoveObject();
}

void GraphicsMap::showShootRange(const MechEntity *mech)
{
	if (mech == nullptr)
		return;

	drawShootRange(mech);
	shootRangeVisible = true;
}

void GraphicsMap::showWalkRange(const MovementObject &movement)
{
	hideWalkRange();
	drawWalkRange(movement);
	walkRangeVisible = true;
}

void GraphicsMap::noPhase()
{
	hideShootRange();
	hideWalkRange();
}

void GraphicsMap::emitGameStarted()
{
	emit gameStarted();
}

void GraphicsMap::emitGameEnded()
{
	emit gameEnded();
}

void GraphicsMap::emitMechInfoNeeded(const MechEntity *mech)
{
	emit mechInfoNeeded(mech);
}

void GraphicsMap::emitMechInfoNotNeeded()
{
	emit mechInfoNotNeeded();
}

void GraphicsMap::emitMechActionsNeeded(BTech::GamePhase phase)
{
	emit mechActionsNeeded(phase);
}

void GraphicsMap::emitMechActionsNotNeeded()
{
	emit mechActionsNotNeeded();
}

void GraphicsMap::emitMechWalkRangeNeeded(const MovementObject &movement)
{
	showWalkRange(movement);
	scene()->update();
}

void GraphicsMap::emitMechShootRangeNeeded(const MechEntity *mech)
{
	showShootRange(mech);
	scene()->update();
}

void GraphicsMap::emitMechRangesNotNeeded()
{
	hideWalkRange();
	hideShootRange();
	scene()->update();
}

void GraphicsMap::emitPlayerTurn(const Player *player)
{
	drawFriendlyMechs(player);
	emit playerTurn(player);
}

void GraphicsMap::emitHexesNeedClearing()
{
	hideAll();
	scene()->update();
}

void GraphicsMap::emitHexesNeedUpdating()
{
	scene()->update();
}

void GraphicsMap::emitMessageSent(const QString &message, const QColor &color)
{
	this->message = message;
	this->messageColor = color;
	emit messageSent();
}

void GraphicsMap::emitExtensiveInfoSent(const QString &info, const QColor &color)
{
	this->extensiveInfo = info;
	this->extensiveInfoColor = color;
	emit extensiveInfoSent();
}

void GraphicsMap::clearMap()
{
	Map::clearMap();
	mapLoaded = false;
	GraphicsFactory::clear();
	emit mapCleared();
}

void GraphicsMap::onHexClicked(Hex *hex)
{
	emit hexClicked(hex);
	setCurrentHex(hex);
	(this->*GraphicsMap::phaseToFunction[getCurrentPhase()])();
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

void GraphicsMap::mechInfoReceived()
{
	MechEntity *mech = static_cast<MechEntity *>(sender());
	emitMessageSent(mech->getName() + ": " + mech->getInfo(),
	                playerNameToColor[mech->getOwnerName()]);
}

void GraphicsMap::mechExtensiveInfoReceived()
{
	MechEntity *mech = static_cast<MechEntity *>(sender());
	if (mech->getInfo().isEmpty())
		emitExtensiveInfoSent(mech->getName() + " (" + mech->getOwnerName() + ")",
		                      playerNameToColor[mech->getOwnerName()]); /** signature */
	else
		emitExtensiveInfoSent(mech->getInfo());
}

void GraphicsMap::mechStateInfoReceived(const QString &message)
{
	emitMessageSent(getCurrentMech()->getOwnerName() + ": " + getCurrentMech()->getName() + ": " + message,
	                playerNameToColor[getCurrentMech()->getOwnerName()]);
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
