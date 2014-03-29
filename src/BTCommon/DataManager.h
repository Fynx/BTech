#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "BTCommon/MechBase.h"
#include "BTCommon/WeaponBase.h"

class DataManager
{

public:
	static DataManager & getInstance();

	static bool loadFromFile(const QString &fileName);
	static bool saveToFile(const QString &fileName);

private:
	explicit DataManager();
	DataManager(const DataManager &) = delete;
	DataManager(DataManager &&) = delete;
	~DataManager();

	void operator = (const DataManager &) = delete;
	void operator = (DataManager &&) = delete;

	friend QDataStream & operator << (QDataStream &out, const DataManager &dataManager);
	friend QDataStream & operator >> (QDataStream &in, DataManager &dataManager);
	friend bool ::saveToFile<DataManager>(DataManager &obj, const QString &fileName);

	bool changed;
};

#endif // DATA_MANAGER_H