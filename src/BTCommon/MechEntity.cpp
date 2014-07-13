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

#include "BTCommon/MechEntity.h"

/**
 * \class MechEntity
 */

MechEntity::MechEntity()
	: mechWarrior(nullptr)
{
	init();
}

MechEntity::MechEntity(UID uid)
	: Mech(uid), ownerName(BTech::Strings::PlayerNone), mechPosition(nullptr), mechWarrior(new MechWarrior)
{
	init();
}

MechEntity::~MechEntity()
{
	clearData();
	if (mechPosition != nullptr)
		mechPosition->removeMech();
}

MechEntity::operator QString() const
{
	return Mech::operator QString();
}

BTech::EntityType MechEntity::getEntityType() const
{
	return BTech::EntityType::Mech;
}

QString MechEntity::getUnitName() const
{
	return QString(*this);
}

void MechEntity::setOwnerName(const QString &ownerName)
{
	this->ownerName = ownerName;
}

QString MechEntity::getOwnerName() const
{
	return ownerName;
}

void MechEntity::setMechPosition(MechPosition *mechPosition)
{
	this->mechPosition = mechPosition;
}

Direction MechEntity::getTorsoDirection() const
{
	return torsoDirection;
}

void MechEntity::turnTorsoRight()
{
	++torsoDirection;
}

void MechEntity::turnTorsoLeft()
{
	--torsoDirection;
}

void MechEntity::move(const MoveObject &moveObject)
{
	switch (moveObject.getAction()) {
		case BTech::MovementAction::Idle:
			qCritical() << "Error: move object with action 'Idle'";
			exit(EXIT_FAILURE);
		case BTech::MovementAction::Walk:
			movePointsUsed += moveObject.getMovePoints();
			runPointsUsed  = getMaxRunPoints();
			jumpPointsUsed = getMaxJumpPoints();
			break;
		case BTech::MovementAction::Run:
			runPointsUsed += moveObject.getMovePoints();
			movePointsUsed = getMaxMovePoints();
			jumpPointsUsed = getMaxJumpPoints();
			break;
		case BTech::MovementAction::Jump:
			jumpPointsUsed += moveObject.getMovePoints();
			movePointsUsed = getMaxMovePoints();
			runPointsUsed  = getMaxRunPoints();
			break;
		default:;
	}

	setMoveObject(moveObject);
	setDestinationPosition(moveObject.getDest());

	switch (moveObject.getAction()) {
		case BTech::MovementAction::Walk:
			addEffect(Effect(BTech::EffectType::Walked, BTech::EffectSource::Movement, 1, moveObject.getDistance()));
			break;
		case BTech::MovementAction::Run:
			addEffect(Effect(BTech::EffectType::Run, BTech::EffectSource::Movement, 1, moveObject.getDistance()));
			break;
		case BTech::MovementAction::Jump:
			addEffect(Effect(BTech::EffectType::Jumped, BTech::EffectSource::Movement, 1, moveObject.getDistance()));
			break;
		default:;
	}

	sendExtensiveInfo();
	sendExtensiveInfo(QString(" ") + BTech::ExtInfo::MovedToHex.arg(moveObject.getDest().getNumber()));
}

void MechEntity::turnRight()
{
	turnTorsoRight();
}

void MechEntity::turnLeft()
{
	turnTorsoLeft();
}

int MechEntity::getDistanceCrossed() const
{
	return getEffect(BTech::EffectType::Walked).getValue() * (int)hasEffect(BTech::EffectType::Walked)
	     + getEffect(BTech::EffectType::Run).getValue()    * (int)hasEffect(BTech::EffectType::Run)
	     + getEffect(BTech::EffectType::Jumped).getValue() * (int)hasEffect(BTech::EffectType::Jumped);
}

void MechEntity::attack(MechEntity *enemy)
{
	sendExtensiveInfo(BTech::ExtInfo::AttackedObject.arg(QString(*enemy)));

	AttackObject attackObject = enemy->getAttackObject();

	if (attackObject.getWeaponHolder()->getCurrentWeapon() != nullptr)
		findWeapon(attackObject.getWeapon())->setUsed(true);
	else
		attacked = true;
	setMoved(true);

	enemy->receiveAttack();
}

