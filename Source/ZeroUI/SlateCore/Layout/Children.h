#pragma once
#include "Core.h"

namespace ZeroUI
{
	class SWidget;
	class FSlotBase;
	/*
	 * FChildren is an interface that must be implemented by all child containers
	 * it allows iteration over a list of any widget's children regardless of how
	 * the underlying widget happens to store it's children
	 *
	 * FChildren is intended to be returned by the GetChildren() method
	 */
	class FChildren
	{
	public:
		FChildren(SWidget* InOwner)
			: m_Owner(InOwner)
			, m_Name("Children")
		{
		}

		FChildren(SWidget* InOwner, std::string InName)
			: m_Owner(InOwner)
			, m_Name(InName)
		{
		}

		FChildren(std::nullptr_t) = delete;
		//prevents allocation of FChildren. it creates confusion between FSlot and FChildren
		void* operator new(size_t) = delete;
		void* operator new[](size_t) = delete;

		/* return the number of children */
		virtual int32_t Num() const = 0;

		/* return pointer to the widget at the specified index */
		virtual Ref<SWidget> GetChildAt(int32_t Index) = 0;

		/* return const pointer to the widget at the specified index */
		virtual Ref<const SWidget> GetChildAt(int32_t Index) const = 0;

		/* return the SWidget that own the FChildren */
		SWidget& GetOwner() const { return *m_Owner;  }

				/* applies the predicate to all the widgets contained by the FChildren */
		template<typename Predicate>
		void ForEachWidget(Predicate Pred)
		{
			for(int32_t Index = 0; Index < Num(); ++Index)
			{
				Ref<SWidget> Reference = GetChildAt(Index);
				Pred(Reference.get());
			}
		}

		/* return the number of slot the children has */
		virtual int32_t NumSlot() const
		{
			return Num();
		}

		/* return the const reference to the slot at the specified index */
		virtual const FSlotBase& GetSlotAt(int32_t ChildIndex) const = 0;

				/* return if the children can support slot with slate slot attribute */
		virtual bool SupportSlotWithSlateAttribute() const
		{
			return false;
		}

		/*option to give a name to children to slot attribute purposes or for debugging*/
		std::string GetName() const
		{
			return m_Name;
		}

	protected:
		SWidget* m_Owner;

	private:
		std::string m_Name;
	};
}