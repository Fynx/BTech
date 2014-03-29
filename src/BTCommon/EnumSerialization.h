#ifndef ENUMSERIALIZATION_HPP
#define ENUMSERIALIZATION_HPP

#include <type_traits>

template <typename ET>
constexpr typename std::underlying_type <ET>::type toUnderlying(ET et)
{
	return static_cast<typename std::underlying_type<ET>::type>(et);
}

template <typename ET>
constexpr typename std::underlying_type <ET>::type & toUnderlyingRef(ET &et)
{
	return reinterpret_cast<typename std::add_lvalue_reference<decltype(toUnderlying(et))>::type>(et);
}

#endif
