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

#include "BTCommon/GraphicsEntity.h"
#include "BTCommon/MechEntity.h"
#include "BTCommon/Objects.h"

/**
 * \class GraphicsEntity
 */

/* constructor */
GraphicsEntity::GraphicsEntity(Entity *entity)
	: entity(entity)
{
	setParent(entity);
	this->entity = entity;

	connect(entity, &Entity::coordinateSet, this, &GraphicsEntity::adjustCoordinates);
	connect(entity, &Entity::directionSet,  this, &GraphicsEntity::adjustRotation);

	/**
	 * The funny thing is that even if the entity is Movable, only the signal directionChanged will start the
	 * animation. This is because functions setDestinationRotation/PositionNumber are called in order:
	 * positionNumber, rotation.
	 * So, the signal positionChanged is ignored.
	 */

	if (entity->isRotable())
		connect(static_cast<Rotable *>(entity), &Rotable::directionChanged, this, &GraphicsEntity::startMovement);

	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &GraphicsEntity::advance);
	setRotationSpeed(DEFAULT_ROTATION_SPEED);
	setStraightSpeed(DEFAULT_STRAIGHT_SPEED);
}

void GraphicsEntity::init(const CoordinateMapper *coordinateMapper)
{
	this->coordinateMapper = coordinateMapper;
	destPosition = coordinateMapper->mapCoordinateToScene(entity->getCurrentCoordinate());
	setPos(destPosition);
	destRotation = entity->getCurrentDirection().toAngle();
	setRotation(destRotation);

	update();
}

void GraphicsEntity::setStraightSpeed(int speed)
{
	straightSpeed = speed;
}

int GraphicsEntity::getStraightSpeed() const
{
	return straightSpeed;
}

void GraphicsEntity::setRotationSpeed(int angleSpeed)
{
	rotationSpeed = angleSpeed;
}

int GraphicsEntity::getRotationSpeed() const
{
	return rotationSpeed;
}

void GraphicsEntity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (!isInMove())
		staticAdvance();
	paintEntity(painter);
}

QPointF GraphicsEntity::nextPoint(const QPointF &src, const QPointF &dest) const
{
	qreal x = dest.x() - src.x();
	qreal y = dest.y() - src.y();

	if (qAbs(x) <= straightSpeed && qAbs(y) <= straightSpeed)
		return dest;

	qreal arctan = qAtan2(x, y);

	return QPointF(qSin(arctan) * straightSpeed, qCos(arctan) * straightSpeed) + src;
}

int GraphicsEntity::nextRotation(int src, int dest) const
{
	int s = (src  + BTech::Math::FULL_ANGLE) % BTech::Math::FULL_ANGLE;
	int d = (dest + BTech::Math::FULL_ANGLE) % BTech::Math::FULL_ANGLE;

	int distance = s - d;
	int difference = qMin(( distance + BTech::Math::FULL_ANGLE) % BTech::Math::FULL_ANGLE,
			      (-distance + BTech::Math::FULL_ANGLE) % BTech::Math::FULL_ANGLE);

	if (difference <= rotationSpeed)
		return d;

	if ((s > d) == (qAbs(distance) > BTech::Math::STRAIGHT_ANGLE))
		return s + rotationSpeed;
	else
		return s - rotationSpeed;
}

bool GraphicsEntity::isInMove() const
{
	if (!entity->isRotable())
		return false;
	else
		return (static_cast<const Rotable *>(entity))->isInMove();
}

void GraphicsEntity::countMidPoints()
{
	MoveObject moveObject = (static_cast<Rotable *>(entity))->getMoveObject();
	path = moveObject.getPath();
}

void GraphicsEntity::startMovement()
{
	countMidPoints();
	timer->start(TIMER_INTERVAL);
}

void GraphicsEntity::advance()
{
	Q_ASSERT(entity->isRotable());

	dynamicAdvance();

	Rotable *rotableEntity = static_cast<Rotable *>(entity);

	if (!rotableEntity->isInMove()) {
		timer->stop();
		emit movementEnded();
		return;
	}

	if (rotation() != destRotation)
		setRotation(nextRotation(rotation(), destRotation));

	if (pos() != destPosition) {
		QPointF curPoint = pos();
		curPoint = nextPoint(curPoint, destPosition);
		setPos(curPoint);
	}

	if (rotation() == destRotation && pos() == destPosition) {
		if (!path.empty()) {
			destPosition = coordinateMapper->mapCoordinateToScene(path.first().getCoordinate());
			destRotation = path.first().getDirection().toAngle();
			path.removeFirst();
		} else {
			rotableEntity->reachDestination();
		}
	}

	update();
}

void GraphicsEntity::adjustCoordinates()
{
	setPos(coordinateMapper->mapCoordinateToScene(entity->getCurrentCoordinate()));
	update();
}

void GraphicsEntity::adjustRotation()
{
	setRotation(entity->getCurrentDirection() * BTech::Math::HEX_ANGLE);
	update();
}

/**
 * \class GraphicsMech
 */

/**
 * WARNING
 * Do not manipulate with coordinates or rotation, or you'll likely get sort of fuckup you won't be able to recover
 * from. GraphicsEntity will take care of that.
 * However, you can create variable rotationChange and use rotation + rotationChange instead of rotation.
 */

GraphicsMech::GraphicsMech(MechEntity *entity)
	: GraphicsEntity(entity)
{}

QRectF GraphicsMech::boundingRect() const
{
	return QRectF(-20, -20, 40, 40);
}

void GraphicsMech::paintEntity(QPainter *painter)
{
	int size = 20;

	painter->save();

	/** Base mech */

	painter->setPen(Qt::darkGray);
	painter->setBrush(Qt::lightGray);
	painter->drawRect(-size, -size, size * 2, size * 2);

	/** Mech torso (perhaps torso can be treated as Rotable...?) */

	painter->rotate(((MechEntity *)entity)->getTorsoDirection() * BTech::Math::HEX_ANGLE);

	painter->setPen(Qt::darkGray);
	painter->setBrush(Qt::lightGray);
	painter->drawRect(-size / 2, -size / 2, size, size);

	painter->setPen(Qt::black);
	painter->setBrush(Qt::black);
	QPainterPath path;
	QPolygon polygon;
	polygon << QPoint(-4, 0) << QPoint(0, -7) << QPoint(4, 0);
	path.addPolygon(polygon);
	painter->drawPath(path);

	painter->restore();
}

void GraphicsMech::staticAdvance()
{}

void GraphicsMech::dynamicAdvance()
{}
