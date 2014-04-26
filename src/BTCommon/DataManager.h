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

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "BTCommon/MechBase.h"
#include "BTCommon/WeaponBase.h"

class DataManager
{

public:
	static DataManager & getInstance();

	static bool loadFromFile(const QString &fileName = BTech::Paths::DATA_PATH);
	static bool saveToFile(const QString &fileName = BTech::Paths::DATA_PATH);

private:
	explicit DataManager();
	DataManager(const DataManager &) = delete;
	DataManager(DataManager &&) = delete;
	~DataManager();

	void operator = (const DataManager &) = delete;
	void operator = (DataManager &&) = delete;

	friend QDataStream & operator << (QDataStream &out, const DataManager &dataManager);
	friend QDataStream & operator >> (QDataStream &in, DataManager &dataManager);
	friend bool ::saveToFile<DataManager>(const DataManager &obj, const QString &fileName);

	bool changed;
};

#endif // DATA_MANAGER_H