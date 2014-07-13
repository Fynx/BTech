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

#include "BTCommon/EnumHashFunctions.h"
#include "BTCommon/Utils.h"

/**
 * \enum BTech::GamePhase
 */

const BiHash <BTech::GamePhase, QString> BTech::phaseStringChange {
	{GamePhase::Initiative,     Strings::PhaseInitiative},
	{GamePhase::Movement,       Strings::PhaseMovement},
	{GamePhase::Reaction,       Strings::PhaseReaction},
	{GamePhase::WeaponAttack,   Strings::PhaseWeaponAttack},
	{GamePhase::PhysicalAttack, Strings::PhasePhysicalAttack},
	{GamePhase::Combat,         Strings::PhaseCombat},
	{GamePhase::Heat,           Strings::PhaseHeat},
	{GamePhase::End,            Strings::PhaseEnd},
};

/**
 * \enum BTech::GameVersion
 */

const BiHash <BTech::GameVersion, QString> BTech::gameVersionStringChange {
	{GameVersion::BasicBattleDroids,    Strings::BasicBattleDroids},
	{GameVersion::AdvancedBattleDroids, Strings::AdvancedBattleDroids},
};

const QHash <BTech::GameVersion, QList <BTech::GamePhase> > BTech::versionToPhaseList {
	{GameVersion::BasicBattleDroids,
		{GamePhase::Initiative, GamePhase::Movement, GamePhase::Combat, GamePhase::End}
	},
	{GameVersion::AdvancedBattleDroids,
		{GamePhase::Initiative, GamePhase::Movement, GamePhase::WeaponAttack, GamePhase::PhysicalAttack, GamePhase::Heat, GamePhase::End}
	}
};

QDataStream & BTech::operator << (QDataStream &out, const BTech::GameVersion &gameVersion)
{
	out << toUnderlying(gameVersion);
	return out;
}

QDataStream & BTech::operator >> (QDataStream &in, BTech::GameVersion &gameVersion)
{
	in >> toUnderlyingRef(gameVersion);
	return in;
}

/**
 * \enum BTech::EntityType
 */

const BiHash <BTech::EntityType, QString> BTech::entityStringChange {
	{EntityType::Mech, Strings::EntityMech},
};

/**
 * \enum BTech::MechPartType
 */

const BiHash <BTech::MechPartType, QString> BTech::mechPartTypeStringChange {
	{MechPartType::Head,  Strings::Head},
	{MechPartType::Torso, Strings::Torso},
	{MechPartType::Arm,   Strings::Arm},
	{MechPartType::Leg,   Strings::Leg},
};

QDataStream & BTech::operator << (QDataStream &out, const BTech::MechPartType &mechPartType)
{
	out << toUnderlying(mechPartType);
	return out;
}

QDataStream & BTech::operator >> (QDataStream &in, BTech::MechPartType &mechPartType)
{
	in >> toUnderlyingRef(mechPartType);
	return in;
}

/**
 * \enum BTech::MechPartSide
 */

const BiHash <BTech::MechPartSide, QString> BTech::mechPartSideStringChange {
	{MechPartSide::General, Strings::General},
	{MechPartSide::Center,  Strings::Center},
	{MechPartSide::Left,    Strings::Left},
	{MechPartSide::Right,   Strings::Right},
	{MechPartSide::Front,   Strings::Front},
	{MechPartSide::Rear,    Strings::Rear},
};

QDataStream & BTech::operator << (QDataStream &out, const BTech::MechPartSide &mechPartSide)
{
	out << toUnderlying(mechPartSide);
	return out;
}

QDataStream & BTech::operator >> (QDataStream &in, BTech::MechPartSide &mechPartSide)
{
	in >> toUnderlyingRef(mechPartSide);
	return in;
}

/**
 * \enum BTech::WeaponType
 */

const BiHash <BTech::WeaponType, QString> BTech::weaponTypeStringChange {
	{WeaponType::Energy,    Strings::WeaponTypeEnergy},
	{WeaponType::Ballistic, Strings::WeaponTypeBallistic},
	{WeaponType::Missile,   Strings::WeaponTypeMissile},
};

QDataStream & BTech::operator << (QDataStream &out, const BTech::WeaponType &weaponType)
{
	out << toUnderlying(weaponType);
	return out;
}

QDataStream & BTech::operator >> (QDataStream &in, BTech::WeaponType &weaponType)
{
	in >> toUnderlyingRef(weaponType);
	return in;
}

/**
 * \enum BTech::Range
 */

const BiHash <BTech::Range, QString> BTech::rangeStringChange {
	{Range::Contact, Strings::RangeContact},
	{Range::Short,   Strings::RangeShort},
	{Range::Medium,  Strings::RangeMedium},
	{Range::Long,    Strings::RangeLong},
};

const QHash <BTech::Range, int> BTech::MAX_RANGE {
	{Range::Contact,  1},
	{Range::Short,    3},
	{Range::Medium,  10},
	{Range::Long,    21},
};

QDataStream & BTech::operator << (QDataStream &out, const BTech::Range &range)
{
	out << toUnderlying(range);
	return out;
}

QDataStream & BTech::operator >> (QDataStream &in, BTech::Range &range)
{
	in >> toUnderlyingRef(range);
	return in;
}

/**
 * \enum BTech::EffectType
 */

