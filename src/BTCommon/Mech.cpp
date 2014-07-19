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

#include "BTCommon/Mech.h"

/**
 * \class Mech
 */

Mech::Mech()
{
	setBase(nullptr);
}

Mech::Mech(UID uid)
	: Mech(MechModel::getMech(uid))
{}

Mech::Mech(const MechBase *base)
{
	setBase(base);
}

Mech::~Mech()
{
	clearData();
}

Mech::operator QString() const
{
	return getName();
}

void Mech::setBase(const MechBase *base)
{
	clearData();
	this->base = base;

	if (base == nullptr)
		return;

	for (const MechPartBase *mechPartBase : base->getMechParts())
		this->parts.append(new MechPart(mechPartBase));

	this->armorValue = base->getArmorValue();
	this->baseFireRange = base->getBaseFireRange();
	this->heatSinksNumber = base->getHeatSinksNumber();
}

QString Mech::getName() const
{
	return base->getName();
}

int Mech::getTonnage() const
{
	return base->getTonnage();
}

int Mech::getMaxMovePoints() const
{
	return base->getMaxMovePoints();
}

int Mech::getMaxRunPoints() const
{
	return getMaxMovePoints() * 3 / 2 + getMaxMovePoints() % 2;
}

int Mech::getMaxJumpPoints() const
{
	return base->getMaxJumpPoints();
}

void Mech::setArmorValue(int value)
{
	this->armorValue = value;
}

int Mech::getArmorValue() const
{
	return armorValue;
}

void Mech::setBaseFireRange(int range)
{
	this->baseFireRange = range;
}

int Mech::getBaseFireRange() const
{
	return baseFireRange;
}

void Mech::setHeatSinksNumber(int number)
{
	this->heatSinksNumber = number;
}

int Mech::getHeatSinksNumber() const
{
	return heatSinksNumber;
}

BTech::Range Mech::distanceToRange(int distance) const
{
	return base->distanceToRange(distance);
}

int Mech::getRangeDamage(BTech::Range range) const
{
	return base->getRangeDamage(range);
}

int Mech::getDamageValue(int distance) const
{
	return base->getDamageValue(distance);
}

QList <const MechPart *> Mech::getMechParts() const
{
	QList <const MechPart *> result;
	for (MechPart *mechPart : parts)
		result.append(mechPart);
	return result;
}

QList <const MechPart *> Mech::getMechParts(BTech::MechPartType type) const
{
	QList <const MechPart *> result;
	for (MechPart *mechPart : parts)
		if (mechPart->getType() == type)
			result.append(mechPart);
	return result;
}

QList <const Weapon *> Mech::getWeapons() const
{
	QList <const Weapon *> result;
	for (MechPart *mechPart : parts)
		for (Weapon *weapon : mechPart->getWeapons())
			result.append(weapon);
	return result;
}

QDataStream & operator << (QDataStream &out, const Mech &mech)
{
	qDebug() << "\t\tuid:             " << mech.base->getUid() << "\n"
	         << "\t\tarmorValue:      " << mech.armorValue << "\n"
	         << "\t\tbaseFireRange:   " << mech.baseFireRange << "\n"
	         << "\t\theatSinksNumber: " << mech.heatSinksNumber;
	out << mech.base->getUid()
	    << mech.armorValue << mech.baseFireRange << mech.heatSinksNumber;

	for (MechPart *mechPart : mech.parts)
		out << *mechPart;

	return out;
}

QDataStream & operator >> (QDataStream &in, Mech &mech)
{
	UID uid;
	in >> uid;
	mech.setBase(MechModel::getMech(uid));
	in >> mech.armorValue >> mech.baseFireRange >> mech.heatSinksNumber;

	qDebug() << "\t\tuid:             " << mech.base->getUid() << "\n"
	         << "\t\tarmorValue:      " << mech.armorValue << "\n"
	         << "\t\tbaseFireRange:   " << mech.baseFireRange << "\n"
	         << "\t\theatSinksNumber: " << mech.heatSinksNumber;

	for (MechPart *mechPart : mech.parts)
		in >> *mechPart;

	return in;
}

QList <MechPart *> Mech::findMechParts(BTech::MechPartType type)
{
	QList <MechPart *> result;

	for (MechPart * mechPart : parts)
		if (mechPart->getType() == type)
			result.append(mechPart);
	return result;
}

MechPart * Mech::findMechPart(BTech::MechPartType type, BTech::MechPartSide side)
{
	for (MechPart *mechPart : parts)
		if (mechPart->getType() == type && mechPart->getSide() == side)
			return mechPart;
	return nullptr;
}

void Mech::clearData()
{
	qDeleteAll(parts);
	parts.clear();
}
