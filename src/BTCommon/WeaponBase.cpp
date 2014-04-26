/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
Copyright (C) 2014 by Bartosz Szreder <szreder [at] mimuw [dot] edu [dot] pl>
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
#include "BTCommon/WeaponBase.h"

/**
 * \class WeaponBase
 */

WeaponBase::WeaponBase()
	: uid(EmptyUid)
{
	maxRange[BTech::Range::Contact] = 1;
	load(QString(), BTech::WeaponType::Energy,
	     0, 0, 0, 0, 0, 0, 0, 0,
	     BTech::DEFAULT_AMMO_PER_TON, BTech::DEFAULT_MISSILES_PER_SHOT);
}

WeaponBase::WeaponBase(const QString &name, UID uid)
	: WeaponBase()
{
	setName(name);
	this->uid = uid;
}

WeaponBase::WeaponBase(const WeaponBase &weapon)
{
	load(weapon);
}

WeaponBase::WeaponBase(const QString &name,
                       BTech::WeaponType weaponType,
                       int minRange,
                       int maxShortRange,
                       int maxMediumRange,
                       int maxLongRange,
                       int heat,
                       int damage,
                       int tonnage,
                       int criticalSpaces,
                       int ammoShotsPerTon,
                       int missilesNumberShot)
{
	load(name, weaponType,
	     minRange, maxShortRange, maxMediumRange, maxLongRange,
	     heat, damage, tonnage, criticalSpaces, ammoShotsPerTon, missilesNumberShot);
}

WeaponBase::operator QString () const
{
	return getName();
}

QString WeaponBase::getName() const
{
	return name;
}

void WeaponBase::setName(const QString &name)
{
	this->name = name;
}

BTech::WeaponType WeaponBase::getWeaponType() const
{
	return type;
}

void WeaponBase::setWeaponType(BTech::WeaponType weaponType)
{
	this->type = weaponType;
}

int WeaponBase::getRangeModifier(int distance) const
{
	BTech::Range range = distanceToRange(distance);
	int minimumDistanceModifier = qMax(0, getMinRange() - distance);
	return getRangeModifier(range) + minimumDistanceModifier;
}

int WeaponBase::getRangeModifier(BTech::Range range) const
{
	return rangeModifier[toUnderlying(range)];
}

BTech::Range WeaponBase::distanceToRange(int distance) const
{
	for (BTech::Range range : BTech::attackRanges)
		if (distance <= maxRange[range])
			return range;
	return BTech::Range::OutOfRange;
}

int WeaponBase::getMinRange() const
{
	return minRange;
}

void WeaponBase::setMinRange(int minRange)
{
	this->minRange = minRange;
}

int WeaponBase::getMaxRange(BTech::Range range) const
{
	return maxRange[range];
}

void WeaponBase::setMaxRange(BTech::Range range, int maxRange)
{
	this->maxRange[range] = maxRange;
}

int WeaponBase::getHeat() const
{
	return heat;
}

void WeaponBase::setHeat(int heat)
{
	this->heat = heat;
}

int WeaponBase::getDamage() const
{
	return damage;
}

void WeaponBase::setDamage(int damage)
{
	this->damage = damage;
}

int WeaponBase::getTonnage() const
{
	return tonnage;
}

void WeaponBase::setTonnage(int tonnage)
{
	this->tonnage = tonnage;
}

int WeaponBase::getCriticalSpaces() const
{
	return criticalSpaces;
}

void WeaponBase::setCriticalSpaces(int criticalSpaces)
{
	this->criticalSpaces = criticalSpaces;
}

int WeaponBase::getAmmoPerTon() const
{
	return ammoPerTon;
}

void WeaponBase::setAmmoPerTon(int ammoPerTon)
{
	this->ammoPerTon = ammoPerTon;
}

int WeaponBase::getMissilesPerShot() const
{
	return missilesPerShot;
}

void WeaponBase::setMissilesPerShot(int missilesPerShot)
{
	this->missilesPerShot = missilesPerShot;
}

QDataStream & operator << (QDataStream &out, const WeaponBase &weapon)
{
	out << weapon.name << weapon.uid << weapon.type;

	for (BTech::Range range : BTech::shootRanges)
		out << weapon.maxRange[range];
	out << weapon.minRange;

	out << weapon.heat << weapon.damage << weapon.tonnage << weapon.criticalSpaces << weapon.ammoPerTon << weapon.missilesPerShot;

	return out;
}

