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

#ifndef MECH_BASE_H
#define MECH_BASE_H

#include <QtWidgets>
#include "BTCommon/Action.h"
#include "BTCommon/MechPartBase.h"
#include "BTCommon/Utils.h"

class MechModel;
class EditMechPartsWidget;

class MechBase
{

public:
	MechBase();
	MechBase(UID uid);
	MechBase(const QString &name, UID uid);
	MechBase(const QString &name,
	         int tonnage,
	         int armorValue,
	         int maxMPS,
	         int maxJumpPoints,
	         int baseFireRange,
	         int heatSinksNumber,
	         const QList <MechPartBase *> &parts,
	         int contactRangeDamage,
	         int shortRangeDamage,
	         int mediumRangeDamage,
	         int longRangeDamage);
	MechBase(const MechBase &mech);
	~MechBase();

	void load(const MechBase &mech);
	void load(const QString &name,
	          int tonnage,
	          int armorValue,
	          int maxMPS,
	          int maxJumpPoints,
	          int baseFireRange,
	          int heatSinksNumber,
	          const QList <MechPartBase *> &parts,
	          int contactRangeDamage,
	          int shortRangeDamage,
	          int mediumRangeDamage,
	          int longRangeDamage);

	explicit operator QString() const;

	bool operator == (const MechBase &mech) const;

	void setName(const QString &name);
	QString getName() const;

	void setTonnage(int tonnage);
	int getTonnage() const;
	void setArmorValue(int value);
	int getArmorValue() const;
	void setMaxMovePoints(int maxMovePoints);
	int getMaxMovePoints() const;
	void setMaxJumpPoints(int maxJumpPoints);
	int getMaxJumpPoints() const;
	void setBaseFireRange(int range);
	int getBaseFireRange() const;
	void setHeatSinksNumber(int number);
	int getHeatSinksNumber() const;

	BTech::Range distanceToRange(int distance) const;
	void setRangeDamage(BTech::Range range, int damage);
	int getRangeDamage(BTech::Range range) const;
	int getDamageValue(int distance) const;

	MechPartBase * getMechPart(BTech::MechPartType type, BTech::MechPartSide side);
	MechPartBase * getMechPart(QPair <BTech::MechPartType, BTech::MechPartSide> typeAndSide);
	const QList <MechPartBase *> & getMechParts() const;

	friend QDataStream & operator << (QDataStream &out, const MechBase &mech);
	friend QDataStream & operator >> (QDataStream &in, MechBase &mech);

	void setUid(UID uid);
	UID getUid() const;

private:
	void clearData();

	UID uid;
	QString name;
	int tonnage;
	int armorValue;
	int maxMovePoints;
	int maxJumpPoints;
	int baseFireRange;
	int heatSinksNumber;
	QList <MechPartBase *> parts;
	QHash <BTech::Range, int> rangeDamage;

	static const int DEFAULT_TONNAGE             = 10;
	static const int DEFAULT_ARMOR_VALUE         = 1;
	static const int DEFAULT_MAX_MOVE_POINTS     = 1;
	static const int DEFAULT_MAX_JUMP_POINTS     = 1;
	static const int DEFAULT_BASE_FIRE_RANGE     = 1;
	static const int DEFAULT_HEAT_SINKS_NUMBER   = 1;
	static const int DEFAULT_RANGE_DAMAGE        = 1;
	static const int DEFAULT_OUT_OF_RANGE_DAMAGE = 0;
};

/**
 * \class MechModel
 * Data model for storing \ref Mech "Mechs"
 */
class MechModel : public QAbstractItemModel
{
Q_OBJECT;

public:
	enum class Level : quint8 {
		Root,
		Mech,
		MechPart,
		Equipment,
	};

private:
	class MechModelNode {
	public:
		MechModelNode(MechModelNode *parent, void *data, Level level);
		MechModelNode(MechModelNode *parent, const void *constData, Level level);
		~MechModelNode();

