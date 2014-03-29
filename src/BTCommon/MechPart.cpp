#include "BTCommon/MechPart.h"

MechPart::MechPart()
{}

MechPart::MechPart(const MechPartBase *base)
{
	Q_ASSERT(base != nullptr);
	setBase(base);
}

MechPart::~MechPart()
{
	clearData();
}

MechPart::operator QString() const
{
	return static_cast<QString>(*base);
}

QString MechPart::getUnitName() const
{
	return QString();
}

QString MechPart::getOwnerName() const
{
	return QString();
}

bool operator == (const MechPart &lhs, const MechPart &rhs)
{
	return lhs.base == rhs.base;
}

BTech::MechPartType MechPart::getType() const
{
	return base->getType();
}

BTech::MechPartSide MechPart::getSide() const
{
	return base->getSide();
}

int MechPart::getMaxArmorValue() const
{
	return base->getMaxArmorValue();
}

int MechPart::getMaxInternalValue() const
{
	return base->getMaxInternalValue();
}

void MechPart::setBase(const MechPartBase *base)
{
	clearData();
	this->base = base;
	armorValue = base->getMaxArmorValue();
	internalValue = base->getMaxInternalValue();
	for (const WeaponBase *weapon : base->getWeapons())
		addWeapon(new Weapon(weapon));
}

void MechPart::setArmorValue(int value)
{
	this->armorValue = value;
}

int MechPart::getArmorValue() const
{
	return armorValue;
}

void MechPart::setInternalValue(int value)
{
	this->internalValue = value;
}

int MechPart::getInternalValue() const
{
	return internalValue;
}

void MechPart::addWeapon(Weapon *weapon)
{
	weapon->setWeaponHolder(this);
	weapons.append(weapon);
}

void MechPart::removeWeapon(const Weapon *weapon)
{
	for (Weapon *cur : weapons)
		if (cur == weapon)
			weapons.removeOne(cur);
}

QList <Weapon *> MechPart::getWeapons()
{
	return weapons;
}

void MechPart::applyDamage(int damage)
{
	int armorDamage = qMin(damage, armorValue);

	damage -= armorDamage;
	armorValue -= armorValue;

	internalValue = qMin(internalValue - damage, 0);

	if (internalValue == 0)
		addEffect(Effect(BTech::EffectType::Destroyed,
		                 BTech::EffectSource::Attack,
		                 Effect::FOREVER));
}

void MechPart::triggerTurnRecovery()
{
	for (Effect effect : editEffects())
		effect.triggerTurnRecovery();
}

QDataStream & operator << (QDataStream &out, const MechPart &mechPart)
{
	out << mechPart.armorValue << mechPart.internalValue;

	out << static_cast<qint32>(mechPart.weapons.size());
	for (Weapon *weapon : mechPart.weapons)
		out << *weapon;

	out << *(const EffectProne *)(&mechPart);

	return out;
}

QDataStream & operator >> (QDataStream &in, MechPart &mechPart)
{
	mechPart.clearData();

	in >> mechPart.armorValue >> mechPart.internalValue;

	qint32 weaponsSize;
	in >> weaponsSize;
	for (int i = 0; i < weaponsSize; ++i) {
		Weapon *weapon = new Weapon;
		in >> *weapon;
		mechPart.addWeapon(weapon);
	}

	in >> *(EffectProne *)(&mechPart);

	return in;
}

void MechPart::clearData()
{
	qDeleteAll(weapons);

	weapons.clear();
	editEffects().clear();
}

