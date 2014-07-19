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

#include "BTCommon/EnumHashFunctions.h"

#include "BTCommon/Bases/MechBase.h"
#include "BTCommon/Bases/MechPartBase.h"
#include "BTCommon/Bases/WeaponBase.h"
#include "BTCommon/Models/MechModel.h"
#include "BTCommon/Models/WeaponModel.h"
#include "BTCommon/Utils.h"

/**
 * \class MechModel::MechModelNode
 */

QList <MechBase *> MechModel::mechList;
QHash <UID, MechBase *> MechModel::uidToMech;

MechModel::MechModelNode::MechModelNode(MechModelNode *parent, void *data, Level level)
	: parent(parent), data(data), level(level)
{
}

MechModel::MechModelNode::MechModelNode(MechModelNode *parent, const void *constData, Level level)
	: parent(parent), constData(constData), level(level)
{
}

MechModel::MechModelNode::~MechModelNode()
{
	qDeleteAll(children);
}

void MechModel::MechModelNode::addChild(int idx, MechModelNode *child)
{
	children.insert(idx, child);
}

void MechModel::MechModelNode::appendChild(MechModelNode *child)
{
	children.append(child);
}

int MechModel::MechModelNode::childrenCount() const
{
	return children.count();
}

void MechModel::MechModelNode::deleteChild(int idx)
{
	delete children.takeAt(idx);
}

int MechModel::MechModelNode::row() const
{
	return parent->children.indexOf(const_cast<MechModelNode *>(this));
}

MechModel::MechModelNode * MechModel::MechModelNode::getChild(int idx) const
{
	return children[idx];
}

MechModel::MechModelNode * MechModel::MechModelNode::getParent() const
{
	return parent;
}

MechModel::Level MechModel::MechModelNode::getLevel() const
{
	return level;
}

MechBase * MechModel::MechModelNode::getMechBase() const
{
	return reinterpret_cast<MechBase *>(data);
}

MechPartBase * MechModel::MechModelNode::getMechPartBase() const
{
	return reinterpret_cast<MechPartBase *>(data);
}

const WeaponBase * MechModel::MechModelNode::getWeaponBase() const
{
	return reinterpret_cast<const WeaponBase *>(constData);
}

/**
 * \class MechModel
 */

int MechModel::columnCount(const QModelIndex &parent) const
{
	switch (getLevel(parent)) {
		case Level::Root:     return Mech::ColumnCount;
		case Level::Mech:     return MechPart::ColumnCount;
		case Level::MechPart: return WeaponModel::ColumnCount;
		default:              return 0;
	}
}

QVariant MechModel::data(const QModelIndex &index, int role) const
{
	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	MechModelNode *modelNode = getNode(index);
	switch (getLevel(index)) {
		case Level::Mech: {
			const MechBase *mech = modelNode->getMechBase();
			switch (index.column()) {
				case Mech::Name:               return mech->getName();
				case Mech::Tonnage:            return mech->getTonnage();
				case Mech::ArmorValue:         return mech->getArmorValue();
				case Mech::MaxMovePoints:      return mech->getMaxMovePoints();
				case Mech::MaxJumpPoints:      return mech->getMaxJumpPoints();
				case Mech::BaseFireRange:      return mech->getBaseFireRange();
				case Mech::HeatSinksNumber:    return mech->getHeatSinksNumber();
				case Mech::ContactRangeDamage: return mech->getRangeDamage(BTech::Range::Contact);
				case Mech::ShortRangeDamage:   return mech->getRangeDamage(BTech::Range::Short);
				case Mech::MediumRangeDamage:  return mech->getRangeDamage(BTech::Range::Medium);
				case Mech::LongRangeDamage:    return mech->getRangeDamage(BTech::Range::Long);
			}
			break;
		}

		case Level::MechPart: {
			const MechPartBase *mechPart = modelNode->getMechPartBase();
			switch (index.column()) {
				case MechPart::Name:             return QString(*mechPart);
				case MechPart::MaxArmorValue:    return mechPart->getMaxArmorValue();
				case MechPart::MaxInternalValue: return mechPart->getMaxInternalValue();
			}
			break;
		}

		case Level::Equipment: {
			const WeaponBase *weapon = modelNode->getWeaponBase();
			switch (index.column()) {
				case WeaponModel::Name:               return weapon->getName();
				case WeaponModel::Type:               return toUnderlying(weapon->getWeaponType());
				case WeaponModel::MinimalRange:       return weapon->getMinRange();
				case WeaponModel::MaximalShortRange:  return weapon->getMaxRange(BTech::Range::Short);
				case WeaponModel::MaximalMediumRange: return weapon->getMaxRange(BTech::Range::Medium);
				case WeaponModel::MaximalLongRange:   return weapon->getMaxRange(BTech::Range::Long);
				case WeaponModel::Heat:               return weapon->getHeat();
				case WeaponModel::Damage:             return weapon->getDamage();
				case WeaponModel::Tonnage:            return weapon->getTonnage();
				case WeaponModel::CriticalSpaces:     return weapon->getCriticalSpaces();
				case WeaponModel::AmmoPerTon:         return weapon->getAmmoPerTon();
				case WeaponModel::MissilesPerShot:    return weapon->getMissilesPerShot();
			}
			break;
		}

		default:
			return QVariant();
	}

	return QVariant();
}

