#ifndef COMMON_STRINGS_H
#define COMMON_STRINGS_H

#include <QtCore>

namespace BTech {
	namespace Strings {
		const QString EntityMech = QObject::tr("Mech");

		const QString Head  = QObject::tr("Head");
		const QString Torso = QObject::tr("Torso");
		const QString Arm   = QObject::tr("Arm");
		const QString Leg   = QObject::tr("Leg");

		const QString General = QObject::tr("General");
		const QString Center  = QObject::tr("Center");
		const QString Left    = QObject::tr("Left");
		const QString Right   = QObject::tr("Right");
		const QString Front   = QObject::tr("Front");
		const QString Rear    = QObject::tr("Rear");

		const QString PhaseInitiative     = QObject::tr("Initiative Phase");
		const QString PhaseMovement       = QObject::tr("Movement Phase");
		const QString PhaseReaction       = QObject::tr("Reaction Phase");
		const QString PhaseWeaponAttack   = QObject::tr("Weapon Attack Phase");
		const QString PhasePhysicalAttack = QObject::tr("Physical Attack Phase");
		const QString PhaseCombat         = QObject::tr("Combat Phase");
		const QString PhaseHeat           = QObject::tr("Heat Phase");
		const QString PhaseEnd            = QObject::tr("End Phase");

		const QString BasicBattleDroids    = QObject::tr("Basic BattleDroids");
		const QString AdvancedBattleDroids = QObject::tr("Advanced BattleDroids");

		const QString PlayerNone     = QObject::tr("Player None");
		const QString PlayerComputer = QObject::tr("Computer Player");

		const QString TerrainClear = QObject::tr("Clear");
		const QString TerrainLightWoods = QObject::tr("Light woods");
		const QString TerrainHeavyWoods = QObject::tr("Heavy woods");
		const QString TerrainRough = QObject::tr("Rough");
		const QString TerrainWater = QObject::tr("Water");

		const QString DirectionN  = QObject::tr("N");
		const QString DirectionNE = QObject::tr("NE");
		const QString DirectionSE = QObject::tr("SE");
		const QString DirectionS  = QObject::tr("S");
		const QString DirectionSW = QObject::tr("SW");
		const QString DirectionNW = QObject::tr("NW");

		const QString DirectionFront      = QObject::tr("front");
		const QString DirectionRightFront = QObject::tr("right front");
		const QString DirectionRightRear  = QObject::tr("right rear");
		const QString DirectionRear       = QObject::tr("rear");
		const QString DirectionLeftRear   = QObject::tr("left rear");
		const QString DirectionLeftFront  = QObject::tr("left front");

		const QString RangeContact = QObject::tr("Contact Range");
		const QString RangeShort   = QObject::tr("Short Range");
		const QString RangeMedium  = QObject::tr("Medium Range");
		const QString RangeLong    = QObject::tr("Long Range");

		const QString WeaponTypeEnergy    = QObject::tr("Energy");
		const QString WeaponTypeBallistic = QObject::tr("Ballistic");
		const QString WeaponTypeMissile   = QObject::tr("Missile");

		const QString DefaultNewWeaponName = QObject::tr("New weapon");
		const QString DefaultNewMechName   = QObject::tr("New mech");

		const QString EffectNone           = QObject::tr("none");
		const QString EffectDestroyed      = QObject::tr("destroyed");
		const QString EffectShutDown       = QObject::tr("shut down");
		const QString EffectImmobilised    = QObject::tr("immobilised");
		const QString EffectSlowed         = QObject::tr("slowed");
		const QString EffectCannotRun      = QObject::tr("cannot run");
		const QString EffectCannotShoot    = QObject::tr("cannot shoot");
		const QString EffectCannotAttack   = QObject::tr("cannot attack");
		const QString EffectAimingBothered = QObject::tr("aiming bothered");
		const QString EffectWalked         = QObject::tr("walked");
		const QString EffectRun            = QObject::tr("run");
		const QString EffectJumped         = QObject::tr("jumped");

		const QString DurationPermanent = QObject::tr("permanently");
		const QString DurationUndefined = QObject::tr("undefined");

		const QString ModifierBase             = QObject::tr("base");
		const QString ModifierRange            = QObject::tr("range");
		const QString ModifierDirection        = QObject::tr("direction");
		const QString ModifierTerrain          = QObject::tr("terrain");
		const QString ModifierHeight           = QObject::tr("height");
		const QString ModifierAttackerMovement = QObject::tr("attacker movement");
		const QString ModifierTargetMovement   = QObject::tr("target movement");

