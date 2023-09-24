#pragma once
#include "Core.h"
#include "SlateCore/Types/SlateEnums.h"


namespace ZeroUI
{
	template<typename MixedIntoType>
	class TAlignmentWidgetSlotMixin
	{
	public:
		TAlignmentWidgetSlotMixin()
		: m_HAlignment(HAlign_Fill)
		, m_VAlignment(VAlign_Fill)
		{
		}
		TAlignmentWidgetSlotMixin(const EHorizontalAlignment InHAlign, const EVerticalAlignment InVAlign)
		: m_HAlignment(InHAlign)
		, m_VAlignment(InVAlign)
		{
		}
	public:
		struct FSlotArgumentsMixin
		{
		public:
			friend class TAlignmentWidgetSlotMixin;
		public:
			typename MixedIntoType::FSlotArguments& HAlign(EHorizontalAlignment InHAlignment)
			{
				m_HAlignment = InHAlignment;
				return static_cast<typename MixedIntoType::FSlotArguments&>(this);
			}

			typename MixedIntoType::FSlotArguments& VAlign(EVerticalAlignment InVAlignment)
			{
				m_VAlignment = InVAlignment;
				return static_cast<typename MixedIntoType::FSlotArguments&>(*this);
			}
		private:
			std::optional<EHorizontalAlignment> m_HAlignment;
			std::optional<EVerticalAlignment> m_VAlignment;
		};
	protected:
		void ConstructMixin(const FChildren& SlotOwner, FSlotArgumentsMixin&& InArgs)
		{
			m_HAlignment = InArgs.m_VAlignment.value_or(m_HAlignment);
			m_VAlignment = InArgs.m_VAlignment.value_or(m_VAlignment);
		}
	public:
		//HAlign will be deprecated soon, use set vertical alignment or construct a new slot with FSlotArguments
		MixedIntoType& HAlign(EHorizontalAlignment InHalignment)
		{
			m_HAlignment = InHalignment;
			return *(static_cast<MixedIntoType*>(this));
		}

		MixedIntoType& VAlign(EVerticalAlignment InValignment)
		{
			m_VAlignment = InValignment;
			return *(static_cast<MixedIntoType*>(this));
		}

		void SetHorizontalAlignment(EHorizontalAlignment Alignment)
		{
			if(m_HAlignment != Alignment)
			{
				m_HAlignment = Alignment;
				//call widget's Invalidate function
				static_cast<MixedIntoType*>(this)->Invalidate(EInvalidateWidgetReason::Layout);
			}
		}

		EHorizontalAlignment GetHorizontalAlignment() const
		{
			return m_HAlignment;
		}

		void SetVerticalAlignment(EVerticalAlignment Alignment)
		{
			if(m_VAlignment != Alignment)
			{
				m_VAlignment = Alignment;
				static_cast<MixedIntoType*>(this)->Invalidate(EInvalidateWidgetReason::Layout);
			}
		}

		EVerticalAlignment GetVerticalAlignment() const
		{
			return m_VAlignment;
		}
	public:
		/*horizontal positioning of child within the allocated slot*/
		EHorizontalAlignment m_HAlignment;

		/*vertical positioning of child within the allocated slot*/
		EVerticalAlignment m_VAlignment;
	};

	/*
	template<typename MixedIntoType>
	class TPaddingWidgetSlotMixin
	{
	public:
		TPaddingWidgetSlotMixin()
			: 
	};
	*/
}