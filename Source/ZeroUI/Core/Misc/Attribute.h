#pragma once

#include "../Delegate.h"

namespace ZeroUI
{
	template<typename ObjectType>
	class TAttribute
	{
	public:
		/**
		 * Attribute 'getter' delegate
		 *
		 * ObjectType GetValue() const
		 *
		 * @return  The attribute's value
		 */
		DECLARE_DELEGATE_RetVal( ObjectType, FGetter );
		/** Default constructor. */
		TAttribute()
			: m_Value()         // NOTE: Potentially uninitialized for atomics!!
			, m_bIsSet(false)
			, m_Getter()
		{
		}

		//constructor implicitly from an initial value
		//param In_Initial_Value the value for this attribute
		template<typename OtherType>
		TAttribute(const OtherType& InInitialValue)
			: m_Value(static_cast<ObjectType>(InInitialValue))
			, m_bIsSet(true)
			, m_Getter()
		{
		}

			//construct implicitly from moving an initial value
		//param In_Initial_Value
		TAttribute(ObjectType&& InInitialValue)
			: m_Value(std::move(InInitialValue))
			, m_bIsSet(true)
			, m_Getter()
		{
		}

		/*
		 * set the attribute's value
		 *
		 * param InNewValue the value to set the attribute to
		 */
		template<typename OtherType>
		void Set(const OtherType& InNewValue)
		{
			//unbind delegate
			m_Value = InNewValue;
			m_bIsSet = true;
		}

		/*
		 * set the attribute's value
		 *
		 * param InNewValue the value to set the attribute to
		 */
		void Set(ObjectType&& InNewValue)
		{
			m_Value = std::move(InNewValue);
			m_bIsSet = true;
		}

		//was this TAttribute ever assigned?
		bool IsSet() const
		{
			return m_bIsSet;
		}

		/*
		 * gets the attribute's current value
		 * assumes that the attribute is set
		 *
		 * return the attribute's value
		 */
		const ObjectType& Get() const
		{
			//if we have a getter delegate, then we'll call that to generate the value
			if(m_Getter.IsBound())
			{
				m_Value = m_Getter.Execute();
			}

			//return stored value
			return m_Value;
		}

		/*
		 * gets the attribute's current value, the attribute may not be set, in which case use the default value provided
		 * shorthand for the boilerplate code: MyAttribute.IsSet() ? MyAttribute.Get() : DefaultValue
		 */
		const ObjectType& Get(const ObjectType& DefaultValue) const
		{
			return m_bIsSet ? Get() : DefaultValue;
		}

		/*
		 * checks to see if this attribute has a 'getter' function bound
		 * return true if attribute is bound to a getter function
		 */
		bool IsBound() const
		{
			return m_Getter.IsBound();
		}

		const FGetter& GetBinding() const
		{
			return m_Getter;
		}

	private:
		/** Special explicit constructor for TAttribute::Create() */
		TAttribute( const FGetter& InGetter, bool bExplicitConstructor )
			: m_Value()
			, m_bIsSet( true )
			, m_Getter(InGetter)
		{ }

		// We declare ourselves as a friend (templated using OtherType) so we can access members as needed
		template< class OtherType > friend class TAttribute;

		/** Current value.  Mutable so that we can cache the value locally when using a bound Getter (allows const ref return value.) */
		mutable ObjectType m_Value;

		/** true when this attribute was explicitly set by a consumer, false when the attribute's value is set to the default*/
		bool m_bIsSet;

		/** Bound member function for this attribute (may be NULL if no function is bound.)  When set, all attempts
			to read the attribute's value will instead call this delegate to generate the value. */
		/** Our attribute's 'getter' delegate */
		FGetter m_Getter;
	};
}