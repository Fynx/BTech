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

#include "BTCommon/GraphicsFactory.h"

QHash <const Entity *, GraphicsEntity *> GraphicsFactory::entityToGraphics;
QHash <const Hex *, GraphicsHex *> GraphicsFactory::hexToGraphics;

GraphicsEntity * GraphicsFactory::get(Entity *entity)
{
	if (entity == nullptr)
		qWarning() << "Warning: trying to get graphics entity from null pointer.";
	const Entity *constEntity = entity;

	if (!entityToGraphics.contains(constEntity)) {
		GraphicsEntity *newGraphicsEntity;
		switch (entity->getEntityType()) {
			case BTech::EntityType::Mech:
				newGraphicsEntity = new GraphicsMech((MechEntity *)entity);
				break;
		}
		entityToGraphics[constEntity] = newGraphicsEntity;
	}
	return entityToGraphics[constEntity];
}

GraphicsEntity * GraphicsFactory::get(const Entity *entity)
{
	if (entity == nullptr)
		qWarning() << "Warning: trying to get graphics entity from null pointer.";
	if (!entityToGraphics.contains(entity)) {
		qWarning() << "Warning: Entity not found in the GraphicsFactory";
		return nullptr;
	}
	return entityToGraphics[entity];
}

GraphicsHex * GraphicsFactory::get(Hex *hex)
{
	if (hex == nullptr)
		qWarning() << "Warning: trying to get graphics hex from null pointer.";
	if (!hexToGraphics.contains(hex))
		hexToGraphics[hex] = new GraphicsHex(hex);
	return hexToGraphics[hex];
}

GraphicsHex * GraphicsFactory::get(const Hex *hex)
{
	if (hex == nullptr)
		qWarning() << "Warning: trying to get graphics hex from null pointer.";
	if (!hexToGraphics.contains(hex)) {
		qWarning() << "Warning: Hex not found in the GraphicsFactory";
		return nullptr;
	}
	return hexToGraphics[hex];
}

void GraphicsFactory::erase(const GraphicsHex *hex)
{
	hexToGraphics.remove(hexToGraphics.key(const_cast<GraphicsHex *>(hex)));
}

void GraphicsFactory::erase(const GraphicsEntity *entity)
{
	entityToGraphics.remove(entityToGraphics.key(const_cast<GraphicsEntity *>(entity)));
}

void GraphicsFactory::clear()
{
	qDebug() << "Clear GraphicsFactory";

	entityToGraphics.clear();
	hexToGraphics.clear();
}
