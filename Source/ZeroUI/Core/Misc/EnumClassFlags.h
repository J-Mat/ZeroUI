#pragma once

#include <type_traits>

namespace ZeroUI
{
	// Defines all bitwise operators for enum classes so it can be (mostly) used as a regular flags enum
	#define ENUM_CLASS_FLAGS(Enum) \
		inline           Enum& operator|=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((std::underlying_type<Enum>::type)Lhs | (std::underlying_type<Enum>::type)Rhs); } \
		inline           Enum& operator&=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((std::underlying_type<Enum>::type)Lhs & (std::underlying_type<Enum>::type)Rhs); } \
		inline           Enum& operator^=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((std::underlying_type<Enum>::type)Lhs ^ (std::underlying_type<Enum>::type)Rhs); } \
		inline constexpr Enum  operator| (Enum  Lhs, Enum Rhs) { return (Enum)((std::underlying_type<Enum>::type)Lhs | (std::underlying_type<Enum>::type)Rhs); } \
		inline constexpr Enum  operator& (Enum  Lhs, Enum Rhs) { return (Enum)((std::underlying_type<Enum>::type)Lhs & (std::underlying_type<Enum>::type)Rhs); } \
		inline constexpr Enum  operator^ (Enum  Lhs, Enum Rhs) { return (Enum)((std::underlying_type<Enum>::type)Lhs ^ (std::underlying_type<Enum>::type)Rhs); } \
		inline constexpr bool  operator! (Enum  E)             { return !(std::underlying_type<Enum>::type)E; } \
		inline constexpr Enum  operator~ (Enum  E)             { return (Enum)~(std::underlying_type<Enum>::type)E; }

	// Friends all bitwise operators for enum classes so the definition can be kept private / protected.
	#define FRIEND_ENUM_CLASS_FLAGS(Enum) \
		friend           Enum& operator|=(Enum& Lhs, Enum Rhs); \
		friend           Enum& operator&=(Enum& Lhs, Enum Rhs); \
		friend           Enum& operator^=(Enum& Lhs, Enum Rhs); \
		friend constexpr Enum  operator| (Enum  Lhs, Enum Rhs); \
		friend constexpr Enum  operator& (Enum  Lhs, Enum Rhs); \
		friend constexpr Enum  operator^ (Enum  Lhs, Enum Rhs); \
		friend constexpr bool  operator! (Enum  E); \
		friend constexpr Enum  operator~ (Enum  E);

	template<typename Enum>
	constexpr bool EnumHasAllFlags(Enum Flags, Enum Contains)
	{
		return ( ( ( std::underlying_type<Enum>::type )Flags ) & ( std::underlying_type<Enum>::type )Contains ) == ( ( std::underlying_type<Enum>::type )Contains );
	}

	template<typename Enum>
	constexpr bool EnumHasAnyFlags(Enum Flags, Enum Contains)
	{
		return ( ( ( std::underlying_type<Enum>::type )Flags ) & ( std::underlying_type<Enum>::type )Contains ) != 0;
	}

	template<typename Enum>
	void EnumAddFlags(Enum& Flags, Enum FlagsToAdd)
	{
		Flags |= FlagsToAdd;
	}

	template<typename Enum>
	void EnumRemoveFlags(Enum& Flags, Enum FlagsToRemove)
	{
		Flags &= ~FlagsToRemove;
	}
}