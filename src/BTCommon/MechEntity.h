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

#ifndef MECH_ENTITY_H
#define MECH_ENTITY_H

#include <QtWidgets>
#include "BTCommon/AttackObject.h"
#include "BTCommon/Mech.h"
#include "BTCommon/Objects.h"
#include "BTCommon/Weapon.h"

class Action;
class CombatAction;
class MechWarrior;

/**
 * \class MechEntity
 * This class represents units (only mechs right now).
 * Units store information about their state, allowed actions, etc.
 */
class MechEntity : public Movable, public Mech, public WeaponHolder, public Attackable
{
Q_OBJECT;

public:
	MechEntity();
	MechEntity(UID uid);
	MechEntity(const Mech &mech);
	MechEntity(const MechEntity &) = delete;
	~MechEntity();

	explicit operator QString() const;

	virtual BTech::EntityType getEntityType() const;

	QString getUnitName() const;

	void setOwnerName(const QString &ownerName);
	QString getOwnerName() const;

	void setMechPosition(MechPosition *mechPosition);

	Direction getTorsoDirection() const;
	void turnTorsoRight();
	void turnTorsoLeft();

	void move(const MoveObject &moveObject);
	void turnRight();
	void turnLeft();

	int getDistanceCrossed() const;

	void attack(MechEntity *target);

	void resolveAttacks();
	void resolveHeat();

	const Weapon * getCurrentWeapon() const;
	QList <const Weapon *> getReadyWeapons() const;

	QList <const Action *> getActions(BTech::GamePhase phase) const;
	const Action * getCurrentAction() const;
	void setCurrentAction(const Action *action);

	const MovementAction * getCurrentMovementAction() const;
	void setCurrentMovementAction(const MovementAction *action);

	const CombatAction * getCurrentCombatAction() const;
	void setCurrentCombatAction(const CombatAction *action);

	int getMovePoints() const;
	int getRunPoints() const;
	int getJumpPoints() const;
	int getMovePoints(BTech::MovementAction action);

	int getFireRange() const;
	int getHeatLevel() const;

	int getBaseAttackModifier() const;
	int getAttackAttackerMovementModifier() const;
	int getAttackTargetMovementModifier() const;

	void setMoved(bool moved);
	bool isMoved() const;
	void setActive(bool active);
	bool isActive() const;
	void setFriendly(bool friendly);
	bool isFriendly() const;

	QString getInfo() const;
	QString getExtensiveInfo() const;

	void init();
	void clear();
	void recover();

	friend QDataStream & operator << (QDataStream &out, const MechEntity &mech);
	friend QDataStream & operator >> (QDataStream &in, MechEntity &mech);

signals:
	void activated();
	void infoSent();
	void extensiveInfoSent();
	void stateInfoSent(const QString &message);

private:
	Weapon * findWeapon(const Weapon *weapon);

	bool attackModifierCheck(const AttackObject &attack);
	bool armorPenetrationCheck(const AttackObject &attack);

	struct HitLocation {
		BTech::MechPartType type;
		BTech::MechPartSide side;
		bool critical;
	};

	HitLocation getHitLocation(const AttackObject &attack);

	void sendInfo(const QString &info);
	void sendExtensiveInfo(const QString &info = QString());

	QString ownerName;
	MechPosition *mechPosition;
	const Attackable *attackManager;
	MechWarrior *mechWarrior;

	Direction torsoDirection;

	int movePointsUsed;
	int runPointsUsed;
	int jumpPointsUsed;

	int heatLevel;

	bool moved;
	bool attacked;
	bool active;
	bool friendly;

	QString info;
	QString extensiveInfo;

	const MovementAction *currentMovementAction;
	const CombatAction *currentCombatAction;

	static const QHash <BTech::GameVersion, void (MechEntity::*)()> resolveAttacks_version;

	void resolveAttacks_BBD();
	void resolveAttacks_ABD();

	QList <const Action *> getActions_BBD(BTech::GamePhase phase) const;
	QList <const Action *> getActions_ABD(BTech::GamePhase phase) const;

	int getDistanceAttackModifier(int distance) const;

	void clearData();
};

namespace BTech {
	static const int MAX_POSSIBLE_DAMAGE = 16;
	static const int MAX_POSSIBLE_ARMOR_VALUE = 13;

	extern const QHash <Direction, MechPartSide> directionToMechPartSide;
	extern const QHash <QPair <BTech::DiceRoll, MechPartSide>, QPair <MechPartType, MechPartSide> > hitLocationTable;
	extern const QHash <BTech::MovementAction, int> attackerMovementModifierTable;
	extern const QHash <int, int> targetMovementModifierTable;
	extern const int armorPenetrationTable[MAX_POSSIBLE_DAMAGE + 1][MAX_POSSIBLE_ARMOR_VALUE + 1];
}

#endif // MECH_ENTITY_H
