#include "BTCommon/Effect.h"
#include "BTCommon/EnumSerialization.h"

/**
 * \class Effect
 */

const int Effect::FOREVER = 100000;
const int Effect::UNDEFINED_DURATION = 270836;	// random magic

QString Effect::durationToString(int duration)
{
	if (duration >= FOREVER)
		return BTech::Strings::DurationPermanent;
	else if (duration == UNDEFINED_DURATION)
		return BTech::Strings::DurationPermanent;
	else
		return QString::number(duration);
}

Effect::Effect(BTech::EffectType type,
               BTech::EffectSource source,
               int duration,
               int value)
	: type(type), source(source), duration(duration), value(value)
{}

bool Effect::operator == (const Effect &effect)
{
	return getType() == effect.getType()
	    && getSource() == effect.getSource()
	    && getDuration() == effect.getDuration()
	    && getValue() == effect.getValue();
}

void Effect::setType(BTech::EffectType type)
{
	this->type = type;
}

BTech::EffectType Effect::getType() const
{
	return type;
}

void Effect::setSource(BTech::EffectSource source)
{
	this->source = source;
}

BTech::EffectSource Effect::getSource() const
{
	return source;
}

void Effect::setDuration(int duration)
{
	this->duration = duration;
}

int Effect::getDuration() const
{
	return duration;
}

void Effect::setValue(int value)
{
	this->value = value;
}

int Effect::getValue() const
{
	return value;
}

inline bool Effect::isActive() const
{
	return type != BTech::EffectType::None && duration > 0;
}

bool Effect::isPermanent() const
{
	return duration >= FOREVER;
}

void Effect::triggerTurnRecovery()
{
	if (!isPermanent() && isActive())
		--duration;
}

Effect::operator QString() const
{
	if (getType() == BTech::EffectType::None)
		return QString();
	return BTech::effectTypeStringChange[getType()] + " " + durationToString(getDuration());
}

QDataStream & operator << (QDataStream &out, const Effect &effect)
{
	out << effect.type
	    << effect.source
	    << effect.duration
	    << effect.value;
	return out;
}

QDataStream & operator >> (QDataStream &in, Effect &effect)
{
	in >> effect.type
	   >> effect.source
	   >> effect.duration
	   >> effect.value;
	return in;
}

/**
 * \class EffectProne
 */

void EffectProne::addEffect(const Effect &effect)
{
	return setEffect(
		Effect(effect.getType(),
		       effect.getSource(),
		       effect.getDuration(),
		       effect.getValue() + getEffect(effect.getType(), effect.getSource()).getValue()));
}

void EffectProne::setEffect(const Effect &effect)
{
	if (effect.getType() == BTech::EffectType::None) {
		qWarning() << "Warning: adding empty effect";
		return;
	}
	Effect oldEffect = getEffect(effect.getType(), effect.getSource());
	effects.removeOne(oldEffect);
	effects.append(effect);
}

Effect EffectProne::getEffect(BTech::EffectType type, BTech::EffectSource source) const
{
	for (Effect effect : effects)
		if (effect.getType() == type && (effect.getSource() == source || source == BTech::EffectSource::NoSource))
			return effect;
	return Effect(type);
}

bool EffectProne::hasEffect(BTech::EffectType type, BTech::EffectSource source) const
{
	return getEffect(type, source).isActive();
}

void EffectProne::removeEffect(BTech::EffectType type, BTech::EffectSource source)
{
	for (Effect &effect : effects)
		if (effect.getType() == type && effect.getSource() == source)
			effects.removeOne(effect);
}

void EffectProne::removeEffects(BTech::EffectSource source)
{
	for (Effect &effect : effects)
		if (effect.getSource() == source)
			effects.removeOne(effect);
}

QList <Effect> EffectProne::getEffects() const
{
	return effects;
}

QList <Effect> & EffectProne::editEffects()
{
	return effects;
}

QDataStream & operator << (QDataStream &out, const EffectProne &obj)
{
	out << obj.effects;
	return out;
}

QDataStream & operator >> (QDataStream &in, EffectProne &obj)
{
	in >> obj.effects;
	return in;
}