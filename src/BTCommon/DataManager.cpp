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
