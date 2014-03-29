#include "BTCommon/EnumHashFunctions.h"
#include "BTCommon/MechWarrior.h"

/**
 * \enum BTech::Skill
 */

QDataStream & BTech::operator << (QDataStream &out, const BTech::Skill &skill)
{
	out << toUnderlying(skill);
	return out;
}

QDataStream & BTech::operator >> (QDataStream &in, BTech::Skill &skill)
{
	in >> toUnderlyingRef(skill);
	return in;
}

/**
 * \class MechWarrior
 */

MechWarrior::MechWarrior()
{
	skills[BTech::Skill::Gunnery] = 4;	// works for <= Advanced.
}

int MechWarrior::getSkill(const BTech::Skill &skill)
{
	return skills[skill];
}

QDataStream & operator << (QDataStream &out, const MechWarrior &mechWarrior)
{
	out << mechWarrior.skills;
	return out;
}

QDataStream & operator >> (QDataStream &in, MechWarrior &mechWarrior)
{
	in >> mechWarrior.skills;
	return in;
}