Qt::ItemFlags MechModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	Qt::ItemFlags defaultFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (getIndexType(index) == Level::Equipment)
		return defaultFlags | Qt::ItemNeverHasChildren;

	return defaultFlags;
}

QModelIndex MechModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	MechModelNode *node = getNode(parent);
	return createIndex(row, column, node->getChild(row));
}

bool MechModel::insertRows(int row, int count, const QModelIndex &parent)
{
	if (!parent.isValid()) {
		beginInsertRows(parent, row, row + count - 1);
		for (int i = 0; i < count; ++i) {
			int nameSuffix = 0;
			QString newName;
			do {
				++nameSuffix;
				newName = QString("%1 %2").arg(BTech::Strings::DefaultNewMechName).arg(nameSuffix);
			} while (hasMech(newName));

			MechBase *newMech = new MechBase(newName, nextUid++);
			addMech(row + i, newMech);
		}
		endInsertRows();

		return true;
	}

	return false;
}

QModelIndex MechModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();
	MechModelNode *node = getNode(index);

	if (node->getParent() == treeRoot)
		return QModelIndex();
	return createIndex(node->getParent()->row(), 0, node->getParent());
}

bool MechModel::removeRows(int row, int count, const QModelIndex &parent)
{
	if (!parent.isValid()) {
		beginRemoveRows(parent, row, row + count - 1);
		for (int i = 0; i < count; ++i)
			removeMechFromRow(row + i);
		endRemoveRows();

		return true;
	}

	return false;
}

int MechModel::rowCount(const QModelIndex &parent) const
{
	return getNode(parent)->childrenCount();
}

bool MechModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;

	MechModelNode *node = static_cast<MechModelNode *>(index.internalPointer());

	switch (getLevel(index)) {
		case Level::Mech: {
			MechBase *mech = node->getMechBase();
			switch (index.column()) {
				case Mech::Name:               mech->setName(value.toString()); break;
				case Mech::Tonnage:            mech->setTonnage(value.toInt()); break;
				case Mech::ArmorValue:         mech->setArmorValue(value.toInt()); break;
				case Mech::MaxMovePoints:      mech->setMaxMovePoints(value.toInt()); break;
				case Mech::MaxJumpPoints:      mech->setMaxJumpPoints(value.toInt()); break;
				case Mech::BaseFireRange:      mech->setBaseFireRange(value.toInt()); break;
				case Mech::HeatSinksNumber:    mech->setHeatSinksNumber(value.toInt()); break;
				case Mech::ContactRangeDamage: mech->setRangeDamage(BTech::Range::Contact, value.toInt()); break;
				case Mech::ShortRangeDamage:   mech->setRangeDamage(BTech::Range::Short, value.toInt()); break;
				case Mech::MediumRangeDamage:  mech->setRangeDamage(BTech::Range::Medium, value.toInt()); break;
				case Mech::LongRangeDamage:    mech->setRangeDamage(BTech::Range::Long, value.toInt()); break;
				default:                       return false;
			}
			break;
		}

		case Level::MechPart: {
			MechPartBase *mechPart = static_cast<MechModelNode *>(index.internalPointer())->getMechPartBase();
			switch (index.column()) {
				case MechPart::MaxArmorValue:    mechPart->setMaxArmorValue(value.toInt()); break;
				case MechPart::MaxInternalValue: mechPart->setMaxInternalValue(value.toInt()); break;
				default:                         return false;
			}
			break;
		}

		default:
			return false;
	}

	emit dataChanged(index, index);
	return true;
}

