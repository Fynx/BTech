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

#include "BTCommon/DataManager.h"
#include "BTCommon/FileIO.h"
#include "BTCommon/Paths.h"

DataManager & DataManager::getInstance()
{
	static DataManager *dataManager = new DataManager();
	return *dataManager;
}

bool DataManager::loadFromFile(const QString &fileName = BTech::Paths::DATA_PATH)
{
	return ::loadFromFile(getInstance(), fileName);
}

bool DataManager::saveToFile(const QString &fileName = BTech::Paths::DATA_PATH)
{
	WeaponModel::getInstance().setChanged(false);
	MechModel::getInstance().setChanged(false);
	return ::saveToFile(getInstance(), fileName);
}

DataManager::DataManager()
	: changed(false)
{}

DataManager::~DataManager()
{}

QDataStream & operator << (QDataStream &out, const DataManager &dataManager)
{
	out << WeaponModel::getInstance() << MechModel::getInstance();
	return out;
}

QDataStream & operator >> (QDataStream &in, DataManager &dataManager)
{
	in >> WeaponModel::getInstance() >> MechModel::getInstance();
	return in;
}