const QHash <BTech::GameVersion, void (MechEntity::*)()> MechEntity::resolveAttacks_version {
	{BTech::GameVersion::BasicBattleDroids,    &MechEntity::resolveAttacks_BBD},
	{BTech::GameVersion::AdvancedBattleDroids, &MechEntity::resolveAttacks_ABD},
};

void MechEntity::resolveAttacks()
{
	(this->*MechEntity::resolveAttacks_version[Rules::getVersion()])();
}

void MechEntity::resolveHeat()
{
	heatLevel = qMax(heatLevel - heatSinksNumber, 0);

	removeEffects(BTech::EffectSource::Heat);

	setEffect(Effect(BTech::EffectType::Slowed,
	                 BTech::EffectSource::Heat,
	                 Effect::FOREVER,
	                 heatLevel / 5));
	setEffect(Effect(BTech::EffectType::AimingBothered,
	                 BTech::EffectSource::Heat,
	                 Effect::FOREVER,
	                 (int)(heatLevel >= 8) + (int)(heatLevel >= 13) + (int)(heatLevel >= 17) + (int)(heatLevel >= 24)));

	if (!BTech::checkRoll((heatLevel - 10) / 2 + 2 + (int)(heatLevel == 30)))
		setEffect(Effect(BTech::EffectType::ShutDown,
		                 BTech::EffectSource::Heat,
		                 Effect::FOREVER));
}

const Weapon * MechEntity::getCurrentWeapon() const
{
	if (getCurrentCombatAction()->hasWeapon() == false)
		return nullptr;
	return getCurrentCombatAction()->getWeapon();
}

QList <const Weapon *> MechEntity::getReadyWeapons() const
{
	//TODO Effects
	return Mech::getWeapons();
}

QList <const Action *> MechEntity::getActions(BTech::GamePhase gamePhase) const
{
	switch (Rules::getVersion()) {
		case BTech::GameVersion::BasicBattleDroids:
			return getActions_BBD(gamePhase);
		case BTech::GameVersion::AdvancedBattleDroids:
			return getActions_ABD(gamePhase);
		default:
			return QList <const Action *>();
	}
}

const Action * MechEntity::getCurrentAction() const
{
	return (getCurrentMovementAction() == nullptr)
		? static_cast<const Action *>(getCurrentCombatAction())
		: static_cast<const Action *>(getCurrentMovementAction());
}

void MechEntity::setCurrentAction(const Action *action)
{
	const MovementAction *movementAction = nullptr;
	const CombatAction *combatAction = nullptr;
	if (action != nullptr) {
		switch (action->getActionType()) {
			case Action::Type::Movement:
				movementAction = static_cast<const MovementAction *>(action);
				break;
			case Action::Type::Combat:
				combatAction = static_cast<const CombatAction *>(action);
				break;
		}
	}
	setCurrentMovementAction(movementAction);
	setCurrentCombatAction(combatAction);
}

const MovementAction * MechEntity::getCurrentMovementAction() const
{
	return currentMovementAction;
}

void MechEntity::setCurrentMovementAction(const MovementAction *action)
{
	currentMovementAction = action;
}

const CombatAction * MechEntity::getCurrentCombatAction() const
{
	return currentCombatAction;
}

void MechEntity::setCurrentCombatAction(const CombatAction *action)
{
	currentCombatAction = action;
}

int MechEntity::getMovePoints() const
{
	int slowEffect = (int)(hasEffect(BTech::EffectType::Slowed)) + 1;  // 1 if no slow effect, 2 otherwise
	return ((getMaxMovePoints() - movePointsUsed * slowEffect) / slowEffect);
}

int MechEntity::getRunPoints() const
{
	int slowEffect = (int)(hasEffect(BTech::EffectType::Slowed)) + 1;  // 1 if no slow effect, 2 otherwise
	return ((getMaxRunPoints() - runPointsUsed * slowEffect) / slowEffect);
}

int MechEntity::getJumpPoints() const
{
	return getMaxJumpPoints() - jumpPointsUsed;
}

int MechEntity::getMovePoints(BTech::MovementAction action)
{
	switch (action) {
		case BTech::MovementAction::Walk: return getMovePoints();
		case BTech::MovementAction::Run:  return getRunPoints();
		case BTech::MovementAction::Jump: return getJumpPoints();
		default:                          return 0;
	}
}

