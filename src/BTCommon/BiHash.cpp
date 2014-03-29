#include "BTCommon/EnumHashFunctions.h"

#include "BTCommon/BiHash.h"
#include "BTCommon/Position.h"
#include "BTCommon/Utils.h"

template <typename T, typename U>
BiHash <T, U>::BiHash(std::initializer_list <std::pair <T, U> > initList)
{
	for (const std::pair <T, U> &p : initList)
		insert(p.first, p.second);
}

template <typename T, typename U>
U & BiHash <T, U>::operator [] (const T &arg)
{
	return left[arg];
}

template <typename T, typename U>
U BiHash <T, U>::operator [] (const T &arg) const
{
	return left[arg];
}

template <typename T, typename U>
T & BiHash <T, U>::operator [] (const U &arg)
{
	return right[arg];
}

template <typename T, typename U>
T BiHash <T, U>::operator [] (const U &arg) const
{
	return right[arg];
}

template <typename T, typename U>
void BiHash <T, U>::insert(const T &lhs, const U &rhs)
{
	left.insert(lhs, rhs);
	right.insert(rhs, lhs);
}

template <typename T, typename U>
void BiHash <T, U>::insert(const U &lhs, const T &rhs)
{
	right.insert(lhs, rhs);
	left.insert(rhs, lhs);
}

template <typename T, typename U>
int BiHash <T, U>::size() const
{
	return left.size();
}

template class BiHash <BTech::EffectType,   QString>;
template class BiHash <BTech::EntityType,   QString>;
template class BiHash <BTech::GamePhase,    QString>;
template class BiHash <BTech::GameVersion,  QString>;
template class BiHash <BTech::MechPartType, QString>;
template class BiHash <BTech::MechPartSide, QString>;
template class BiHash <BTech::Range,        QString>;
template class BiHash <BTech::Terrain,      QString>;
template class BiHash <BTech::WeaponType,   QString>;
template class BiHash <Direction,           QString>;
