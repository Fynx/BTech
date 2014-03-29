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
