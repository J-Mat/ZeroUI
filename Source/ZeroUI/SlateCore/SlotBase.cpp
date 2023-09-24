#include "SlotBase.h"

namespace ZeroUI
{
	FSlotBase::FSlotBase()
	: m_Owner(nullptr)
	, m_Widget()
	{
	}

	FSlotBase::FSlotBase(const FChildren& InParent)
	: m_Owner(&InParent)
	, m_Widget()
	{
	}

	FSlotBase::~FSlotBase()
	{
	}

	FSlotBase::FSlotBase(const Ref<SWidget>& InWidget)
	: m_Owner(nullptr)
	, m_Widget(InWidget)
	{
	}

	SWidget* FSlotBase::GetOwnerWidget() const
	{
		return GetOwner() ? &(GetOwner()->GetOwner()) : nullptr;
	}

	void FSlotBase::SetOwner(const FChildren& Children)
	{
		if(m_Owner != &Children)
		{
			m_Owner = &Children;
			AfterContentOrOwnerAssigned();
		}
	}

	const Ref<SWidget> FSlotBase::DetachWidget()
	{
		if(m_Widget != nullptr)
		{
			m_Widget->ConditionallyDetachParentWidget(GetOwneridget());
		}
	}

	void FSlotBase::DetachParentFromContent()
	{
		if(m_Widget != nullptr)
		{
			m_Widget->ConditionallyDetachParentWidget(GetOwnerWidget());
		}
	}

	void FSlotBase::AfterContentOrOwnerAssigned()
	{
		if (SWidget* OwnerWidget = GetOwnerWidget())
		{
			// TODO NDarnell I want to enable this, but too many places in the codebase
			// have made assumptions about being able to freely reparent widgets, while they're
			// still connected to an existing hierarchy.
			//ensure(!Widget->IsParentValid());
			m_Widget->AssignParentWidget(OwnerWidget->shared_from_this());
		}
	}
	}
}