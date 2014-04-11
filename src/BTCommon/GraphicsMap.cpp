#include "BTCommon/EnumHashFunctions.h"
#include "BTCommon/GraphicsMap.h"

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

GraphicsMap::GraphicsMap()
{}

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

void GraphicsMap::toggleGrid()
{
	grid->toggleGrid();
	scene()->update();
}

bool GraphicsMap::isGridVisible() const
{
	return grid->isGridVisible();
}

void GraphicsMap::toggleCoordinates()
{
	grid->toggleCoordinates();
	scene()->update();
}

bool GraphicsMap::areCoordinatesVisible() const
{
	return grid->areCoordinatesVisible();
}

void GraphicsMap::addMechToHex(MechEntity *mech, Hex *hex, Player *player)
{
	if (hex->getMech() != nullptr) {
		delete mech;
		return;
	}
	mech->setPosition({hex->getNumber(), BTech::DirectionN});

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

void GraphicsMap::scrollView(Qt::Edge direction)
{
	int positive;
	Qt::Orientation orientation;
	// this should be much shorter, but lo! Here we have a qt feature and we want to find a way round!
	switch (direction) {
		case Qt::RightEdge:  positive = -1; orientation = Qt::Horizontal; break;
		case Qt::LeftEdge:   positive =  1; orientation = Qt::Horizontal; break;
		case Qt::BottomEdge: positive = -1; orientation = Qt::Vertical; break;
		case Qt::TopEdge:    positive =  1; orientation = Qt::Vertical; break;
	}
	// funny thing
	QWheelEvent event(QPoint(0, 0), scrollSpeed * positive, Qt::NoButton, Qt::NoModifier, orientation);
	QGraphicsView::wheelEvent(&event);
	//qDebug() << QPoint(scrollSpeed * positive * (orientation == Qt::Horizontal), scrollSpeed * positive * (orientation == Qt::Vertical));
	//translate(scrollSpeed * positive * (orientation == Qt::Horizontal),
	//	  scrollSpeed * positive * (orientation == Qt::Vertical));
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
	grid->hideWalkRange();
	grid->hideShootRange();
	chooseAction(action);
}

void GraphicsMap::onEndMove()
{
	endMove();
}

void GraphicsMap::clearHexes()
{
	grid->clearHexes();
}

void GraphicsMap::initMap()
{
	initScene();
	initGrid();
	initHexes();
	initUnits();
	initScrolling();
	initScaling();
	initWindowSettings();
}

void GraphicsMap::initScene()
{
	QGraphicsScene *scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
	setScene(scene);
}

void GraphicsMap::initGrid()
{
	grid = new Grid(hexes, hexWidth, hexHeight);
}

void GraphicsMap::initHexes()
{
	for (Hex *hex : hexes) {
		GraphicsHex *graphicsHex = GraphicsFactory::get(hex);
		scene()->addItem(graphicsHex);
		for (GridGraphicsObject *object : graphicsHex->getGridGraphicsObjects()) {
			scene()->addItem(object);
			object->setPos(graphicsHex->pos());
		}

		connect(graphicsHex, &GraphicsHex::activated,      this, static_cast<void (GraphicsMap::*)(int)>(&GraphicsMap::hexClicked));
		connect(graphicsHex, &GraphicsHex::mouseEntered,   this, &GraphicsMap::hexTracked);
		connect(graphicsHex, &GraphicsHex::mouseLeft,      this, &GraphicsMap::hexAbandoned);
		connect(graphicsHex, &GraphicsHex::newAreaTracked, this, &GraphicsMap::hexNewAreaTracked);
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
	gEnt->init();
	scene()->addItem(gEnt);

	connect(mech, &MechEntity::stateInfoSent, this, &GraphicsMap::mechStateInfoReceived);
	connect(mech, &MechEntity::infoSent, this, &GraphicsMap::mechInfoReceived);
	connect(mech, &MechEntity::extensiveInfoSent, this, &GraphicsMap::mechExtensiveInfoReceived);

	emit mechAdded(mech);
}

void GraphicsMap::initScrolling()
{
	scrollSpeed = DEFAULT_SCROLL_SPEED;
}

void GraphicsMap::initScaling()
{
	scale = 1;
	scaleSpeed = DEFAULT_SCALE_SPEED;

	qreal mDim = (qreal)(qMax(hexWidth, hexHeight));
	minScale = MIN_SCALE_MULT / mDim;
	maxScale = 3;
}

void GraphicsMap::initWindowSettings()
{
	scene()->setSceneRect(-SCENE_BORDER, -SCENE_BORDER, scene()->width() + SCENE_BORDER, scene()->height() + SCENE_BORDER);
	setContentsMargins(0, 0, 0, 0);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMouseTracking(true);
	setEnabled(true);
}

void GraphicsMap::changeScale(qreal scale)
{
	finalScale = scale;

	//TODO CFiend wymienić na to? http://qt-project.org/doc/qt-5.1/qtcore/qpropertyanimation.html
	//TODO Fynx: Dunno. In your hands.
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
	initScrolling();
	initScaling();
}

void GraphicsMap::noPhase()
{
	grid->hideWalkRange();
	grid->hideShootRange();
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
	grid->showWalkRange(movement);
	scene()->update();
}

void GraphicsMap::emitMechShootRangeNeeded(const MechEntity *mech)
{
	grid->showShootRange(mech);
	scene()->update();
}

void GraphicsMap::emitMechRangesNotNeeded()
{
	grid->hideWalkRange();
	grid->hideShootRange();
	scene()->update();
}

void GraphicsMap::emitPlayerTurn(const Player *player)
{
	grid->drawFriendlyMechs(player);
	emit playerTurn(player);
}

void GraphicsMap::emitHexesNeedClearing()
{
	grid->hideAll();
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
	delete grid;
	mapLoaded = false;
	GraphicsFactory::clear();
	emit mapCleared();
}

void GraphicsMap::hexClicked(int hexNumber)
{
	emit hexClicked(hexes[hexNumber]);
	setCurrentHex(hexes[hexNumber]);
	(this->*GraphicsMap::phaseToFunction[getCurrentPhase()])();
}

void GraphicsMap::hexTracked(int hexNumber)
{
	emit hexDisplayStarted(hexes[hexNumber]);
}

void GraphicsMap::hexAbandoned(int hexNumber)
{
	emit hexDisplayQuit();
}

void GraphicsMap::hexNewAreaTracked(int hexNumber)
{
	emit hexDisplayChanged();
}

void GraphicsMap::mechInfoReceived()
{
	MechEntity *mech = static_cast<MechEntity *>(sender());
	emitMessageSent(mech->getType() + ": " + mech->getInfo(),
	                playerNameToColor[mech->getOwnerName()]);
}

void GraphicsMap::mechExtensiveInfoReceived()
{
	MechEntity *mech = static_cast<MechEntity *>(sender());
	if (mech->getInfo().isEmpty())
		emitExtensiveInfoSent(mech->getType() + " (" + mech->getOwnerName() + ")",
		                      playerNameToColor[mech->getOwnerName()]); /** signature */
	else
		emitExtensiveInfoSent(mech->getInfo());
}

void GraphicsMap::mechStateInfoReceived(const QString &message)
{
	emitMessageSent(getCurrentMech()->getOwnerName() + ": " + getCurrentMech()->getType() + ": " + message,
	                playerNameToColor[getCurrentMech()->getOwnerName()]);
}

void GraphicsMap::scaleView()
{
	qreal initScale = scale;
	if (scale > finalScale)
		scale = qMax(finalScale, scale - SCALE_ANIM_SPEED);
	else if (scale < finalScale)
		scale = qMin(finalScale, scale + SCALE_ANIM_SPEED);
	else
		return;

	QGraphicsView::scale(scale / initScale, scale / initScale);
	update();
}

void GraphicsMap::scaleAnimFinished()
{
	delete sender();
}
