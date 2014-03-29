#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

#include <QtCore/qglobal.h>
#include "BTCommon/EnumSerialization.h"

#define EnumHashFunctionBTech(EnumName, TypeName) \
	namespace BTech { \
		enum class EnumName : TypeName; \
	} \
	\
	uint qHash(TypeName, uint) noexcept (true); \
	\
	static inline uint qHash(BTech::EnumName enumVar) \
	{ \
		return qHash(toUnderlying(enumVar), 0); \
	} \

EnumHashFunctionBTech(CombatAction,     quint8);
EnumHashFunctionBTech(EffectType,       quint16);
EnumHashFunctionBTech(EntityType,       quint8);
EnumHashFunctionBTech(GamePhase,        qint8);
EnumHashFunctionBTech(GameVersion,      quint8);
EnumHashFunctionBTech(MechPartSide,     quint8);
EnumHashFunctionBTech(MechPartType,     quint8);
EnumHashFunctionBTech(Modifier,         quint8);
EnumHashFunctionBTech(ModifierType,     quint8);
EnumHashFunctionBTech(MovementAction,   quint8);
EnumHashFunctionBTech(Range,            quint8);
EnumHashFunctionBTech(Skill,            quint8);
EnumHashFunctionBTech(Terrain,          quint8);
EnumHashFunctionBTech(WeaponType,       quint8);

#include <QtCore/QHash>

#endif