		const QString MenuFile                    = QObject::tr("&File");
		const QString MenuActionLoadMap           = QObject::tr("Load map");
		const QString MenuActionQuit              = QObject::tr("Quit");
		const QString MenuView                    = QObject::tr("&View");
		const QString MenuActionToggleGrid        = QObject::tr("Toggle hexagonal &grid");
		const QString MenuActionToggleCoordinates = QObject::tr("Toggle &coordinates");
		const QString MenuActionToggleInfoBar     = QObject::tr("Toggle &infobar");

		const QString DialogOpenFile               = QObject::tr("Open file");
		const QString DialogBTechMapFilesExtension = QObject::tr("btm");
		const QString DialogBTechMapFiles          = QObject::tr("BTech Map files (*.%1)").arg(DialogBTechMapFilesExtension);

		const QString LabelArmor      = QObject::tr("Armor");
		const QString LabelDamage     = QObject::tr("Damage");
		const QString LabelHeight     = QObject::tr("Height");
		const QString LabelHex        = QObject::tr("Hex");
		const QString LabelJumpPoints = QObject::tr("Jump Points");
		const QString LabelMovePoints = QObject::tr("Move Points");
		const QString LabelRunPoints  = QObject::tr("Run Points");
		const QString LabelNone       = QObject::tr("None");
		const QString LabelTerrain    = QObject::tr("Terrain");
		const QString LabelType       = QObject::tr("Type");
		const QString LabelUnits      = QObject::tr("Units");

		const QString None      = QObject::tr("None");
		const QString Walk      = QObject::tr("Walk");
		const QString Run       = QObject::tr("Run");
		const QString Jump      = QObject::tr("Jump");
		const QString TurnRight = QObject::tr("Turn right");
		const QString TurnLeft  = QObject::tr("Turn left");
		const QString Attack    = QObject::tr("Attack");
		const QString Kick      = QObject::tr("Kick");
		const QString Punch     = QObject::tr("Punch");
		const QString Push      = QObject::tr("Push");
		const QString Charge    = QObject::tr("Charge");

		const QString MechInfoHit = QObject::tr("hit by %1: %2 %3");
		const QString AttackMissed  = QObject::tr("attack missed");
		const QString AttackStopped = QObject::tr("attack stopped");
	}

	namespace Messages {
		const QString Separator = QObject::tr("-----------------------------------------");

		const QString MapLoaded = QObject::tr("Map: ");

		const QString GameStarted    = QObject::tr("Game started!");
		const QString GameOver       = QObject::tr("Game Over");
		const QString ListInitiative = QObject::tr("initiative:");

		const QString WinMessage = QObject::tr("The winner is:");
		const QString NoWinner   = QObject::tr("All units destroyed - no one wins.");
	}

	namespace ExtInfo {
		const QString ArmorPenetrated         = QObject::tr("armor penetrated!\n");
		const QString ArmorPenetrationCheck   = QObject::tr("Armor penetration check\n");
		const QString AttackDeflected         = QObject::tr("attack deflected.\n");
		const QString AttackMissed            = QObject::tr("missed.\n");
		const QString AttackModifierCheck     = QObject::tr("Attack modifier check\n");
		const QString AttackedBy              = QObject::tr("attacked by %1 (%2)\n");
		const QString Critical                = QObject::tr("critical ");
		const QString DisplayAttackDirection  = QObject::tr("\tattack direction: %1\n");
		const QString DeterminingHitLocation  = QObject::tr("Determining hit location\n");
		const QString DisplayD2Roll           = QObject::tr("\td2 roll: %1\n");
		const QString DisplayGeneralModifier  = QObject::tr("\tmodifier: %1\n");
		const QString DisplayValue            = QObject::tr("\t%1: %2\n");
		const QString ListEffects             = QObject::tr("Effects:\n");
		const QString Hit                     = QObject::tr("hit!\n");
		const QString HitPart                 = QObject::tr("hit %1 %2\n");
		const QString MovedToHex              = QObject::tr("moved to hex %1\n");
		const QString SubstractionOfTwoValues = QObject::tr("%1 - %2 = %3\n");
		const QString SumOfTwoValues          = QObject::tr("%1 + %2 = %3\n");
	}
}

#endif
