#pragma once

namespace ZeroUI
{
	/*
	 * template to store enumeration values as bytes in a type-safe way
	 */
	template<class TEnum>
	class TEnumAsByte
	{
	public:
		typedef TEnum EnumType;

		TEnumAsByte() = default;

		TEnumAsByte(const TEnumAsByte&) = default;

		TEnumAsByte& operator=(const TEnumAsByte&) = default;

		/*
		 * constructor, initialize to the enum value
		 *
		 * @param InValue value to construct with
		 */
		TEnumAsByte(TEnum InValue)
			: m_Value(static_cast<uint8_t>(InValue))
		{}

		/*
		 * compares two enumeration values for equality
		 *
		 * @param InValue the value to compare with
		 * @return true if the two values are equal, false otherwise
		 */
		bool operator==(TEnum InValue) const
		{
			return static_cast<TEnum>(m_Value) == InValue;
		}

		/*
		 * compares two enumeration values for equality
		 *
		 * @param in_value the value to compare with
		 * @return true if the two values are equal, false otherwise
		 */
		bool operator==(TEnumAsByte in_value) const
		{
			return m_Value == in_value.m_Value;
		}

		/* implicit conversion to TEnum */
		operator TEnum() const
		{
			return (TEnum)m_Value;
		}

	private:
		//holds the value as a byte
		uint8_t m_Value;
	};
}