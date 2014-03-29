#include "BTCommon/Player.h"

/* constructor */
Player::Player(const QString name, const QString description)
	: name(name), description(description)
{}

/* destructor */
Player::~Player()
{
	qDeleteAll(mechs);
}

QList <MechEntity *> Player::getMechs() const
{
	return mechs;
}

void Player::setName(const QString &name)
{
	this->name = name;
}

QString Player::getName() const
{
	return name;
}

void Player::setDescription(const QString &description)
{
	this->description = description;
}

QString Player::getDescription() const
{
	return description;
}

void Player::setColor(const QColor &color)
{
	this->color = color;
}

QColor Player::getColor() const
{
	return color;
}

void Player::addMech(MechEntity *mech)
{
	mechs << mech;
	mech->setOwnerName(getName());
}

void Player::removeMech(const MechEntity *mech)
{
	for (int i = 0; i < mechs.size(); ++i)
		if (mechs[i] == mech)
			mechs[i] = mechs.back();
	mechs.removeLast();
	delete mech;
}

bool Player::hasMech(const MechEntity *mech) const
{
	for (MechEntity *currentMech : mechs)
		if (currentMech == mech)
			return true;
	return false;
}

QDataStream & operator << (QDataStream &out, const Player &player)
{
// 	qDebug() << "saving player" << player.name << "\n";
	out << player.name << player.description;
	out << player.mechs.size();
	for (MechEntity *mech : player.mechs)
		out << *mech;

// 	qDebug() << "player saved\n";
	return out;
}

QDataStream & operator >> (QDataStream &in, Player &player)
{
	in >> player.name >> player.description;
	qDebug() << "loading" << player.name;
	quint32 mechsSize;
	in >> mechsSize;
	for (quint32 i = 0; i < mechsSize; ++i) {
		MechEntity *mech = new MechEntity;
		in >> *mech;
		player.addMech(mech);
	}

	qDebug() << "player loaded";
	return in;
}