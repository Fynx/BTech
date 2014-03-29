#ifndef RULES_H
#define RULES_H

#include <QtWidgets>
#include "BTCommon/Utils.h"

/**
 * \class Rules
 * Contains description of rules of the game. This includes name of the version, phases that are allowed during the game, allowed actions and much more.
 */
class Rules : public QObject
{
Q_OBJECT;

public:
	static void initRules();

	static void setVersion(const BTech::GameVersion version);
	static BTech::GameVersion getVersion();
	static void setDescription(const QString &description);
	static QString getDescription();
	static QList <BTech::GamePhase> getAllowedPhases();

private:
	Rules() = delete;
	Rules(const Rules &) = delete;
	Rules(Rules &&) = delete;
	void operator = (const Rules &) = delete;
	void operator = (Rules &&) = delete;

	static BTech::GameVersion version;
	static QString description;
	static QList <BTech::GamePhase> allowedPhases;
};

#endif // RULES_H
