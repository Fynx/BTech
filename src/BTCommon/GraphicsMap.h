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

#ifndef GRAPHICSMAP_H
#define GRAPHICSMAP_H

#include <QtWidgets>
#include "BTCommon/Map.h"
#include "BTCommon/Position.h"

class Grid;
class Hex;
class MechEntity;
class MovementObject;
class Player;

/**
 * \class GraphicsMap
 * Provides game-based Map with display functions and QObject-like functionalities.
 */
class GraphicsMap : public QGraphicsView, public CoordinateMapper
{
Q_OBJECT;

public:
	GraphicsMap(Map *map);

	void newMap(int width, int height);
	bool loadMap(const QString &mapFileName);
	bool isValid() const;

	void clearHexes();
	void toggleGrid();
	bool isGridVisible() const;
	void toggleCoordinates();
	bool areCoordinatesVisible() const;
	QPointF mapCoordinateToScene(const Coordinate &c) const;

	void addMechToHex(MechEntity *mech, Hex *hex, Player *player);

public slots:
	void incScale(int steps = 1);
	void decScale(int steps = 1);

	void onChooseAction(const Action *action);
	void onEndMove();

signals:
	void mapLoaded();
	void mapCleared();

	void hexClicked(Hex *hex);
	void hexDisplayStarted(const Hex *hex);
	void hexDisplayQuit();
	void hexDisplayChanged();

private:
	void initMap();
	void initScene();
	void initHexes();
	void initUnits();
	void initScaling();
	void initWindowSettings();

	void initUnit(MechEntity *mech);

	void changeScale(qreal scale);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void resizeEvent(QResizeEvent *event);

	void drawFriendlyMechs(const Player *player);
	void drawShootRange(const MechEntity *mech);
	void drawWalkRange(const MovementObject &movement);
	void hideAll();
	void hideAttackRange();
	void hideMovementRange();
	void showAttackRange(const MechEntity *mech);
	void showMovementRange(const MovementObject &movement);

	void clearMap();

	static const int SCENE_BORDER              = 200;
	static constexpr qreal DEFAULT_SCALE_SPEED = 0.1;
	static constexpr qreal MAX_SCALE_MULT      = 6;
	static constexpr qreal MIN_SCALE_MULT      = 0.1;
	static const int SCALE_ANIM_TIME           = 200;
	static const int SCALE_ANIM_INTERVAL       = 20;

	QHash <Coordinate, QPointF> coordinateToScenePos;

	Map *map;

	qreal maxScale;
	qreal minScale;
	qreal scale;
	qreal scaleSpeed;
	qreal finalScale;
	QPoint mousePosition;

	bool shootRangeVisible;
	bool walkRangeVisible;

	bool valid;

private slots:
	void onHexesInitialized();
	void onUnitInitialized();

	void onHexClicked(Hex *hex);
	void onHexTracked(Hex *hex);
	void onHexAbandoned();
	void onHexNewAreaTracked();

	void onAttackRangeNeeded();
	void onMovementRangeNeeded();
	void onRangesNotNeeded();

	void onPlayerTurn();

	void onHexesNeedClearing();
	void onHexesNeedUpdating();

	void scaleView();
	void scaleAnimFinished();
};

#endif // GRAPHICSMAP_H