int MechEntity::getFireRange() const
{
	int fireRange = hasEffect(BTech::EffectType::CannotShoot)
		? 1
		: baseFireRange;
	return fireRange * (int)(hasEffect(BTech::EffectType::CannotAttack) && !attacked);
}

int MechEntity::getHeatLevel() const
{
	return heatLevel;
}

int MechEntity::getBaseAttackModifier() const
{
	return mechWarrior->getSkill(BTech::Skill::Gunnery);
}

int MechEntity::getAttackAttackerMovementModifier() const
{
	if (hasEffect(BTech::EffectType::Jumped))
		return 3;
	if (hasEffect(BTech::EffectType::Run))
		return 2;
	if (hasEffect(BTech::EffectType::Walked))
		return 1;
	return 0;
}

int MechEntity::getAttackTargetMovementModifier() const
{
	return (int)hasEffect(BTech::EffectType::Jumped)
	     + getDistanceCrossed();
}

void MechEntity::setMoved(bool moved)
{
	this->moved = moved;
	if (isMoved()) {
		movePointsUsed = getMaxMovePoints();
		jumpPointsUsed = getMaxJumpPoints();
	} else {
		movePointsUsed = 0;
		runPointsUsed = 0;
		jumpPointsUsed = 0;	//TODO check it.
	}
}

bool MechEntity::isMoved() const
{
	return moved;
}

void MechEntity::setActive(bool active)
{
	if (!this->active && active)
		emit activated();
	this->active = active;
}

bool MechEntity::isActive() const
{
	return active;
}

void MechEntity::setFriendly(bool friendly)
{
	this->friendly = friendly;
}

bool MechEntity::isFriendly() const
{
	return friendly;
}

QString MechEntity::getInfo() const
{
	return info;
}

QString MechEntity::getExtensiveInfo() const
{
	return extensiveInfo;
}

void MechEntity::init()
{
	mechPosition = nullptr;
	torsoDirection = BTech::DirectionFront;
	attackManager = nullptr;
	setMoved(false);
	jumpPointsUsed = 0;
	clear();
}

void MechEntity::clear()
{
	setCurrentMovementAction(new MovementAction(BTech::MovementAction::Idle));
	setCurrentCombatAction(new CombatAction(BTech::CombatAction::Idle));
	setActive(false);
	setFriendly(false);
	attacked = false;
}

void MechEntity::recover()
{
	for (Effect &effect : editEffects())
		effect.triggerTurnRecovery();

	for (auto mechPart : parts)
		for (Weapon *weapon : mechPart->getWeapons())
			weapon->setUsed(false);
}

QDataStream & operator << (QDataStream &out, const MechEntity &mech)
{
	out << static_cast<const Mech &>(mech) << static_cast<const Movable &>(mech)
	    << mech.ownerName
	    << mech.heatLevel
	    << *(mech.mechWarrior);

	return out;
}

QDataStream & operator >> (QDataStream &in, MechEntity &mech)
{
	mech.clearData();

	in >> static_cast<Mech &>(mech) >> static_cast<Movable &>(mech);

	in >> mech.ownerName
	   >> mech.heatLevel;

	mech.mechWarrior = new MechWarrior;
	in >> *(mech.mechWarrior);

	return in;
}

Weapon * MechEntity::findWeapon(const Weapon *weapon)
{
	for (MechPart *mechPart : parts)
		for (Weapon *current : mechPart->getWeapons())
			if (const_cast<const Weapon *>(current) == weapon)
				return current;
	return nullptr;

}