bool MechModel::isChanged() const
{
	return changed;
}

void MechModel::setChanged(bool changed)
{
	this->changed = changed;
}

MechModel & MechModel::getInstance()
{
	static MechModel *model = new MechModel;
	return *model;
}

bool MechModel::loadFromFile(const QString &fileName)
{
	return ::loadFromFile(getInstance(), BTech::resolvePath(fileName));
}

bool MechModel::saveToFile(const QString &fileName)
{
	return ::saveToFile(getInstance(), BTech::resolvePath(fileName));
}

void MechModel::addNewMech()
{
	getInstance().insertRows(mechList.size(), 1, QModelIndex());
}

bool MechModel::empty()
{
	return mechList.isEmpty();
}

const MechBase * MechModel::getRow(int row)
{
	return mechList[row];
}

const MechBase * MechModel::getMech(UID uid)
{
	return uidToMech.value(uid, nullptr);
}

const MechBase * MechModel::getMech(const QString &name)
{
	const MechBase *result = nullptr;
	for (MechBase *cand : mechList)
		if (cand->getName() == name)
			result = cand;
	if (result == nullptr)
		qWarning() << "Warning: MechBase" << name << "not found.";
	return result;
}

const QList <MechBase *> & MechModel::getMechs()
{
	return mechList;
}

bool MechModel::hasMech(const QString &name)
{
	for (MechBase *mech : mechList)
		if (mech->getName() == name)
			return true;
	return false;
}

void MechModel::removeMech(UID uid)
{
	for (int i = 0; i < mechList.count(); ++i) {
		if (mechList[i]->getUid() == uid) {
			getInstance().removeRows(i, 1, QModelIndex());
			return;
		}
	}
}

void MechModel::addWeaponToMechPart(const QModelIndex &mechPartIndex, const WeaponBase *weapon)
{
	MechModel &mModel = getInstance();
	Q_ASSERT(mModel.getLevel(mechPartIndex) == Level::MechPart);

	MechModelNode *node = mModel.getNode(mechPartIndex);
	MechModelNode *weaponNode = new MechModelNode(node, weapon, Level::Equipment);
	MechPartBase *mechPart = node->getMechPartBase();

	mModel.beginInsertRows(mechPartIndex, mechPart->getWeapons().count(), mechPart->getWeapons().count());
	mechPart->addWeapon(weapon);
	node->appendChild(weaponNode);
	mModel.endInsertRows();
}

void MechModel::removeWeaponFromMechPart(const QModelIndex &mechPartIndex, int row)
{
	MechModel &mModel = getInstance();
	Q_ASSERT(mModel.getLevel(mechPartIndex) == Level::MechPart);

	MechModelNode *node = mModel.getNode(mechPartIndex);
	MechPartBase *mechPart = node->getMechPartBase();

	mModel.beginRemoveRows(mechPartIndex, row, row);
	mechPart->removeWeapon(row);
	node->deleteChild(row);
	mModel.endRemoveRows();
}

