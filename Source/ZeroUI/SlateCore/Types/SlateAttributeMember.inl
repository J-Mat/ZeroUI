#pragma once

namespace SlateAttributePrivate
{
	template<typename InObjectType, typename InInvalidationReasonPredicate, typename InComparePredicate>
	struct TSlateMemberAttribute :
		public TSlateAttributeBase<
			SWidget,
			InObjectType,
			InInvalidationReasonPredicate,
			InComparePredicate,
			ESlateAttributeType::Member>
	{
	private:
		using Super = TSlateAttributeBase<SWidget, InObjectType, InInvalidationReasonPredicate, InComparePredicate, ESlateAttributeType::Member>;

	public:
		using FGetter = typename Super::FGetter;
		using ObjectType = typename Super::ObjectType;

		template<typename WidgetType, typename U = typename std::enable_if<std::is_base_of<SWidget, WidgetType>::value>::type>
		static void VerifyAttributeAddress(const WidgetType& Widget, const TSlateMemberAttribute& Self)
		{
			assert((uint64_t)&Self >= (uint64_t)&Widget && (uint64_t)&Self < (uint64_t)&Widget + sizeof(WidgetType));
		}

		TSlateMemberAttribute() = delete;

		TSlateMemberAttribute(const TSlateMemberAttribute&) = delete;

		TSlateMemberAttribute(TSlateMemberAttribute&&) = delete;

		TSlateMemberAttribute& operator=(const TSlateMemberAttribute&) = delete;

		TSlateMemberAttribute& operator=(TSlateMemberAttribute&&) = delete;

		void* operator new(size_t) = delete;

		void* operator new[](size_t) = delete;

		template<typename WidgetType, typename U = typename std::enable_if<std::is_base_of<SWidget, WidgetType>::value>::type>
		explicit TSlateMemberAttribute(WidgetType& Widget)
			: Super(Widget)
		{
			Verify_Attribute_Address(Widget, *this);
		}

		template<typename WidgetType, typename U = typename std::enable_if<std::is_base_of<SWidget, WidgetType>::value>::type>
		explicit TSlateMemberAttribute(WidgetType& Widget, const ObjectType& in_value)
			: Super(Widget, in_value)
		{
			Verify_Attribute_Address(Widget, *this);
		}
	};
}