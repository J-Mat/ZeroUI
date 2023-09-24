#pragma once

#include "Core.h"
#include "SlateCore/Widgets/InvalidateWidgetReason.h"

namespace ZeroUI
{
	class SWidget;

	struct FSlateAttributeBase
	{
		/*
			not all invalidation is supported by slate attribute
			child order : the update of slate attribute is done in the slate prepass, we can't add or remove children in slate prepass
			attribute registration : in fast path, the slate attribute are updated in a loop, the iterator can't be modified while we are looping
		*/
		template<typename T>
		constexpr static bool IsInvalidateWidgetReasonSupported(T Reason)
		{
			return false;
		}

		constexpr static bool IsInvalidateWidgetReasonSupported(EInvalidateWidgetReason Reason)
		{
			return (Reason & (EInvalidateWidgetReason::Child_Order | EInvalidateWidgetReason::Attribute_Registration)) == EInvalidateWidgetReason::None;
		}
	};

	template<typename ComparePredicate = std::equal_to<void>>
	struct TSlateAttributeComparePredicate
	{
		template<typename ObjectType>
		static bool IdenticalTo(const SWidget&, ObjectType&& LHS, ObjectType&& RHS)
		{
			//construct a ComparePredicate object, and call it's operator, to compare two objects
			return ComparePredicate{}(std::forward<ObjectType>(LHS), std::forward<ObjectType>(RHS));
		}
	};

	template<EInvalidateWidgetReason InvalidationReason>
	struct TSlateAttributeInvalidationReason
	{
		static_assert(FSlateAttributeBase::IsInvalidateWidgetReasonSupported(InvalidationReason));
		static constexpr EInvalidateWidgetReason GetInvalidationReason(const SWidget&) { return InvalidationReason; }
	};
	
	//------inline template header file------
	#include "SlateAttributeDefinition.inl"
	#include "SlateAttributeBase.inl"

	#include "SlateAttributeContained.inl"
	#include "SlateAttributeManaged.inl"
	#include "SlateAttributeMember.inl"
	//------inline template header file------
}