QDataStream & operator >> (QDataStream &in, WeaponBase &weapon)
{
	in >> weapon.name >> weapon.uid >> weapon.type;

	for (BTech::Range range : BTech::shootRanges)
		in >> weapon.maxRange[range];
	in >> weapon.minRange;

	in >> weapon.heat >> weapon.damage >> weapon.tonnage >> weapon.criticalSpaces >> weapon.ammoPerTon >> weapon.missilesPerShot;

	return in;
}

QDebug & operator << (QDebug &out, const WeaponBase &weapon)
{
	out << weapon.name << "(" << weapon.uid << ")"
	    << "\ntype:                      " << BTech::weaponTypeStringChange[weapon.type]
	    << "\nstats:"
	    << "\n\tminimal range:           " << weapon.minRange
	    << "\n\tmaximal ranges:          "
	    << weapon.maxRange[BTech::Range::Short]
	    << weapon.maxRange[BTech::Range::Medium]
	    << weapon.maxRange[BTech::Range::Long]
	    << "\n\theat:                    " << weapon.heat
	    << "\n\tdamage:                  " << weapon.damage
	    << "\n\ttonnage:                 " << weapon.tonnage
	    << "\n\tcritical spaces:         " << weapon.criticalSpaces
	    << "\n\tammo shots per ton:      " << weapon.ammoPerTon
	    << "\n\tmissiles number per shot:" << weapon.missilesPerShot;

	return out;
}

UID WeaponBase::getUid() const
{
	return uid;
}

const std::array <int, BTech::ranges.size()> WeaponBase::rangeModifier {
	BTech::CONTACT_RANGE_ATTACK_MODIFIER,
	BTech::SHORT_RANGE_ATTACK_MODIFIER,
	BTech::MEDIUM_RANGE_ATTACK_MODIFIER,
	BTech::LONG_RANGE_ATTACK_MODIFIER,
	BTech::INF_ATTACK_MODIFIER
};

void WeaponBase::load(const QString &name,
                      BTech::WeaponType weaponType,
                      int minRange,
                      int maxShortRange,
                      int maxMediumRange,
                      int maxLongRange,
                      int heat,
                      int damage,
                      int tonnage,
                      int criticalSpaces,
                      int ammoPerTon,
                      int missilesPerShot)
{
	this->name = name;
	this->type = weaponType;
	this->minRange = minRange;
	this->maxRange[BTech::Range::Short] = maxShortRange;
	this->maxRange[BTech::Range::Medium] = maxMediumRange;
	this->maxRange[BTech::Range::Long] = maxLongRange;
	this->heat = heat;
	this->damage = damage;
	this->tonnage = tonnage;
	this->criticalSpaces = criticalSpaces;
	this->ammoPerTon = ammoPerTon;
	this->missilesPerShot = missilesPerShot;
}

void WeaponBase::load(const WeaponBase &weapon)
{
	return load(weapon.name,
		    weapon.type,
	            weapon.minRange,
	            weapon.maxRange[BTech::Range::Short],
	            weapon.maxRange[BTech::Range::Medium],
	            weapon.maxRange[BTech::Range::Long],
	            weapon.heat,
	            weapon.damage,
	            weapon.tonnage,
	            weapon.criticalSpaces,
	            weapon.ammoPerTon,
	            weapon.missilesPerShot);
}

/**
 * \namespace BTech
 */

const QHash <BTech::Modifier, QString> BTech::modifierStringChange = {
	{BTech::Modifier::Base,             BTech::Strings::ModifierBase},
	{BTech::Modifier::Range,            BTech::Strings::ModifierRange},
	{BTech::Modifier::Direction,        BTech::Strings::ModifierDirection},
	{BTech::Modifier::Terrain,          BTech::Strings::ModifierTerrain},
	{BTech::Modifier::Height,           BTech::Strings::ModifierHeight},
	{BTech::Modifier::AttackerMovement, BTech::Strings::ModifierAttackerMovement},
	{BTech::Modifier::TargetMovement,   BTech::Strings::ModifierTargetMovement},
};

const QHash <BTech::Range, int> BTech::maximalMaxRange = {
	{BTech::Range::Short,  BTech::MAXIMAL_MAX_SHORT_RANGE},
	{BTech::Range::Medium, BTech::MAXIMAL_MAX_MEDIUM_RANGE},
	{BTech::Range::Long,   BTech::MAXIMAL_MAX_LONG_RANGE},
};

const QHash <BTech::Range, int> BTech::minimalMaxRange = {
	{BTech::Range::Short,  BTech::MINIMAL_MAX_SHORT_RANGE},
	{BTech::Range::Medium, BTech::MINIMAL_MAX_MEDIUM_RANGE},
	{BTech::Range::Long,   BTech::MINIMAL_MAX_LONG_RANGE},
};

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
