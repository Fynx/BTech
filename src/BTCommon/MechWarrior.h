#ifndef MECH_WARRIOR_H
#define MECH_WARRIOR_H

#include <QtWidgets>

namespace BTech {
	enum class Skill : quint8 {
		NoSkill,
		Gunnery,
	};

	QDataStream & operator << (QDataStream &out, const BTech::Skill &skill);
	QDataStream & operator >> (QDataStream &in, BTech::Skill &skill);
}

class MechWarrior
{

public:
	MechWarrior();

	int getSkill(const BTech::Skill &skill);

	friend QDataStream & operator << (QDataStream &out, const MechWarrior &mechWarrior);
	friend QDataStream & operator >> (QDataStream &in, MechWarrior &mechWarrior);
	friend QDebug & operator << (QDebug out, const MechWarrior &mechWarrior);

private:
	QMap <BTech::Skill, int> skills;
};

#endif // MECH_WARRIOR_H
