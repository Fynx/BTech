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

#ifndef BTMEDITOR_STRINGS_H
#define BTMEDITOR_STRINGS_H

#include <QtCore>
#include "BTCommon/CommonStrings.h"

namespace BTech {
	namespace Strings {
		const QString MenuActionNewMap      = QObject::tr("New");
		const QString MenuActionSaveMap     = QObject::tr("Save");
		const QString MenuActionSaveAsMap   = QObject::tr("Save as");
		const QString MenuEdit              = QObject::tr("&Edit");
		const QString MenuActionEditWeapons = QObject::tr("Edit &weapons");
		const QString MenuActionEditMechs   = QObject::tr("Edit &mechs");
		const QString MenuActionSaveData    = QObject::tr("Save &data");

		const QString DialogSaveMap           = QObject::tr("Save map");
		const QString DialogQuitWithoutSaving = QObject::tr("Quit message");

		const QString ButtonOk                    = QObject::tr("&Ok");
		const QString ButtonConfirm               = QObject::tr("Con&firm");
		const QString ButtonCancel                = QObject::tr("&Cancel");
		const QString ButtonSaveMechPart          = QObject::tr("&Save part");
		const QString ButtonAddWeapon             = QObject::tr("Add &weapon");
		const QString ButtonRemoveWeapon          = QObject::tr("R&emove weapon");
		const QString ButtonSaveWeapon            = QObject::tr("Save wea&pon");
		const QString ButtonAddMech               = QObject::tr("&Add mech");
		const QString ButtonRemoveMech            = QObject::tr("&Remove mech");
		const QString ButtonSaveMech              = QObject::tr("&Save mech");
		const QString ButtonCreateMap             = QObject::tr("&Create map");
		const QString ButtonSaveChanges           = QObject::tr("Save changes");
		const QString ButtonSaveAndQuit           = QObject::tr("Save and Quit");
		const QString ButtonDiscardChanges        = QObject::tr("Discard changes");
		const QString ButtonAddNewPlayer          = QObject::tr("Add new player");
		const QString ButtonRemovePlayer          = QObject::tr("Remove player");
		const QString ButtonSavePlayerDescription = QObject::tr("Save player description");
		const QString ButtonSaveMapDescription    = QObject::tr("Save map description");
		const QString ButtonVersions              = QObject::tr("Versions");

		const QString LabelMaxArmorValue       = QObject::tr("Armor value");
		const QString LabelArmorValues         = QObject::tr("Structure");
		const QString LabelMaxInternalValue    = QObject::tr("Internal value");
		const QString LabelMechParts           = QObject::tr("Parts");
		const QString LabelMechStats           = QObject::tr("Stats");
		const QString LabelMapProperties       = QObject::tr("Map properties");
		const QString LabelClickMode           = QObject::tr("Click mode");
		const QString LabelUnit                = QObject::tr("Unit");
		const QString LabelMechType            = QObject::tr("Mech type");
		const QString LabelDirection           = QObject::tr("Direction");
		const QString LabelWidth               = QObject::tr("Width");
		const QString LabelModifier            = QObject::tr("Modifier");
		const QString LabelMaximal             = QObject::tr("Maximal");
		const QString LabelMinimal             = QObject::tr("Minimal");
		const QString LabelRange               = QObject::tr("Range");
		const QString LabelRangeModifiers      = QObject::tr("Range modifiers");
		const QString LabelRangeThresholds     = QObject::tr("Range thresholds");
		const QString LabelWeaponName          = QObject::tr("Weapon name");
		const QString LabelWeaponParameters    = QObject::tr("Weapon parameters");
		const QString LabelHeat                = QObject::tr("Heat");
		const QString LabelTonnage             = QObject::tr("Tonnage");
		const QString LabelCriticalSpaces      = QObject::tr("Critical spaces");
		const QString LabelAmmoShotsPerTon     = QObject::tr("Ammo per ton");
		const QString LabelMissilesNumberShot  = QObject::tr("Missiles per shot");
		const QString LabelWeaponType          = QObject::tr("Weapon type");
		const QString LabelWeaponTypeEnergy    = WeaponTypeEnergy;
		const QString LabelWeaponTypeBallistic = WeaponTypeBallistic;
		const QString LabelWeaponTypeMissile   = WeaponTypeMissile;
		const QString LabelAvailableWeapons    = QObject::tr("Available weapons");
		const QString LabelWeaponsUsed         = QObject::tr("Weapons used");
		const QString LabelArmorValue          = QObject::tr("Armor value");
		const QString LabelMaxMovePoints       = QObject::tr("Max move points");
		const QString LabelMaxJumpPoints       = QObject::tr("Max jump points");
		const QString LabelBaseFireRange       = QObject::tr("Base fire range");
		const QString LabelHeatSinksNumber     = QObject::tr("Heat sinks number");
		const QString LabelPlayers             = QObject::tr("Players");
		const QString LabelMap                 = QObject::tr("Map");

		const QString UnnamedMech   = QObject::tr("Unnamed Mech");
		const QString UnnamedWeapon = QObject::tr("Unnamed Weapon");
		const QString UnnamedPlayer = QObject::tr("UnnamedPlayer");

		const QString EditMechsDialog   = QObject::tr("Edit mechs");
		const QString EditWeaponsDialog = QObject::tr("Edit weapons");

		const QString ErrorWeaponNameEmpty  = QObject::tr("Weapon name cannot be empty");
		const QString ErrorWeaponNameExists = QObject::tr("A weapon with the same name has already been defined");
		const QString ErrorMechNameEmpty    = QObject::tr("Mech name cannot be empty");
		const QString ErrorMechNameExists   = QObject::tr("A mech with the same name has already been defined");

		const QString RemoveMech        = QObject::tr("Remove mech");
		const QString RemoveMechWarning = QObject::tr("Do you really want to remove this mech? All previous versions will be erased.");

		const QString GameVersionDialogWarning = QObject::tr("At least one version must be checked.");

		const QString QuitWithoutSavingWarning = QObject::tr("Data has been changed. Do you want to save it?");
	}
}

#endif // BTMEDITOR_STRINGS_H
