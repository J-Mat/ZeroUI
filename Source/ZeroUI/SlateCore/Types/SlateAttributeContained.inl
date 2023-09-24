#pragma once

namespace SlateAttributePrivate
{
	template<typename InObjectValue, typename InInvalidationReasonPredicate, typename InComparePredicate>
	struct TSlateContainedAttribute :
		public TSlateAttributeBase<ISlateAttributeContainer,
									InObjectValue,
									InInvalidationReasonPredicate,
									InComparePredicate,
									ESlateAttributeType::Contained>
	{
	private:
		using Super = TSlateAttributeBase<ISlateAttributeContainer, InObjectValue, InInvalidationReasonPredicate, InComparePredicate, ESlateAttributeType::Contained>;

		template<typename ContainerType, typename U = typename std::enable_if<std::is_base_of<ISlateAttributeContainer, ContainerType>::value>::type>
		static void VerifyAttributeAddress(const ContainerType& Container, const TSlateContainedAttribute& Self)
		{
			//todo:check address
		}
	public:
		using FGetter = typename Super::FGetter;//note:this is a real delegate
		using ObjectType = typename Super::ObjectType;

	public:
		TSlateContainedAttribute() = delete;
		TSlateContainedAttribute(const TSlateContainedAttribute&) = delete;
		TSlateContainedAttribute(TSlateContainedAttribute&&) = delete;
		TSlateContainedAttribute& operator=(const TSlateContainedAttribute&) = delete;
		TSlateContainedAttribute& operator=(TSlateContainedAttribute&&) = delete;

		template<typename ContainerType, typename U = typename std::enable_if<std::is_base_of<ISlateAttributeContainer, ContainerType>::value>::type>
		explicit TSlateContainedAttribute(ContainerType& Container)
		{
			//todo:check address
		}

		template<typename ContainerType, typename U = typename std::enable_if<std::is_base_of<ISlateAttributeContainer, ContainerType>::value>::type>
		explicit TSlateContainedAttribute(ContainerType& Container, ObjectType&& InValue)
			: Super(std::move(InValue))
		{
			//todo:check address
		}

		template<typename ContainerType, typename U = typename std::enable_if<std::is_base_of<ISlateAttributeContainer, ContainerType>::value>::type>
		explicit TSlateContainedAttribute(ContainerType& Container, ObjectType&& InValue)
			: Super(std::move(InValue))
		{
			//todo:check address
		}
	};
}