bool MechEntity::attackModifierCheck(const AttackObject &attack)
{
	BTech::DiceRoll roll = BTech::d2Throw();
	int total = attack.getTotalModifier(BTech::ModifierType::Attack);

	sendExtensiveInfo(BTech::ExtInfo::AttackModifierCheck);
	sendExtensiveInfo(BTech::ExtInfo::DisplayD2Roll.arg(roll));

	for (BTech::Modifier modifier : BTech::modifiers) {
		int modifierValue = attack.getModifier(BTech::ModifierType::Attack, modifier);
		if (modifierValue != 0) {
			sendExtensiveInfo(BTech::ExtInfo::DisplayValue
			                  .arg(BTech::modifierStringChange[modifier])
			                  .arg(modifierValue));
		}
	}

	sendExtensiveInfo(BTech::ExtInfo::SumOfTwoValues
	                  .arg(roll)
	                  .arg(total)
	                  .arg(roll + total));

	if (roll + total > 12) {
		sendExtensiveInfo(BTech::ExtInfo::AttackMissed);
		return false;
	} else {
		sendExtensiveInfo(BTech::ExtInfo::Hit);
		return true;
	}
}

bool MechEntity::armorPenetrationCheck(const AttackObject &attack)
{
	BTech::DiceRoll roll = BTech::d2Throw();
	int total = attack.getTotalModifier(BTech::ModifierType::ArmorPenetration);

	sendExtensiveInfo(BTech::ExtInfo::ArmorPenetrationCheck);
	sendExtensiveInfo(BTech::ExtInfo::DisplayD2Roll.arg(roll));
	sendExtensiveInfo(BTech::ExtInfo::DisplayGeneralModifier
	                  .arg(total));
	sendExtensiveInfo(BTech::ExtInfo::SubstractionOfTwoValues
	                  .arg(roll)
	                  .arg(total)
	                  .arg(roll - total));

	if (roll < total) {
		sendExtensiveInfo(BTech::ExtInfo::AttackDeflected);
		return false;
	} else {
		sendExtensiveInfo(BTech::ExtInfo::ArmorPenetrated);
		return true;
	}
}

MechEntity::HitLocation MechEntity::getHitLocation(const AttackObject &attack)
{
	BTech::DiceRoll roll = BTech::d2Throw();

	sendExtensiveInfo(BTech::ExtInfo::DeterminingHitLocation);
	sendExtensiveInfo(BTech::ExtInfo::DisplayD2Roll
	                  .arg(roll));
	sendExtensiveInfo(BTech::ExtInfo::DisplayAttackDirection
	                  .arg(BTech::directionSideStringChange[attack.getDirection()]));

	QPair <BTech::MechPartType, BTech::MechPartSide> hitLocation =
		BTech::hitLocationTable[{roll, BTech::directionToMechPartSide[attack.getDirection()]}];

	while (findMechPart(hitLocation.first, hitLocation.second)->hasEffect(BTech::EffectType::Destroyed)) {
		switch (hitLocation.first) {
			case BTech::MechPartType::Arm:
			case BTech::MechPartType::Leg:
				hitLocation.first = BTech::MechPartType::Torso;
				break;
			case BTech::MechPartType::Torso:
				hitLocation.second = BTech::MechPartSide::Center;
				break;
			default:;
		}
	}

	HitLocation result;
	result.type = hitLocation.first;
	result.side = hitLocation.second;
	result.critical = (roll == 2);

	QString sideString;
	if (result.side != BTech::MechPartSide::General)
		sideString = BTech::mechPartSideStringChange[result.side];

	if (result.critical)
		sendExtensiveInfo(BTech::ExtInfo::Critical);
	sendExtensiveInfo(BTech::ExtInfo::HitPart
	                  .arg(sideString)
	                  .arg(BTech::mechPartTypeStringChange[result.type]));

	return result;
}

void MechEntity::sendInfo(const QString &info)
{
	this->info = info;
	emit infoSent();
}

void MechEntity::sendExtensiveInfo(const QString &info)
{
	this->info = info;
	emit extensiveInfoSent();
}

QList <const Action *> MechEntity::getActions_BBD(BTech::GamePhase phase) const
{
	QList <const Action *> result;

	switch (phase) {
		case BTech::GamePhase::Movement:
			if (hasEffect(BTech::EffectType::Immobilised))
				break;

			result.append(new const MovementAction(BTech::MovementAction::Walk));

			if (!hasEffect(BTech::EffectType::CannotRun)) {
				result.append({
					new const MovementAction(BTech::MovementAction::Run),
					new const MovementAction(BTech::MovementAction::Jump),
				});
			}
			break;
		case BTech::GamePhase::Combat:
			if (!hasEffect(BTech::EffectType::CannotAttack))
				result.append(new const CombatAction(BTech::CombatAction::SimpleAttack));
			break;
		default:;
	}

	return result;
}

