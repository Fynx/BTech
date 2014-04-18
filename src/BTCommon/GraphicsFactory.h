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
