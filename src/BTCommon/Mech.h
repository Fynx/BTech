#ifndef MECH_H
#define MECH_H

#include <QtWidgets>
#include "BTCommon/Action.h"
#include "BTCommon/CommonStrings.h"
#include "BTCommon/MechBase.h"
#include "BTCommon/MechPart.h"
#include "BTCommon/Weapon.h"

/**
 * \class Mech
 */
class Mech
{
public:
	Mech();
	Mech(UID uid);
	Mech(const MechBase *base);
	Mech(const Mech &) = delete;
	virtual ~Mech();

	explicit operator QString() const;

	void setBase(const MechBase *base);

	QString getType() const;

	int getTonnage() const;

	int getMaxMovePoints() const;
	int getMaxRunPoints() const;
	int getMaxJumpPoints() const;

	void setArmorValue(int value);
	int getArmorValue() const;

	void setBaseFireRange(int range);
	int getBaseFireRange() const;

	void setHeatSinksNumber(int number);
	int getHeatSinksNumber() const;

	BTech::Range distanceToRange(int distance) const;
	int getRangeDamage(BTech::Range range) const;
	int getDamageValue(int distance) const;

	void removeMechPart(const MechPartBase *mechPart);
	QList <const MechPart *> getMechParts() const;
	QList <const MechPart *> getMechParts(BTech::MechPartType type) const;
	QList <const Weapon *> getWeapons() const;

	friend QDataStream & operator << (QDataStream &out, const Mech &mech);
	friend QDataStream & operator >> (QDataStream &in, Mech &mech);

protected:
	QList <MechPart *> findMechParts(BTech::MechPartType type);
	MechPart * findMechPart(BTech::MechPartType type, BTech::MechPartSide side);

	const MechBase *base;

	int armorValue;
	int baseFireRange;
	int heatSinksNumber;

	QList <MechPart *> parts;

	void clearData();
};

#endif // MECH_H