QList <const Action *> MechEntity::getActions_ABD(BTech::GamePhase phase) const
{
	QList <const Action *> result;

	switch (phase) {
		case BTech::GamePhase::Movement:

			if (hasEffect(BTech::EffectType::Immobilised))
				break;

			result.append(new const MovementAction(BTech::MovementAction::Walk));

			if (!hasEffect(BTech::EffectType::CannotRun)) {
				result.append({
					new const MovementAction(BTech::MovementAction::Run),
					new const MovementAction(BTech::MovementAction::Jump),
				});
			}

			result.append({
				new const MovementAction(BTech::MovementAction::TurnLeft),
				new const MovementAction(BTech::MovementAction::TurnRight),
			});
			break;

		case BTech::GamePhase::Combat:

			if (hasEffect(BTech::EffectType::CannotAttack))
				break;

			for (const MechPart *mechPart : getMechParts(BTech::MechPartType::Arm))
				result.append(new const CombatAction(BTech::CombatAction::Punch,
				                                     this,
				                                     mechPart->getSide()));
			result.append({
				new const CombatAction(BTech::CombatAction::Kick),
				new const CombatAction(BTech::CombatAction::Punch),
			});

			//TODO check if everything
			if (!hasEffect(BTech::EffectType::CannotShoot)) {
				for (const Weapon *weapon : getWeapons()) {
					setCurrentWeapon(weapon);
					result.append(new const CombatAction(BTech::CombatAction::WeaponAttack,
					                                     this));
				}
			}
			break;

		default:;
	}

	return result;
}

void MechEntity::resolveAttacks_BBD()
{
	if (!incomingAttacks.isEmpty())
		sendExtensiveInfo(); /** signature */
	while (!incomingAttacks.empty()) {
		AttackObject attack = incomingAttacks.first();
		bool rollAgain = true;
		Effect effect1, effect2;

		sendExtensiveInfo(BTech::ExtInfo::AttackedBy
		                  .arg(attack.getWeaponHolder()->getUnitName())
		                  .arg(attack.getWeaponHolder()->getOwnerName()));

		if (attackModifierCheck(attack) && armorPenetrationCheck(attack)) {
			while (!hasEffect(BTech::EffectType::Destroyed) && rollAgain) {
				rollAgain = false;
				BTech::DiceRoll diceRoll = BTech::d2Throw();

				switch (diceRoll) {
				case 2:
				case 3:
				case 4:
				case 10:
				case 11:
				case 12:
					effect1 = Effect(BTech::EffectType::Destroyed,
					                 BTech::EffectSource::Attack,
					                 Effect::FOREVER);
					break;
				case 5:
					if (hasEffect(BTech::EffectType::CannotAttack))
						rollAgain = true;
					else
						effect1 = Effect(BTech::EffectType::CannotAttack,
						                 BTech::EffectSource::Attack,
						                 Effect::FOREVER);
					break;
				case 6:
				case 8:
					effect1 = Effect(BTech::EffectType::Immobilised,
					                 BTech::EffectSource::Attack,
					                 qMax(getEffect(BTech::EffectType::Immobilised).getDuration(), 3));
					effect2 = Effect(BTech::EffectType::CannotAttack,
					                 BTech::EffectSource::Attack,
					                 qMax(getEffect(BTech::EffectType::CannotAttack).getDuration(), 3));
					break;
				case 7:
					effect1 = Effect(BTech::EffectType::Immobilised,
					                 BTech::EffectSource::Attack,
					                 qMax(getEffect(BTech::EffectType::Immobilised).getDuration(), 2));
					effect2 = Effect(BTech::EffectType::CannotAttack,
					                 BTech::EffectSource::Attack,
					                 qMax(getEffect(BTech::EffectType::CannotAttack).getDuration(), 2));
					break;
				case 9:
					if (!hasEffect(BTech::EffectType::Slowed, BTech::EffectSource::Attack))
						effect1 = Effect(BTech::EffectType::Slowed,
						                 BTech::EffectSource::Attack,
						                 Effect::FOREVER);
					else if (!hasEffect(BTech::EffectType::Immobilised, BTech::EffectSource::Attack))
						effect1 = Effect(BTech::EffectType::Immobilised,
						                 BTech::EffectSource::Attack,
						                 Effect::FOREVER);
					else
						rollAgain = true;
					break;
				}
			}

			sendExtensiveInfo(BTech::ExtInfo::ListEffects);
			if (effect1.isActive()) {
				sendExtensiveInfo(QString(effect1));
				setEffect(effect1);
			}
			if (effect2.isActive()) {
				sendExtensiveInfo(QString(effect2));
				setEffect(effect2);
			}

			sendInfo(BTech::Strings::MechInfoHit
			         .arg(attack.getWeaponHolder()->getUnitName())
			         .arg(QString(effect1))
			         .arg(QString(effect2)));
		}
		incomingAttacks.removeFirst();
	}
}

