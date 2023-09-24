#pragma once

#include "Core/Misc/Attribute.h"
#include "SlateCore/Widgets/InvalidateWidgetReason.h"

namespace ZeroUI
{
	class FChildren;
	class SWidget;

	class FSlotBase
	{
	public:
		FSlotBase();
		FSlotBase(const FChildren& InParent);
		virtual ~FSlotBase();
		FSlotBase& operator=(const FSlotBase&) = delete;
		FSlotBase(const FSlotBase&) = delete;
		FSlotBase(const Ref<SWidget>& InWidget);
	public:
		//this is empty, used by inherited
		struct FSlotArguments {};

		void AttachWidgetParent(SWidget* InParent) {}

		/*
		 * access the FChildren that own the slot
		 * the owner can be invalid when slot is not attached
		 */
		const FChildren* GetOwner() const
		{
			return m_Owner;
		}

		/*
		 * access the FChildren that own the slot
		 * the owner can be invalid when the slot is not attached
		 */
		SWidget* GetOwnerWidget() const;

		/*
		 * set the owner of the slot
		 * slots cannot be reassigned to different parents
		 */
		void SetOwner(const FChildren& children);

		void AttachWidget(const Ref<SWidget>& InWidget)
		{
			detach_parent_from_content();
			m_Widget = InWidget;
			AfterContentOrOwnerAssigned();
		}

		/*
		 * access the widget in the current slot
		 * there will always be a widget in the slot, sometimes it is the SNullWidget instance
		 */
		const Ref<SWidget> GetWidget() const
		{
			return m_Widget;
		}

		/**
		 * Remove the widget from its current slot.
		* The removed widget is returned so that operations could be performed on it.
		* If the null widget was being stored, an invalid shared ptr is returned instead.
		*/
		const Ref<SWidget> DetachWidget();

		/* invalidate the widget's owner */
		void Invalidate(EInvalidateWidgetReason InvalidateReason);
	protected:
		/*
		 * performs the attribute assignment and invalidates the widget minimally based on what actually changed
		 * so if the boundness of the attribute didn't changed volatility won't need to be recalcualted
		 * returns true if the value changed
		 */
		template<typename TargetValueType, typename SourceValueType>
		bool SetAttribute(TAttribute<TargetValueType>& TargetValue, const TAttribute<SourceValueType>& SourceValue, EInvalidateWidgetReason BaseInvalidationReason)
		{
			return false;
		}

	private:
		void DetachParentFromContent();
		void AfterContentOrOwnerAssigned();
	private:
		/* the children that own the slot */
		const FChildren* m_Owner;

		/* the content widget of the slot */
		Ref<SWidget> m_Widget;
	};

	template<typename SlotType>
	class TSlotBase : public FSlotBase
	{
	public:
		using FSlotBase::FSlotBase;

		SlotType& operator[](const Ref<SWidget>& InChildWidget)
		{
			this->AttachWidget(InChildWidget);
			return static_cast<SlotType&>(*this);
		}

		SlotType& Expose(SlotType*& OutVarToInit)
		{
			OutVarToInit = static_cast<SlotType*>(this);
			return static_cast<SlotType&>(*this);
		}

		/* argument to indicate the slot is also it's owner */
		enum EConstructSlotIsFChildren{ ConstructSlotIsFChildren };

		struct FSlotArguments : public FSlotBase::FSlotArguments
		{
		public:
			FSlotArguments(EConstructSlotIsFChildren){}
			FSlotArguments(Scope<SlotType> InSlot)
			: m_Slot(std::move(InSlot)){}

			FSlotArguments(const FSlotArguments&) = delete;
			FSlotArguments& operator=(const FSlotArguments&) = delete;
			FSlotArguments(FSlotArguments&&) = default;
			FSlotArguments& operator=(FSlotArguments&&) = default;
		public:
			typename SlotType::FSlotArguments& operator[](const Ref<SWidget>& InChildWidget)
			{
				m_ChildWidget = InChildWidget;
				return Me();
			}

			typename SlotType::FSlotArguments& Expose(SlotType*& OutVarToInit)
			{
				OutVarToInit = m_Slot.get();

				return Me();
			}

			void AttachWidget(const std::shared_ptr<SWidget>& InChildWidget)
			{
				m_ChildWidget = InChildWidget;
			}

			const Ref<SWidget>& GetAttachedWidget() const { return m_ChildWidget; }

			SlotType* GetSlot() const { return m_Slot.get(); }

			Scope<SlotType> StealSlot()
			{
				return std::move(m_Slot);
			}

			typename SlotType::FSlotArguments& Me()
			{
				return static_cast<typename SlotType::FSlotArguments&>(*this);
			}
		private:
			//owner ship?
			Scope<SlotType> m_Slot;

			Ref<SWidget> m_ChildWidget;
		};

		void Construct(const FChildren& SlotOwner, FSlotArguments&& InArgs)
		{
			if (InArgs.GetAttachedWidget())
			{
				AttachWidget(InArgs.GetAttachedWidget());
			}
			SetOwner(SlotOwner);
		}
	};
}