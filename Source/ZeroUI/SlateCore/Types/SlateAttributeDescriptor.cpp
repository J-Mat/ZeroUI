#include "SlateAttributeDescriptor.h"

namespace ZeroUI
{
		
	FSlateAttributeDescriptor::FAttribute::FAttribute(std::string Name, OffsetType Offset, FInvalidateWidgetReasonAttribute Reason)
		: m_Name(Name)
		, m_Offset(Offset)
		, m_InvalidationReason(Reason)
	{
	}

	FSlateAttributeDescriptor::FContainerInitializer::FAttributeEntry(
		FSlateAttributeDescriptor& Descriptor, std::string ContainerName, int32_t AtttributeIndex)
		: m_Descriptor(Descriptor)
		, m_AttributeIndex(AtttributeIndex)
	{
	}

	FSlateAttributeDescriptor::FInitializer::~FInitializer()
	{
	}

	FSlateAttributeDescriptor::FInitializer::FAttributeEntry FSlateAttributeDescriptor::FInitializer::AddMemberAttribute(std::string AttributeName, OffsetType Offset, const FInvalidateWidgetReasonAttribute& ReasonGetter)
	{
		//call descriptor's private function
		return m_Descriptor.AddMemberAttribute(AttributeName, Offset, ReasonGetter);
	}

	FSlateAttributeDescriptor::FInitializer::FAttributeEntry FSlateAttributeDescriptor::FInitializer::AddMemberAttribute(std::string AttributeName, OffsetType Offset, FInvalidateWidgetReasonAttribute&& ReasonGetter)
	{
		return m_Descriptor.AddMemberAttribute(AttributeName, Offset, std::move(ReasonGetter));
	}

	FSlateAttributeDescriptor::FContainerInitializer FSlateAttributeDescriptor::FInitializer::AddContainer(std::string container_name, OffsetType Offset)
	{
	}

	FSlateAttributeDescriptor::FInitializer::FAttributeEntry(FSlateAttributeDescriptor& Descriptor, int32_t InAttributeIndex)
		:m_Descriptor(Descriptor)
		, m_AttributeIndex(InAttributeIndex)
	{
	}

	FSlateAttributeDescriptor::FInitializer::FAttributeEntry& FSlateAttributeDescriptor::FInitializer::FAttributeEntry::AffectVisibility()
	{
		return (*this);
	}

	const FSlateAttributeDescriptor::FAttribute* FSlateAttributeDescriptor::FindMemberAttribute(OffsetType AttributeOffset) const
	{
		const FSlateAttributeDescriptor::FAttribute* Result = nullptr;
		auto Iter = std::find_if(m_Attributes.begin(), m_Attributes.end(), [AttributeOffset](const FAttribute& Other)
			{
				return Other.m_Offset == AttributeOffset
					&& Other.m_AttributeType == SlateAttributePrivate::ESlateAttributeType::Member;
			});
		if (Iter != m_Attributes.end()) Result = &(*Iter);

		return Result;
	}

	FSlateAttributeDescriptor::FAttribute* FSlateAttributeDescriptor::FindAttribute(std::string AttributeName)
	{
		auto Iter = std::find_if(m_Attributes.begin(), m_Attributes.end(), [AttributeName](const FAttribute& Other) { return Other.m_Name == AttributeName; });

		if (Iter == m_Attributes.end()) 
			return nullptr;
		return &(*Iter);
	}

	FSlateAttributeDescriptor::FInitializer::FAttributeEntry FSlateAttributeDescriptor::AddMemberAttribute(std::string AttributeName, OffsetType Offset, FInvalidateWidgetReasonAttribute ReasonGetter)
	{
		int32_t NewIndex = = 1;
		FAttribute const* Attribute = FindAttribute(AttributeName);

		m_Attributes.emplace_back(AttributeName, Offset, ReasonGetter);
		NewIndex = m_Attributes.size() - 1;

		return FInitializer::FAttributeEntry(*this, NewIndex);
	}

} // namespace ZeroUI