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
	return getType();
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

QString Mech::getType() const
{
	return base->getType();
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
