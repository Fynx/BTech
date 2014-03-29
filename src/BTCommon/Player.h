#ifndef PLAYER_H
#define PLAYER_H

#include <QtWidgets>
#include "BTCommon/CommonStrings.h"
#include "BTCommon/MechEntity.h"
#include "BTCommon/Utils.h"

/**
 * \class Player
 * Contains a description of a given player, including name, statistics, optionally AI description (if computer player).
 */
class Player : public QObject
{
Q_OBJECT;

public:
	Player(const QString name = BTech::Strings::PlayerComputer, const QString description = QString());
	~Player();

	QList <MechEntity *> getMechs() const;
	void setName(const QString &name);
	QString getName() const;
	void setDescription(const QString &description);
	QString getDescription() const;
	void setColor(const QColor &color);
	QColor getColor() const;

	void addMech(MechEntity *mech);
	void removeMech(const MechEntity *mech);
	bool hasMech(const MechEntity *mech) const;

	friend QDataStream & operator << (QDataStream &out, const Player &player);
	friend QDataStream & operator >> (QDataStream &in, Player &player);

private:
	QList <MechEntity *> mechs;

	QString name;
	QString description;
	QColor color;
};

#endif // PLAYER_H