		void addChild(int idx, MechModelNode *child);
		void appendChild(MechModelNode *child);
		int childrenCount() const;
		void deleteChild(int idx);
		MechModelNode * getChild(int idx) const;
		MechModelNode * getParent() const;
		int row() const;

		Level getLevel() const;
		MechBase * getMechBase() const;
		MechPartBase * getMechPartBase() const;
		const WeaponBase * getWeaponBase() const;

	private:
		MechModelNode *parent;
		QList <MechModelNode *> children;
		void *data;
		const void *constData;
		Level level;
	};

	MechModelNode *treeRoot;

public:
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	bool insertRows(int row, int count, const QModelIndex &parent);
	QModelIndex parent(const QModelIndex &index) const;
	bool removeRows(int row, int count, const QModelIndex &parent);
	int rowCount(const QModelIndex &parent) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	bool isChanged() const;
	void setChanged(bool changed);

	static MechModel & getInstance();
	static bool loadFromFile(const QString &fileName = BTech::Paths::MECHS_PATH);
	static bool saveToFile(const QString &fileName = BTech::Paths::MECHS_PATH);

	static void addNewMech();
	static bool empty();
	static const MechBase * getRow(int row);
	static const MechBase * getMech(UID uid);
	static const MechBase * getMech(const QString &name);
	static const QList <MechBase *> & getMechs();
	static bool hasMech(const QString &name);
	static void removeMech(UID uid);

	static void addWeaponToMechPart(const QModelIndex &mechPartIndex, const WeaponBase *weapon);
	static void removeWeaponFromMechPart(const QModelIndex &mechPartIndex, int row);
	static void moveWeaponInMechUp(const QModelIndex &mechPartIndex, int weaponPosition);
	static void moveWeaponInMechDown(const QModelIndex &mechPartIndex, int weaponPosition);

	static Level getIndexType(const QModelIndex &index);

	struct Mech {
		static const int Name               = 0;
		static const int Tonnage            = 1;
		static const int ArmorValue         = 2;
		static const int MaxMovePoints      = 3;
		static const int MaxJumpPoints      = 4;
		static const int BaseFireRange      = 5;
		static const int HeatSinksNumber    = 6;
		static const int ContactRangeDamage = 7;
		static const int ShortRangeDamage   = 8;
		static const int MediumRangeDamage  = 9;
		static const int LongRangeDamage    = 10;
		static const int ColumnCount        = 11;
	};

	struct MechPart {
		static const int Name             = 0;
		static const int MaxArmorValue    = 1;
		static const int MaxInternalValue = 2;
		static const int ColumnCount      = 3;
	};

	static const int Head        = 0;
	static const int LeftTorso   = 1;
	static const int RightTorso  = 2;
	static const int CenterTorso = 3;
	static const int LeftArm     = 4;
	static const int RightArm    = 5;
	static const int LeftLeg     = 6;
	static const int RightLeg    = 7;

	friend QDataStream & operator << (QDataStream &out, const MechModel &mModel);
	friend QDataStream & operator >> (QDataStream &in, MechModel &mModel);
	friend bool ::saveToFile<MechModel>(const MechModel &obj, const QString &fileName);

private:
	explicit MechModel(QObject *parent = nullptr);
	MechModel(const MechModel &) = delete;
	MechModel(MechModel &&) = delete;
	void operator = (const MechModel &) = delete;
	void operator = (MechModel &&) = delete;
	~MechModel();

	void clearModel();
	void initModel();

	void addMech(int row, MechBase *mech);
	void removeMechFromRow(int row);

	Level getLevel(const QModelIndex &index) const;
	MechModelNode * getNode(const QModelIndex &index) const;

	bool changed;
	UID nextUid;

	static QList <MechBase *> mechList;
	static QHash <UID, MechBase *> uidToMech;
};

inline uint qHash(MechModel::Level level)
{
	return qHash(toUnderlying(level));
}

#endif // MECH_BASE_H