void MechModel::moveWeaponInMechUp(const QModelIndex &mechPartIndex, int weaponPosition)
{
	//TODO check & test
	//MechModel &mModel = getInstance();
	Q_ASSERT(getInstance().getLevel(mechPartIndex) == Level::MechPart);

// 	MechModelNode *node = mModel.getNode(mechPartIndex);
// 	MechPartBase *mechPart = node->getMechPartBase();

	if (weaponPosition > 0)
		getInstance().moveRow(mechPartIndex, weaponPosition, mechPartIndex, weaponPosition - 1);
}

void MechModel::moveWeaponInMechDown(const QModelIndex &mechPartIndex, int weaponPosition)
{
	//TODO check & test
	MechModel &mModel = getInstance();
	Q_ASSERT(mModel.getLevel(mechPartIndex) == Level::MechPart);

	MechModelNode *node = mModel.getNode(mechPartIndex);
// 	MechPartBase *mechPart = node->getMechPartBase();

	if (weaponPosition < node->childrenCount() - 1)
		getInstance().moveRow(mechPartIndex, weaponPosition, mechPartIndex, weaponPosition + 1);
}

MechModel::Level MechModel::getIndexType(const QModelIndex &index)
{
	return getInstance().getLevel(index);
}

QDataStream & operator << (QDataStream &out, const MechModel &mModel)
{
	out << mModel.nextUid << static_cast<UID>(mModel.mechList.count());
	for (const MechBase *mech : mModel.mechList)
		out << *mech;
	return out;
}

QDataStream & operator >> (QDataStream &in, MechModel &mModel)
{
	mModel.beginResetModel();
	mModel.clearModel();
	mModel.initModel();

	UID count;
	in >> mModel.nextUid >> count;
	for (UID i = 0; i < count; ++i) {
		MechBase *mech = new MechBase;
		in >> *mech;
		mModel.addMech(i, mech);
	}
	mModel.endResetModel();

	return in;
}

MechModel::MechModel(QObject *parent)
	: QAbstractItemModel(parent), changed(false), nextUid(MinUid)
{
	auto modelChanged = [this]{
		this->changed = true;
	};

	connect(this, &QAbstractTableModel::dataChanged,  modelChanged);
	connect(this, &QAbstractTableModel::rowsInserted, modelChanged);
	connect(this, &QAbstractTableModel::rowsMoved,    modelChanged);
	connect(this, &QAbstractTableModel::rowsRemoved,  modelChanged);

	initModel();
}

MechModel::~MechModel()
{
	clearModel();
}

void MechModel::clearModel()
{
	qDeleteAll(mechList);
	mechList.clear();
	delete treeRoot;
}

void MechModel::initModel()
{
	treeRoot = new MechModelNode(nullptr, static_cast<const void *>(nullptr), Level::Root);
	uidToMech.clear();
}

void MechModel::addMech(int row, MechBase *mech)
{
	mechList.insert(row, mech);
	uidToMech[mech->getUid()] = mech;

	MechModelNode *mechNode = new MechModelNode(treeRoot, mech, Level::Mech);
	treeRoot->addChild(row, mechNode);

	for (MechPartBase *mechPartBase : mech->getMechParts()) {
		MechModelNode *mechPartNode = new MechModelNode(mechNode, mechPartBase, Level::MechPart);
		mechNode->appendChild(mechPartNode);

		for (const WeaponBase *weaponBase : mechPartBase->getWeapons()) {
			MechModelNode *weaponNode = new MechModelNode(mechPartNode, weaponBase, Level::Equipment);
			mechPartNode->appendChild(weaponNode);
		}
	}
}

void MechModel::removeMechFromRow(int row)
{
	uidToMech.remove(mechList[row]->getUid());
	delete mechList.takeAt(row);
	treeRoot->deleteChild(row);
}

MechModel::Level MechModel::getLevel(const QModelIndex &index) const
{
	if (!index.isValid())
		return Level::Root;
	return static_cast<MechModelNode *>(index.internalPointer())->getLevel();
}

MechModel::MechModelNode * MechModel::getNode(const QModelIndex &index) const
{
	if (!index.isValid())
		return treeRoot;
	return static_cast<MechModelNode *>(index.internalPointer());
}