#include "BTCommon/EnumHashFunctions.h"

#include "BTCommon/Rules.h"

BTech::GameVersion Rules::version;
QString Rules::description;
QList <BTech::GamePhase> Rules::allowedPhases;

void Rules::setVersion(const BTech::GameVersion newVersion)
{
	version = newVersion;
}

BTech::GameVersion Rules::getVersion()
{
	return version;
}

void Rules::setDescription(const QString &newDescription)
{
	description = newDescription;
}

QString Rules::getDescription()
{
	return description;
}

QList <BTech::GamePhase> Rules::getAllowedPhases()
{
	return BTech::versionToPhaseList[getVersion()];
}