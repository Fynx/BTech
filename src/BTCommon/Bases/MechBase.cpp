/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
Copyright (C) 2014 by Bartosz Szreder <szreder [at] mimuw [dot] edu [dot] pl>
Copyright (C) 2014 by Krzysztof Adamek <squadack [at] students [dot] mimuw [dot] edu [dot] pl>
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

#include "BTCommon/Bases/MechBase.h"
#include "BTCommon/Bases/MechPartBase.h"
#include "BTCommon/Utils.h"

MechBase::MechBase()
{
	parts.append(new MechPartBase(BTech::MechPartType::Head,  BTech::MechPartSide::General));
	parts.append(new MechPartBase(BTech::MechPartType::Torso, BTech::MechPartSide::Left));
	parts.append(new MechPartBase(BTech::MechPartType::Torso, BTech::MechPartSide::Right));
	parts.append(new MechPartBase(BTech::MechPartType::Torso, BTech::MechPartSide::Center));
	parts.append(new MechPartBase(BTech::MechPartType::Arm,   BTech::MechPartSide::Left));
	parts.append(new MechPartBase(BTech::MechPartType::Arm,   BTech::MechPartSide::Right));
	parts.append(new MechPartBase(BTech::MechPartType::Leg,   BTech::MechPartSide::Left));
	parts.append(new MechPartBase(BTech::MechPartType::Leg,   BTech::MechPartSide::Right));

	setTonnage(DEFAULT_TONNAGE);
	setArmorValue(DEFAULT_ARMOR_VALUE);
	setMaxMovePoints(DEFAULT_MAX_MOVE_POINTS);
	setMaxJumpPoints(DEFAULT_MAX_JUMP_POINTS);
	setBaseFireRange(DEFAULT_BASE_FIRE_RANGE);
	setHeatSinksNumber(DEFAULT_HEAT_SINKS_NUMBER);

	for (BTech::Range range : BTech::attackRanges)
		setRangeDamage(range, DEFAULT_RANGE_DAMAGE);
	setRangeDamage(BTech::Range::OutOfRange, DEFAULT_OUT_OF_RANGE_DAMAGE);
}

MechBase::MechBase(UID uid)
	: MechBase()
{
	this->uid = uid;
}

MechBase::MechBase(const QString &name, UID uid)
	: MechBase(uid)
{
	this->name = name;
}

MechBase::MechBase(const QString &name,
                   int tonnage,
                   int armorValue,
                   int maxMPS,
                   int maxJumpPoints,
                   int baseFireRange,
                   int heatSinksNumber,
                   const QList <MechPartBase *> &parts,
                   int contactRangeDamage,
                   int shortRangeDamage,
                   int mediumRangeDamage,
                   int longRangeDamage)
{
	load(name,
	     tonnage,
	     armorValue,
	     maxMPS,
	     maxJumpPoints,
	     baseFireRange,
	     heatSinksNumber,
	     parts,
	     contactRangeDamage,
	     shortRangeDamage,
	     mediumRangeDamage,
	     longRangeDamage);
}

MechBase::MechBase(const MechBase &mech)
{
	load(mech.name,
	     mech.tonnage,
	     mech.armorValue,
	     mech.maxMovePoints,
	     mech.maxJumpPoints,
	     mech.baseFireRange,
	     mech.heatSinksNumber,
	     mech.parts,
	     mech.rangeDamage[BTech::Range::Contact],
	     mech.rangeDamage[BTech::Range::Short],
	     mech.rangeDamage[BTech::Range::Medium],
	     mech.rangeDamage[BTech::Range::Long]);
}

MechBase::~MechBase()
{
	qDeleteAll(parts);
}

void MechBase::load(const MechBase &mech)
{
	load(mech.name,
	     mech.tonnage,
	     mech.armorValue,
	     mech.maxMovePoints,
	     mech.maxJumpPoints,
	     mech.baseFireRange,
	     mech.heatSinksNumber,
	     mech.parts,
	     mech.rangeDamage[BTech::Range::Contact],
	     mech.rangeDamage[BTech::Range::Short],
	     mech.rangeDamage[BTech::Range::Medium],
	     mech.rangeDamage[BTech::Range::Long]);
}

