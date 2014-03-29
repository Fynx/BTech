#ifndef MECH_PART_H
#define MECH_PART_H

#include "BTCommon/AttackObject.h"
#include "BTCommon/Effect.h"
#include "BTCommon/MechPartBase.h"
#include "BTCommon/Weapon.h"

class MechPart : public WeaponHolder, public EffectProne
{
public:
	static void initMechPart();

	MechPart();
	MechPart(const MechPartBase *mechPartBase);
	~MechPart();

	explicit operator QString() const;

	QString getUnitName() const;
	QString getOwnerName() const;

	friend bool operator == (const MechPart &lhs, const MechPart &rhs);

	BTech::MechPartType getType() const;
	BTech::MechPartSide getSide() const;
	int getMaxArmorValue() const;
	int getMaxInternalValue() const;

	void setBase(const MechPartBase *base);

	void setArmorValue(int value);
	int getArmorValue() const;

	void setInternalValue(int value);
	int getInternalValue() const;

	void addWeapon(Weapon *weapon);
	void removeWeapon(const Weapon *weapon);
	QList <Weapon *> getWeapons();

	void applyDamage(int damage);

	void triggerTurnRecovery();
	void attack(const AttackObject &attack);

	friend QDataStream & operator << (QDataStream &out, const MechPart &mechPart);
	friend QDataStream & operator >> (QDataStream &in, MechPart &mechPart);

private:
	const MechPartBase *base;

	int armorValue;
	int internalValue;
	QList <Weapon *> weapons;

	void clearData();
};

#endif // MECH_PART_H