//TODO weapon ammo

void MechEntity::resolveAttacks_ABD()
{
	if (!incomingAttacks.isEmpty())
		sendExtensiveInfo(); /** signature */
	while (!incomingAttacks.isEmpty()) {
		AttackObject attack = incomingAttacks.first();
		incomingAttacks.removeFirst();

		if (attackModifierCheck(attack)) {
			HitLocation hitLocation = getHitLocation(attack);
			findMechPart(hitLocation.type, hitLocation.side)->applyDamage(attack.getDamage());
		}
	}
}

int MechEntity::getDistanceAttackModifier(int distance) const
{
	if (distance == 1)
		return 0;
	else
		return 2;
}

void MechEntity::clearData()
{
	delete mechWarrior;
	mechWarrior = nullptr;
	Mech::clearData();
	editEffects().clear();
}

/**
 * \namespace BTech
 */

const QHash <Direction, BTech::MechPartSide> BTech::directionToMechPartSide {
	{DirectionFront,      BTech::MechPartSide::Center},
	{DirectionRightFront, BTech::MechPartSide::Right},
	{DirectionRightRear,  BTech::MechPartSide::Right},
	{DirectionRear,       BTech::MechPartSide::Center},
	{DirectionLeftRear,   BTech::MechPartSide::Left},
	{DirectionLeftFront,  BTech::MechPartSide::Left},
};

