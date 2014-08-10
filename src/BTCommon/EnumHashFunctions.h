/*
The MIT License (MIT)

Copyright (c) 2013 Bartosz Szreder

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

EnumHashFunctionBTech(ActionType,       quint8);
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
