/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
Copyright (C) 2014 by Krzysztof Adamek <squadack [at] students [dot] mimuw [dot] edu [dot] pl>
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

#ifndef WEAPON_MODEL_H
#define WEAPON_MODEL_H

#include <QtWidgets>
#include "BTCommon/FileIO.h"
#include "BTCommon/Paths.h"
#include "BTCommon/Uid.h"
#include "BTCommon/Utils.h"

class WeaponBase;

/**
 * \class WeaponModel
 * Data model for storing \ref Weapon "Weapons"
 */

class WeaponModel : public QAbstractTableModel
{
Q_OBJECT;

public:
	int columnCount(const QModelIndex &index = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	bool isChanged() const;
	void setChanged(bool changed);

	static WeaponModel & getInstance();
	static bool loadFromFile(const QString &fileName = BTech::Paths::WEAPONS_PATH);
	static bool saveToFile(const QString &fileName = BTech::Paths::WEAPONS_PATH);

	static void addNewWeapon();
	static bool empty();
	static const WeaponBase * getRow(int row);
	static const WeaponBase * getWeapon(UID uid);
	static const WeaponBase * getWeapon(const QModelIndex &index);
	static const WeaponBase * getWeapon(const QString &name);
	static const QList <WeaponBase *> & getWeapons();
	static bool hasWeapon(const QString &name);
	static void removeWeapon(UID uid);

	static const int Name                 =  0;
	static const int Type                 =  1;
	static const int MinimalRange         =  2;
	static const int MaximalShortRange    =  3;
	static const int MaximalMediumRange   =  4;
	static const int MaximalLongRange     =  5;
	static const int Heat                 =  6;
	static const int Damage               =  7;
	static const int Tonnage              =  8;
	static const int CriticalSpaces       =  9;
	static const int AmmoPerTon           = 10;
	static const int MissilesPerShot      = 11;
	static const int ColumnCount          = 12;

	friend QDataStream & operator << (QDataStream &out, const WeaponModel &wModel);
	friend QDataStream & operator >> (QDataStream &in, WeaponModel &wModel);
	friend bool ::saveToFile<WeaponModel>(const WeaponModel &obj, const QString &fileName);

private:
	explicit WeaponModel(QObject *parent = nullptr);
	WeaponModel(const WeaponModel &) = delete;
	WeaponModel(WeaponModel &&) = delete;
	void operator=(const WeaponModel &) = delete;
	void operator=(WeaponModel &&) = delete;
	~WeaponModel();

	void addWeapon(int row, WeaponBase *weapon);
	void removeWeaponFromRow(int row);

	bool changed;
	UID nextUid;

	static QList <WeaponBase *> weaponList;
	static QHash <UID, WeaponBase *> uidToWeapon;
};

#endif
