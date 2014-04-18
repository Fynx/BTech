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

#ifndef GRAPHICSMAP_H
#define GRAPHICSMAP_H

#include <QtWidgets>
#include "BTCommon/GraphicsFactory.h"
#include "BTCommon/Map.h"

/**
 * \class GraphicsMap
 * Provides game-based Map with display functions and QObject-like functionalities.
 */
class GraphicsMap : public QGraphicsView, public Map
{
Q_OBJECT;

public:
	GraphicsMap();

	void createNewMap(int width, int height);
	bool loadMap(const QString &mapFileName);

	void toggleGrid();
	bool isGridVisible() const;
	void toggleCoordinates();
	bool areCoordinatesVisible() const;

	void addMechToHex(MechEntity *mech, Hex *hex, Player *player);

	QPair <QString, QColor> getMessage() const;
	QPair <QString, QColor> getExtInfo() const;

public slots:
	void incScale(int steps = 1);
	void decScale(int steps = 1);

	void onChooseAction(const Action *action);
	void onEndMove();

	void clearHexes();

signals:
	void mapHasBeenLoaded();
	void mapCleared();

	void hexClicked(Hex *hex);
	void hexDisplayStarted(const Hex *hex);
	void hexDisplayQuit();
	void hexDisplayChanged();
	void mechAdded(MechEntity *mech);

	void gameStarted();
	void gameEnded();
	void mechInfoNeeded(const MechEntity *mech);
	void mechInfoNotNeeded();
	void mechActionsNeeded(BTech::GamePhase phase);
	void mechActionsNotNeeded();
	void playerTurn(const Player *player);

	void messageSent();
	void extensiveInfoSent();

private:
	void initMap();

	void initScene();
	void initGrid();
	void initHexes();
	void initUnits();
	void initUnit(MechEntity *mech);
	void initScaling();
	void initWindowSettings();

	static const int SCENE_BORDER = 200;					/**< Empty space from each side that separates Hexes from the border of the scene. */
	static constexpr qreal DEFAULT_SCALE_SPEED = 0.1;			/**< Default zoom speed. */
	static constexpr qreal MAX_SCALE_MULT = 6;				/**< Max zoom multiplier. */
	static constexpr qreal MIN_SCALE_MULT = 0.1;				/**< Min zoom multiplier. */
	static constexpr qreal SCALE_ANIM_SPEED = 0.02;				/**< Speed of zooming animation. */
	static const int SCALE_ANIM_TIME = 600;					/**< Time of single zoom animation. */
	static const int SCALE_ANIM_INTERVAL = 20;				/**< TIme between zoom animation steps. */

	Grid *grid;

	qreal maxScale;								/**< Max zoom level. */
	qreal minScale;								/**< Min zoom level. */
	qreal scale;								/**< Current zoom level. */
	qreal scaleSpeed;							/**< Current speed of zooming. */
	qreal finalScale;							/**< Destinated zoom level in the animation. */
	QPoint mousePosition;							/**< Current mouse position in the view. */

	void changeScale(qreal scale);						/**< Changes finalScale to scale. */
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void resizeEvent(QResizeEvent *event);

	void noPhase();

	void emitGameStarted();
	void emitGameEnded();
	void emitMechInfoNeeded(const MechEntity *mech);
	void emitMechInfoNotNeeded();
	void emitMechActionsNeeded(BTech::GamePhase phase);
	void emitMechActionsNotNeeded();
	void emitMechWalkRangeNeeded(const MovementObject &movement);
	void emitMechShootRangeNeeded(const MechEntity *mech);
	void emitMechRangesNotNeeded();
	void emitPlayerTurn(const Player *player);
	void emitHexesNeedClearing();
	void emitHexesNeedUpdating();
	void emitMessageSent(const QString &message, const QColor &color = DefaultMessageColor);
	void emitExtensiveInfoSent(const QString &message, const QColor &color = DefaultMessageColor);

	void clearMap();

	QString message;
	QColor messageColor;

	QString extensiveInfo;
	QColor extensiveInfoColor;

	static const QHash <BTech::GamePhase, void (GraphicsMap::*)()> phaseToFunction;

private slots:
	void hexClicked(int hexNumber);
	void hexTracked(int hexNumber);
	void hexAbandoned(int hexNumber);
	void hexNewAreaTracked(int hexNumber);

	void mechInfoReceived();
	void mechExtensiveInfoReceived();
	void mechStateInfoReceived(const QString &message);

	void scaleView();
	void scaleAnimFinished();
};

#endif // GRAPHICSMAP_H