const QHash <QPair <BTech::DiceRoll, BTech::MechPartSide>, QPair <BTech::MechPartType, BTech::MechPartSide> > BTech::hitLocationTable {
	{{ 2, BTech::MechPartSide::Left},   {BTech::MechPartType::Torso, BTech::MechPartSide::Left}},
	{{ 2, BTech::MechPartSide::Center}, {BTech::MechPartType::Torso, BTech::MechPartSide::Center}},
	{{ 2, BTech::MechPartSide::Right},  {BTech::MechPartType::Torso, BTech::MechPartSide::Right}},

	{{ 3, BTech::MechPartSide::Left},   {BTech::MechPartType::Leg,   BTech::MechPartSide::Left}},
	{{ 3, BTech::MechPartSide::Center}, {BTech::MechPartType::Arm,   BTech::MechPartSide::Right}},
	{{ 3, BTech::MechPartSide::Right},  {BTech::MechPartType::Leg,   BTech::MechPartSide::Right}},

	{{ 4, BTech::MechPartSide::Left},   {BTech::MechPartType::Arm,   BTech::MechPartSide::Left}},
	{{ 4, BTech::MechPartSide::Center}, {BTech::MechPartType::Arm,   BTech::MechPartSide::Right}},
	{{ 4, BTech::MechPartSide::Right},  {BTech::MechPartType::Arm,   BTech::MechPartSide::Right}},

	{{ 5, BTech::MechPartSide::Left},   {BTech::MechPartType::Arm,   BTech::MechPartSide::Left}},
	{{ 5, BTech::MechPartSide::Center}, {BTech::MechPartType::Leg,   BTech::MechPartSide::Right}},
	{{ 5, BTech::MechPartSide::Right},  {BTech::MechPartType::Arm,   BTech::MechPartSide::Right}},

	{{ 6, BTech::MechPartSide::Left},   {BTech::MechPartType::Leg,   BTech::MechPartSide::Left}},
	{{ 6, BTech::MechPartSide::Center}, {BTech::MechPartType::Torso, BTech::MechPartSide::Right}},
	{{ 6, BTech::MechPartSide::Right},  {BTech::MechPartType::Leg,   BTech::MechPartSide::Right}},

	{{ 7, BTech::MechPartSide::Left},   {BTech::MechPartType::Torso, BTech::MechPartSide::Left}},
	{{ 7, BTech::MechPartSide::Center}, {BTech::MechPartType::Torso, BTech::MechPartSide::Center}},
	{{ 7, BTech::MechPartSide::Right},  {BTech::MechPartType::Torso, BTech::MechPartSide::Right}},

	{{ 8, BTech::MechPartSide::Left},   {BTech::MechPartType::Torso, BTech::MechPartSide::Center}},
	{{ 8, BTech::MechPartSide::Center}, {BTech::MechPartType::Torso, BTech::MechPartSide::Left}},
	{{ 8, BTech::MechPartSide::Right},  {BTech::MechPartType::Torso, BTech::MechPartSide::Center}},

	{{ 9, BTech::MechPartSide::Left},   {BTech::MechPartType::Torso, BTech::MechPartSide::Right}},
	{{ 9, BTech::MechPartSide::Center}, {BTech::MechPartType::Leg,   BTech::MechPartSide::Left}},
	{{ 9, BTech::MechPartSide::Right},  {BTech::MechPartType::Torso, BTech::MechPartSide::Left}},

	{{10, BTech::MechPartSide::Left},   {BTech::MechPartType::Arm,   BTech::MechPartSide::Right}},
	{{10, BTech::MechPartSide::Center}, {BTech::MechPartType::Arm,   BTech::MechPartSide::Left}},
	{{10, BTech::MechPartSide::Right},  {BTech::MechPartType::Arm,   BTech::MechPartSide::Left}},

	{{11, BTech::MechPartSide::Left},   {BTech::MechPartType::Leg,   BTech::MechPartSide::Right}},
	{{11, BTech::MechPartSide::Center}, {BTech::MechPartType::Arm,   BTech::MechPartSide::Left}},
	{{11, BTech::MechPartSide::Right},  {BTech::MechPartType::Leg,   BTech::MechPartSide::Left}},

	{{12, BTech::MechPartSide::Left},   {BTech::MechPartType::Head,  BTech::MechPartSide::Center}},
	{{12, BTech::MechPartSide::Center}, {BTech::MechPartType::Head,  BTech::MechPartSide::Center}},
	{{12, BTech::MechPartSide::Right},  {BTech::MechPartType::Head,  BTech::MechPartSide::Center}},
};

const QHash <BTech::MovementAction, int> BTech::attackerMovementModifierTable {
	{BTech::MovementAction::Idle, 0},
	{BTech::MovementAction::Walk, 1},
	{BTech::MovementAction::Run,  2},
	{BTech::MovementAction::Jump, 3},
};

const QHash <int, int> BTech::targetMovementModifierTable {
	{2, 0},
	{4, 1},
	{6, 2},
	{9, 3},
};

const int BTech::armorPenetrationTable[BTech::MAX_POSSIBLE_DAMAGE + 1][BTech::MAX_POSSIBLE_ARMOR_VALUE + 1] = {
	{7,  7,  8,  8,  9,  9, 10, 10, 10, 11, 11, 11, 11, 11},
	{6,  7,  7,  8,  8,  9,  9, 10, 10, 10, 11, 11, 11, 11},
	{6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 10, 11, 11, 11},
	{5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 10, 11, 11},
	{5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 10, 11},
	{4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 10},
	{4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10},
	{3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10},
	{3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9},
	{3,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9},
	{3,  3,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8},
	{3,  3,  3,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8},
	{0,  3,  3,  3,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7},
	{0,  0,  3,  3,  3,  3,  3,  4,  4,  5,  5,  6,  6,  7},
	{0,  0,  0,  3,  3,  3,  3,  3,  4,  4,  5,  5,  6,  6},
	{0,  0,  0,  0,  3,  3,  3,  3,  3,  4,  4,  5,  5,  6},
	{0,  0,  0,  0,  0,  3,  3,  3,  3,  3,  4,  4,  5,  5},
};
