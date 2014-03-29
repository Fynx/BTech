#ifndef GRAPHICS_FACTORY_H
#define GRAPHICS_FACTORY_H

#include <QtWidgets>
#include "BTCommon/CommonStrings.h"
#include "BTCommon/GraphicsEntity.h"
#include "BTCommon/GraphicsHex.h"
#include "BTCommon/MechEntity.h"

/**
 * \class GraphicsFactory
 * This class provides static method get(QString) which returns new GraphicsEntity.
 * An object of this class cannot be created, because it does not provide any object-dependent functionalities.
 * Also, function init() must be called when the program starts, so functions behave properly.
 */
class GraphicsFactory
{

public:
	static GraphicsEntity * get(Entity *entity);
	static GraphicsEntity * get(const Entity *entity);
	static GraphicsHex * get(Hex *hex);
	static GraphicsHex * get(const Hex *hex);
	static void erase(const GraphicsHex *hex);
	static void erase(const GraphicsEntity *entity);
	static void clear();

	GraphicsFactory() = delete;
	GraphicsFactory(const GraphicsFactory &) = delete;
	GraphicsFactory(GraphicsFactory &&) = delete;
	void operator = (const GraphicsFactory &) = delete;
	void operator = (GraphicsFactory &&) = delete;

private:
	static QHash <const Entity *, GraphicsEntity *> entityToGraphics;
	static QHash <const Hex *, GraphicsHex *> hexToGraphics;
};

#endif // GRAPHICS_FACTORY_H