const BiHash <BTech::EffectType, QString> BTech::effectTypeStringChange {
	{EffectType::None,           BTech::Strings::EffectNone},
	{EffectType::Destroyed,      BTech::Strings::EffectDestroyed},
	{EffectType::ShutDown,       BTech::Strings::EffectShutDown},
	{EffectType::Immobilised,    BTech::Strings::EffectImmobilised},
	{EffectType::Slowed,         BTech::Strings::EffectSlowed},
	{EffectType::CannotRun,      BTech::Strings::EffectCannotRun},
	{EffectType::CannotShoot,    BTech::Strings::EffectCannotShoot},
	{EffectType::CannotAttack,   BTech::Strings::EffectCannotAttack},
	{EffectType::AimingBothered, BTech::Strings::EffectAimingBothered},
	{EffectType::Walked,         BTech::Strings::EffectWalked},
	{EffectType::Run,            BTech::Strings::EffectRun},
	{EffectType::Jumped,         BTech::Strings::EffectJumped},
};

QDataStream & BTech::operator << (QDataStream &out, const BTech::EffectType &effect)
{
	out << toUnderlying(effect);
	return out;
}

QDataStream & BTech::operator >> (QDataStream &in, BTech::EffectType &effect)
{
	in >> toUnderlyingRef(effect);
	return in;
}

/**
 * \enum BTech::EffectSource
 */

QDataStream & BTech::operator << (QDataStream &out, const BTech::EffectSource &source)
{
	out << toUnderlying(source);
	return out;
}

QDataStream & BTech::operator >> (QDataStream &in, BTech::EffectSource &source)
{
	in >> toUnderlyingRef(source);
	return in;
}

/**
 * \enum BTech::MovementAction
 */

QDataStream & BTech::operator << (QDataStream &out, const BTech::MovementAction &action)
{
	out << toUnderlying(action);
	return out;
}

QDataStream & BTech::operator >> (QDataStream &in, BTech::MovementAction &action)
{
	in >> toUnderlyingRef(action);
	return in;
}

/**
 * \enum BTech::CombatAction
 */

QDataStream & BTech::operator << (QDataStream &out, const BTech::CombatAction &action)
{
	out << toUnderlying(action);
	return out;
}

QDataStream & BTech::operator >> (QDataStream &in, BTech::CombatAction &action)
{
	in >> toUnderlyingRef(action);
	return in;
}

/**
 * \typedef BTech::DiceRoll
 */

int BTech::dThrow()
{
	return randomInt(6) + 1;
}

int BTech::d2Throw()
{
	return dThrow() + dThrow();
}

bool BTech::checkRoll(int value)
{
	return d2Throw() >= value;
}

/**
 * \function BTech::random
 */

int BTech::randomInt()
{
	static bool initialized = false;
	if (!initialized) {
		initialized = true;
		qsrand(QTime(0, 0).secsTo(QTime::currentTime()));
	}
	return qrand();
}

int BTech::randomInt(int modulo)
{
	return randomInt() % modulo;
}

/**
 * \namespace General
 */

QString BTech::General::indexString(const QString &string, const QList <QString> &existing)
{
	QString unnamed = string;
	int c = static_cast<int>('0');
	bool exists;
	do {
		exists = false;
		++c;
		for (const QString &str : existing)
			exists |= str == unnamed + static_cast<char>(c);
	} while (exists);

	return unnamed + static_cast<char>(c);
}

static bool operator < (const QColor &lhs, const QColor &rhs)
{
	return lhs.name() < rhs.name();
}

QString BTech::General::bashColorString(const QString &string, const QColor& color)
{
	static const QMap <QColor, QString> colorBashValue {
		{Qt::white,  "0m"},
		{Qt::red,    "31m"},
		{Qt::green,  "32m"},
		{Qt::yellow, "33m"},
		{Qt::blue,   "34m"},
	};
	
	static const QString pref = QString("\e[");
	
	if (colorBashValue.contains(color))
		return pref + colorBashValue[color] + string + pref + colorBashValue[Qt::white];
	else
		return string;
}

static QElapsedTimer worldTimer;

void BTech::General::startTime()
{
	worldTimer.start();
}

long long unsigned int BTech::General::getTime()
{
	return worldTimer.elapsed();
}

/**
 * \namespace Math
 */

int BTech::Math::lineAngle(const QPoint &src, const QPoint &dest)
{
	return static_cast<int>(qRadiansToDegrees(qAtan2(dest.x() - src.x(), dest.y() - src.y())) + FULL_ANGLE) % FULL_ANGLE;
}

int BTech::Math::lineAngle(const QPoint &src, const QPointF &dest)
{
	return lineAngle(src, dest.toPoint());
}

int BTech::Math::lineAngle(const QPointF &src, const QPoint &dest)
{
	return lineAngle(src.toPoint(), dest);
}

/**
 * \class Matrix
 */
BTech::Matrix::Matrix(int x1, int y1, int x2, int y2)
	: x1(x1), y1(y1), x2(x2), y2(y2)
{}

BTech::Matrix & BTech::Matrix::operator = (Matrix arg)
{
	x1 = arg.x1;
	y1 = arg.y1;
	x2 = arg.x2;
	y2 = arg.y2;

	return *this;
}

int BTech::Matrix::det() const
{
	return x2 * y1 - x1 * y2;
}