void MechBase::load(const QString &name,
                    int tonnage,
                    int armorValue,
                    int maxMPS,
                    int maxJumpPoints,
                    int baseFireRange,
                    int heatSinksNumber,
                    const QList <MechPartBase *> &parts,
                    int contactRangeDamage,
                    int shortRangeDamage,
                    int mediumRangeDamage,
                    int longRangeDamage)
{
	this->name = name;

	this->tonnage         = tonnage;
	this->armorValue      = armorValue;
	this->maxMovePoints   = maxMPS;
	this->maxJumpPoints   = maxJumpPoints;
	this->baseFireRange   = baseFireRange;
	this->heatSinksNumber = heatSinksNumber;

	for (const MechPartBase *part : parts) {
		for (MechPartBase *current : this->parts)
			if (current->getType() == part->getType() && current->getSide() == part->getSide())
				*current = *part;
	}

	this->rangeDamage[BTech::Range::Contact] = contactRangeDamage;
	this->rangeDamage[BTech::Range::Short]   = shortRangeDamage;
	this->rangeDamage[BTech::Range::Medium]  = mediumRangeDamage;
	this->rangeDamage[BTech::Range::Long]    = longRangeDamage;
}

MechBase::operator QString() const
{
	return getName();
}

bool MechBase::operator == (const MechBase &mech) const
{
	return getUid() == mech.getUid();
}

void MechBase::setName(const QString &name)
{
	this->name = name;
}

QString MechBase::getName() const
{
	return name;
}

void MechBase::setTonnage(int tonnage)
{
	this->tonnage = tonnage;
}

int MechBase::getTonnage() const
{
	return tonnage;
}

void MechBase::setArmorValue(int value)
{
	this->armorValue = value;
}

int MechBase::getArmorValue() const
{
	return armorValue;
}

void MechBase::setMaxMovePoints(int maxMovePoints)
{
	this->maxMovePoints = maxMovePoints;
}

int MechBase::getMaxMovePoints() const
{
	return maxMovePoints;
}

void MechBase::setMaxJumpPoints(int maxJumpPoints)
{
	this->maxJumpPoints = maxJumpPoints;
}

int MechBase::getMaxJumpPoints() const
{
	return maxJumpPoints;
}

void MechBase::setBaseFireRange(int range)
{
	this->baseFireRange = range;
}

int MechBase::getBaseFireRange() const
{
	return baseFireRange;
}

void MechBase::setHeatSinksNumber(int number)
{
	this->heatSinksNumber = number;
}

BTech::Range MechBase::distanceToRange(int distance) const
{
	for (BTech::Range range : BTech::attackRanges)
		if (distance <= BTech::MAX_RANGE[range])
			return range;
	return BTech::Range::OutOfRange;
}

int MechBase::getHeatSinksNumber() const
{
	return heatSinksNumber;
}

void MechBase::setRangeDamage(BTech::Range range, int damage)
{
	rangeDamage[range] = damage;
}

int MechBase::getRangeDamage(BTech::Range range) const
{
	return rangeDamage[range];
}

int MechBase::getDamageValue(int distance) const
{
	return rangeDamage[distanceToRange(distance)];
}

MechPartBase * MechBase::getMechPart(BTech::MechPartType type, BTech::MechPartSide side)
{
	for (MechPartBase *mechPart : parts)
		if (mechPart->getType() == type && mechPart->getSide() == side)
			return mechPart;
	qWarning() << "Warning: MechPart with type" << BTech::mechPartTypeStringChange[type] << "and side" << BTech::mechPartSideStringChange[side] << "not found. Curious...";
	return nullptr;
}

MechPartBase * MechBase::getMechPart(QPair <BTech::MechPartType, BTech::MechPartSide> partAndSide)
{
	return getMechPart(partAndSide.first, partAndSide.second);
}

const QList <MechPartBase *> & MechBase::getMechParts() const
{
	return parts;
}

QDataStream & operator << (QDataStream &out, const MechBase &mech)
{
	out << mech.name << mech.uid
	    << mech.tonnage << mech.armorValue << mech.maxMovePoints << mech.maxJumpPoints << mech.baseFireRange << mech.heatSinksNumber;

	for (BTech::Range range : BTech::attackRanges)
		out << mech.rangeDamage[range];

	out << static_cast<quint32>(mech.parts.size());
	for (MechPartBase *mechPart : mech.parts)
		out << *mechPart;

	return out;
}

QDataStream & operator >> (QDataStream &in, MechBase &mech)
{
	in >> mech.name >> mech.uid
	   >> mech.tonnage >> mech.armorValue >> mech.maxMovePoints >> mech.maxJumpPoints >> mech.baseFireRange >> mech.heatSinksNumber;

	for (BTech::Range range : BTech::attackRanges)
		in >> mech.rangeDamage[range];

	quint32 mechPartSize;
	in >> mechPartSize;
	for (quint32 i = 0; i < mechPartSize; ++i)
		in >> *(mech.parts[i]);

	return in;
}

void MechBase::setUid(UID uid)
{
	this->uid = uid;
}

UID MechBase::getUid() const
{
	return uid;
}
