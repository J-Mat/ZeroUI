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
	class FChildreno
	{
	public:
		FChildren(SWidget* InOwner)
			: m_Owner(InOwner)
			, m_Name("Children")
		{
		}

		FChildren(SWidget* InOwner, DoDoUtf8String InName)
			: m_owner(InOwner)
			, m_name(InName)
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

		template<typename Predicate>
		void ForEachWidget(Predicate Pred)
		{
			for(int32_t Index = 0; Index < num(); ++Index)
			{
				std::shared_ptr<SWidget> ref = get_child_at(Index);

				Pred(ref.get());
			}
		}

		/* return the number of slot the children has */
		virtual int32_t NumSlot() const
		{
			return Num();
		}

		/* return the const reference to the slot at the specified index */
		virtual const FSlotBase& GetSlotAt(int32_t child_index) const = 0;


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
		SWidget *m_Owner;
	private:
		std::string m_Name;
	};
}
