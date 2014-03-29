#ifndef EFFECT_H
#define EFFECT_H

#include <QtWidgets>
#include "BTCommon/Utils.h"

class Effect
{
public:
	static const int FOREVER;
	static const int UNDEFINED_DURATION;

	static QString durationToString(int duration);

	Effect(BTech::EffectType type = BTech::EffectType::None,
	       BTech::EffectSource source = BTech::EffectSource::NoSource,
	       int duration = 0,
	       int value = 0);

	bool operator == (const Effect &effect);

	void setType(BTech::EffectType type);
	BTech::EffectType getType() const;
	void setSource(BTech::EffectSource source);
	BTech::EffectSource getSource() const;
	void setDuration(int duration);
	int getDuration() const;
	void setValue(int value);
	int getValue() const;
	bool isActive() const;
	bool isPermanent() const;
	void triggerTurnRecovery();

	explicit operator QString() const;

	friend QDataStream & operator << (QDataStream &out, const Effect &effect);
	friend QDataStream & operator >> (QDataStream &in, Effect &effect);

private:
	BTech::EffectType type;
	BTech::EffectSource source;
	int duration;
	int value;
};

class EffectProne
{
public:
	void addEffect(const Effect &effect);
	void setEffect(const Effect &effect);
	Effect getEffect(BTech::EffectType type, BTech::EffectSource source = BTech::EffectSource::NoSource) const;
	bool hasEffect(BTech::EffectType type, BTech::EffectSource source = BTech::EffectSource::NoSource) const;
	void removeEffect(BTech::EffectType type, BTech::EffectSource source = BTech::EffectSource::NoSource);
	void removeEffects(BTech::EffectSource source);
	QList <Effect> getEffects() const;

	friend QDataStream & operator << (QDataStream &out, const EffectProne &obj);
	friend QDataStream & operator >> (QDataStream &in, EffectProne &obj);

protected:
	QList <Effect> & editEffects();

private:
	QList <Effect> effects;
};

#endif // EFFECT_H
