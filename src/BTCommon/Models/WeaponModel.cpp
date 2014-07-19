/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
Copyright (C) 2014 by Bartosz Szreder <szreder [at] mimuw [dot] edu [dot] pl>
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

#include "BTCommon/EnumSerialization.h"

#include "BTCommon/Bases/WeaponBase.h"
#include "BTCommon/Models/WeaponModel.h"

/**
 * \class WeaponModel
 */

QList <WeaponBase *> WeaponModel::weaponList;
QHash <UID, WeaponBase *> WeaponModel::uidToWeapon;

int WeaponModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	return ColumnCount;
}

QVariant WeaponModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::DisplayRole || role == Qt::EditRole) {
		const WeaponBase *weapon = weaponList[index.row()];
		switch (index.column()) {
			case Name:                 return weapon->getName();
			case Type:                 return toUnderlying(weapon->getWeaponType());
			case MinimalRange:         return weapon->getMinRange();
			case MaximalShortRange:    return weapon->getMaxRange(BTech::Range::Short);
			case MaximalMediumRange:   return weapon->getMaxRange(BTech::Range::Medium);
			case MaximalLongRange:     return weapon->getMaxRange(BTech::Range::Long);
			case Heat:                 return weapon->getHeat();
			case Damage:               return weapon->getDamage();
			case Tonnage:              return weapon->getTonnage();
			case CriticalSpaces:       return weapon->getCriticalSpaces();
			case AmmoPerTon:           return weapon->getAmmoPerTon();
			case MissilesPerShot:      return weapon->getMissilesPerShot();
		}
	}

	return QVariant();
}

Qt::ItemFlags WeaponModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;
	return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable;
}

bool WeaponModel::insertRows(int row, int count, const QModelIndex &)
{
	int nameSuffix = 0;
	beginInsertRows(QModelIndex(), row, row + count - 1);
	for (int i = 0; i < count; ++i) {
		QString weaponName;
		do {
			++nameSuffix;
			weaponName = QString("%1 %2").arg(BTech::Strings::DefaultNewWeaponName).arg(nameSuffix);
		} while (hasWeapon(weaponName));

		weaponList.insert(row + i, new WeaponBase(weaponName, nextUid++));
	}
	endInsertRows();

	return true;
}

bool WeaponModel::removeRows(int row, int count, const QModelIndex &)
{
	beginRemoveRows(QModelIndex(), row, row + count - 1);
	for (int i = 0; i < count; ++i)
		removeWeaponFromRow(row);
	endRemoveRows();

	return true;
}

int WeaponModel::rowCount(const QModelIndex &) const
{
	return weaponList.count();
}

bool WeaponModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid() || role != Qt::EditRole)
		return false;

	WeaponBase *weapon = weaponList[index.row()];
	switch (index.column()) {
		case Name:                 weapon->setName(value.toString()); break;
		case Type:                 weapon->setWeaponType(static_cast<BTech::WeaponType>(value.toInt())); break;
		case MinimalRange:         weapon->setMinRange(value.toInt()); break;
		case MaximalShortRange:    weapon->setMaxRange(BTech::Range::Short, value.toInt()); break;
		case MaximalMediumRange:   weapon->setMaxRange(BTech::Range::Medium, value.toInt()); break;
		case MaximalLongRange:     weapon->setMaxRange(BTech::Range::Long, value.toInt()); break;
		case Heat:                 weapon->setHeat(value.toInt()); break;
		case Damage:               weapon->setDamage(value.toInt()); break;
		case Tonnage:              weapon->setTonnage(value.toInt()); break;
		case CriticalSpaces:       weapon->setCriticalSpaces(value.toInt()); break;
		case AmmoPerTon:           weapon->setAmmoPerTon(value.toInt()); break;
		case MissilesPerShot:      weapon->setMissilesPerShot(value.toInt()); break;
		default:                   return false;
	}

	emit dataChanged(index, index);

	return true;
}

bool WeaponModel::isChanged() const
{
	return changed;
}

void WeaponModel::setChanged(bool changed)
{
	this->changed = changed;
}

WeaponModel & WeaponModel::getInstance()
{
	static WeaponModel *instance = new WeaponModel;
	return *instance;
}

bool WeaponModel::loadFromFile(const QString &fileName)
{
	return ::loadFromFile(getInstance(), BTech::resolvePath(fileName));
}

bool WeaponModel::saveToFile(const QString &fileName)
{
	return ::saveToFile(getInstance(), BTech::resolvePath(fileName));
}

void WeaponModel::addNewWeapon()
{
	getInstance().insertRows(weaponList.size(), 1);
}

bool WeaponModel::empty()
{
	return weaponList.empty();
}

const WeaponBase * WeaponModel::getRow(int row)
{
	return weaponList[row];
}

const WeaponBase * WeaponModel::getWeapon(UID uid)
{
	return uidToWeapon.value(uid, nullptr);
}

const WeaponBase * WeaponModel::getWeapon(const QModelIndex &index)
{
	return getRow(index.row());
}

const WeaponBase * WeaponModel::getWeapon(const QString &name)
{
	const WeaponBase *result = nullptr;
	for (const WeaponBase *weapon : weaponList) {
		if (weapon->getName() == name)
			result = weapon;
	}
	return result;
}

const QList <WeaponBase *> & WeaponModel::getWeapons()
{
	return weaponList;
}

bool WeaponModel::hasWeapon(const QString &name)
{
	return getWeapon(name) != nullptr;
}

void WeaponModel::removeWeapon(UID uid)
{
	WeaponModel &wModel = getInstance();

	for (int i = 0; i < weaponList.count(); ++i) {
		if (weaponList[i]->getUid() == uid) {
			wModel.beginRemoveRows(QModelIndex(), i, i);
			delete weaponList.takeAt(i);
			wModel.endRemoveRows();

			uidToWeapon.remove(uid);
			return;
		}
	}
}

WeaponModel::WeaponModel(QObject *parent)
	: QAbstractTableModel(parent), changed(false), nextUid(MinUid)
{
	auto modelChanged = [this]{
		this->changed = true;
	};

	connect(this, &QAbstractTableModel::dataChanged, modelChanged);
	connect(this, &QAbstractTableModel::rowsInserted, modelChanged);
	connect(this, &QAbstractTableModel::rowsMoved, modelChanged);
	connect(this, &QAbstractTableModel::rowsRemoved, modelChanged);
}

WeaponModel::~WeaponModel()
{
	qDeleteAll(weaponList);
}

void WeaponModel::addWeapon(int row, WeaponBase *weapon)
{
	weaponList.insert(row, weapon);
	uidToWeapon[weapon->getUid()] = weapon;
}

void WeaponModel::removeWeaponFromRow(int row)
{
	uidToWeapon.remove(weaponList[row]->getUid());
	delete weaponList.takeAt(row);
}

QDataStream & operator << (QDataStream &out, const WeaponModel &wModel)
{
	out << wModel.nextUid << static_cast<UID>(wModel.weaponList.count());
	for (const WeaponBase *weapon : wModel.weaponList)
		out << *weapon;
	return out;
}

QDataStream & operator >> (QDataStream &in, WeaponModel &wModel)
{
	wModel.beginResetModel();
	qDeleteAll(wModel.weaponList);
	wModel.weaponList.clear();
	wModel.uidToWeapon.clear();

	UID count;
	in >> wModel.nextUid >> count;
	for (UID i = 0; i < count; ++i) {
		WeaponBase *weapon = new WeaponBase;
		in >> *weapon;
		wModel.addWeapon(i, weapon);
	}
	wModel.endResetModel();

	return in;
}
