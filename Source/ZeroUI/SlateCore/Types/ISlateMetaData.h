#pragma once

#include "Core.h"

namespace ZeroUI
{
	/**
	 * Base class for all slate metadata
	 */
	class ISlateMetaData
	{
	public:
		/** Check if this metadata operation can cast safely to the specified template type */
		template<class TType>
		bool IsOfType() const
		{
			return IsOfTypeImpl(TType::GetTypeId());
		}

		/** Virtual destructor. */
		virtual ~ISlateMetaData() {}

	protected:
		/**
		 * Checks whether this drag and drop operation can cast safely to the specified type.
		 */
		virtual bool IsOfTypeImpl(const std::string& Type) const
		{
			return false;
		}
	};

	/**
	 * All metadata-derived classes must include this macro.
	 * Example Usage:
	 *	class FMyMetaData : public ISlateMetaData
	*	{
	*	public:
	*		SLATE_METADATA_TYPE(FMyMetaData, ISlateMetaData)
	*		...
	*	};
	*/
	#define SLATE_METADATA_TYPE(TYPE, BASE) \
		static const std::string& GetTypeId() { static std::string Type(TEXT(#TYPE)); return Type; } \
		virtual bool IsOfTypeImpl(const std::string& Type) const override { return GetTypeId() == Type || BASE::IsOfTypeImpl(Type); }

	/**
	 * Simple tagging metadata
	 */
	class FTagMetaData : public ISlateMetaData
	{
	public:
		SLATE_METADATA_TYPE(FTagMetaData, ISlateMetaData)

			FTagMetaData(std::string InTag)
			: Tag(InTag)
		{}

		/** Tag name for a widget */
		std::string Tag;
	};
} // namespace ZeroUI
