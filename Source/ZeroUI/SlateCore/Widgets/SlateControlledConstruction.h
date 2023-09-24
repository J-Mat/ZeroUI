#pragma once

#include "Core.h"
#include "Core/Templates/Identity.h"
#include "../Types/SlateAttributeDescriptor.h"

namespace ZeroUI
{
	class FSlateControlledConstruction;
#define SLATE_DECLARE_WIDGET(WidgetType, ParentType) \
	private:\
		using Super = ParentType;\
		using ThisClass = WidgetType;\
		using PrivateThisType = WidgetType;\
		static const FSlateWidgetClassData& GetPrivateWidgetClass() \
		{\
			static FSlateWidgetClassData WidgetClassDataInstance = FSlateWidgetClassData(TIdentity<ParentType>(), #WidgetType, &WidgetType::PrivateRegisterAttributes);\
			return Widget_Class_Data_Instance;\
		}\
		static void PrivateRegisterAttributes(FSlateAttributeInitializer&);\
	public:\
		static const FSlateWidgetClassData& StaticWidgetClass() { return GetPrivateWidgetClass(); }\
		virtual const FSlateWidgetClassData& GetWidgetClass() const override { return GetPrivateWidgetClass();}\
	private:

#define SLATE_IMPLEMENT_WIDGET(WidgetType)\
	FSlateWidgetClassRegistration ClassRegistration__##WidgetType = WidgetType::StaticWidgetClass();

	/** */
	class FSlateWidgetClassData
	{
	private:
		friend FSlateControlledConstruction;

		FSlateWidgetClassData(std::string InWidgetTypeName)
			: WidgetType(InWidgetTypeName)
		{}

	public:
		template<typename InWidgetParentType>
		FSlateWidgetClassData(TIdentity<InWidgetParentType>, std::string InWidgetTypeName, void(*AttributeInitializer)(FSlateAttributeInitializer&))
			: WidgetType(InWidgetTypeName)
		{
			// Initialize the parent class if it's not already done
			const FSlateWidgetClassData& ParentWidgetClassData = InWidgetParentType::StaticWidgetClass();
			// Initialize the attribute descriptor
			FSlateAttributeInitializer Initializer = { Descriptor, ParentWidgetClassData.GetAttributeDescriptor() };
			(*AttributeInitializer)(Initializer);
		}

		const FSlateAttributeDescriptor& GetAttributeDescriptor() const { return Descriptor; };
		std::string GetWidgetType() const { return WidgetType; }

	private:
		FSlateAttributeDescriptor Descriptor;
		std::string WidgetType;
	};

	struct FSlateWidgetClassRegistration
	{
		FSlateWidgetClassRegistration(const FSlateWidgetClassData&) {}
	};

	class FSlateControlledConstruction
	{
	public:
		FSlateControlledConstruction() = default;
		virtual ~FSlateControlledConstruction() = default;

	public:
		static const FSlateWidgetClassData& StaticWidgetClass()
		{
			static FSlateWidgetClassData Instance = FSlateWidgetClassData("FSlateControlledConstruction");
			return Instance;
		}

		virtual const FSlateWidgetClassData& GetWidgetClass() const = 0;

	private:
		/** UI objects cannot be copy-constructed */
		FSlateControlledConstruction(const FSlateControlledConstruction& other) = delete;

		/** Widgets should only ever be constructed via SNew or SAssignNew */
		void* operator new(const size_t In_Size)
		{
			return std::malloc(In_Size);
		}

	public:
		void operator delete(void* mem)
		{
			std::free(mem);
		}
	};
}