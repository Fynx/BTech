#ifndef MECH_PART_BASE_H
#define MECH_PART_BASE_H

#include <QtWidgets>
#include "BTCommon/Utils.h"
#include "BTCommon/WeaponBase.h"

class MechModel;
class EditMechPartsWidget;

class MechPartBase
{
	friend class MechModel;
	friend class EditMechPartsWidget;
public:

	MechPartBase(BTech::MechPartType type = BTech::MechPartType::Head, BTech::MechPartSide side = BTech::MechPartSide::General);
	MechPartBase(BTech::MechPartType type,
		     BTech::MechPartSide side,
		     int maxArmorValue,
		     int maxInternalValue,
		     const QList <const WeaponBase *> &weapons);
	MechPartBase(const MechPartBase &mechPart);

	MechPartBase & operator = (const MechPartBase &mechPart);

	explicit operator QString() const;

	bool operator == (const MechPartBase &mechPart) const;

	void setType(BTech::MechPartType type);
	BTech::MechPartType getType() const;
	void setSide(BTech::MechPartSide side);
	BTech::MechPartSide getSide() const;
	void setMaxArmorValue(int maxArmorValue);
	int getMaxArmorValue() const;
	void setMaxInternalValue(int value);
	int getMaxInternalValue() const;

	void addWeapon(const WeaponBase *weapon);
	const QList <const WeaponBase *> & getWeapons() const;
	void removeWeapon(int index);

	friend QDataStream & operator << (QDataStream &out, const MechPartBase &mechPart);
	friend QDataStream & operator >> (QDataStream &in, MechPartBase &mechPart);

	static const int MINIMAL_ARMOR_VALUE    = 1;
	static const int MINIMAL_INTERNAL_VALUE = 1;

private:
	void load(const MechPartBase &mechPartBase);
	void load(BTech::MechPartType type,
	          BTech::MechPartSide side,
	          int maxArmorValue,
	          int maxInternalValue,
	          const QList <const WeaponBase *> &weapons);

	BTech::MechPartType type;
	BTech::MechPartSide side;
	int maxArmorValue;
	int maxInternalValue;
	QList <const WeaponBase *> weapons;

	void clearData();
};

#endif // MECH_PART_BASE_H
