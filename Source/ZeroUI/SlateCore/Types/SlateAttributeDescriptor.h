#pragma once

#include "Core.h"
#include "Core/Delegate.h"
#include "../Widgets/InvalidateWidgetReason.h"

namespace ZeroUI
{
	class FSlateWidgetClassData;
	class SWidget;
	namespace SlateAttributePrivate
	{
		enum class ESlateAttributeType : uint8_t;
	}
	/*
	 * describes the static information about a Widget's type slate attributes
	 */
	class FSlateAttributeDescriptor
	{
	public:
		/*
		 * a EInvalidationWidgetReason attribute
		 * it can be explicitly initialize or can be callback static function or lambda that returns the EInvalidationReason
		 * the signature of the function takes a const SWidget& as argument
		 */
		struct FInvalidateWidgetReasonAttribute
		{
		public:
			/*
			 * a EInvalidationWidgetReason Attribute
			 * it can be explicitly initialize or can be a callback static function or lambda that returns the EInvalidationReason
			 * the signature of the function takes a const SWidget& as argument
			 */
			friend FSlateAttributeDescriptor;

			using Arg1Type = const class SWidget&;

			//delegate, return EInvalidateWidgetReason, parameter is Arg1Type
			//class FGetter :public FSingleDelegate<int32_t, Arg1Type> {};
			DEFINITION_SIMPLE_SINGLE_DELEGATE(FGetter, EInvalidateWidgetReason, Arg1Type)

			FInvalidateWidgetReasonAttribute(const FInvalidateWidgetReasonAttribute&) = default;
			FInvalidateWidgetReasonAttribute(FInvalidateWidgetReasonAttribute&&) = default;
			FInvalidateWidgetReasonAttribute& operator=(const FInvalidateWidgetReasonAttribute&) = default;
			FInvalidateWidgetReasonAttribute& operator=(FInvalidateWidgetReasonAttribute&&) = default;

			explicit FInvalidateWidgetReasonAttribute(EInvalidateWidgetReason InReason)
				: m_Reason(InReason)
			{
			}

			bool IsBound() const
			{
				return m_Getter.IsBound();
			}

			EInvalidateWidgetReason Get(const SWidget& Widget) const
			{
				return IsBound() ? m_Getter.Execute(Widget) : m_Reason;
			}

		private:
			EInvalidateWidgetReason m_Reason;
			FGetter m_Getter;
		};

		DEFINITION_SIMPLE_SINGLE_DELEGATE(FAttributeValueChangedDelegate, void,  SWidget&);

		enum class ECallbackOverrideType
		{
			//replace the callback that the base class defined
			Replace_Previous,
			//execute the callback that the base class defined, then execute the new callback
			Execute_After_Previous,
			//execute the new call back, then execute the callback that the base class defined
			Execute_Before_Previous
		};

		using OffsetType = uint32_t;

		/* the default sort order that define in which order attributes will be updated */
		static constexpr  OffsetType DefaultSortOrder(OffsetType Offset) { return Offset * 100; }

		struct FContainer
		{
		public:
			FContainer() = default;

			FContainer(std::string InName, OffsetType InOffset)
				: m_Name(InName), m_Offset(InOffset)
			{
			}

			bool IsValid() const
			{
				return m_Name != "";
			}

			std::string GetName() const
			{
				return m_Name;
			}

			uint32_t GetSortOrder()
			{
				return m_SortOrder;
			}
		private:
			std::string m_Name;
			OffsetType m_Offset = 0;
			uint32_t m_SortOrder = 0;
		};

		struct FAttribute
		{
		public:
			friend FSlateAttributeDescriptor;
			//OffsetType = uint32_t
			FAttribute(std::string Name, OffsetType Offset, FInvalidateWidgetReasonAttribute Reason);
			FAttribute(std::string ContainerName, std::string Name, OffsetType Offset, FInvalidateWidgetReasonAttribute Reason);
			std::string Get_Name() const
			{
				return m_Name;
			}

			uint32_t GetSortOrder() const
			{
				return m_SortOrder;
			}

			EInvalidateWidgetReason GetInvalidationReason(const SWidget& Widget) const
			{
				//return m_InvalidationReason.
				return m_InvalidationReason.Get(Widget);//todo:need to complete this function
			}

			SlateAttributePrivate::ESlateAttributeType GetAttributeType() const
			{
				return m_AttributeType;
			}

			bool DoesAffectVisibility() const
			{
				return m_bAffectVisibility;
			}

			void ExecuteOnValueChanged(SWidget& Widget) const
			{
				m_OnValueChanged.Execute(Widget);
			}

		private:
			std::string m_Name;

			OffsetType m_Offset;

			std::string m_Perquisite;

			uint32_t m_SortOrder;

			uint8_t m_ContainerIndex;

			FInvalidateWidgetReasonAttribute m_InvalidationReason;

			FAttributeValueChangedDelegate m_OnValueChanged;

			SlateAttributePrivate::ESlateAttributeType m_AttributeType;

			bool m_bAffectVisibility;
		};

		struct FContainerInitializer
		{
		private:
			friend FSlateAttributeDescriptor;

			FContainerInitializer(FSlateAttributeDescriptor& InDescriptor, std::string ContainerName);

			//parent descriptor?
			FContainerInitializer(FSlateAttributeDescriptor& InDescriptor, const FSlateAttributeDescriptor& ParentDescriptor, std::string ContainerName);

		public:
			FContainerInitializer() = delete;

			FContainerInitializer(const FContainerInitializer&) = delete;

			FContainerInitializer& operator=(const FContainerInitializer&) = delete;

			struct FAttributeEntry
			{
				FAttributeEntry(FSlateAttributeDescriptor& Descriptor, std::string ContainerName, int32_t AtttributeIndex);

				/*
				 * update the attribute after the prerequisite
				 * the order is guaranteed but other attributes may be updated in between
				 * no order is guaranteed if the prerequisite or this property is updated manually
				 */
				FAttributeEntry& UpdatePrerequisite(std::string Prerequisite);

				/*
				 * notified when the attribute value changed
				 * it's preferable that you delay any action to the Tick or Paint function
				 * you are not allowed to make changes that would affect the swidget child order or it's visibility
				 * it will not be called when the swidget is in his construction phase
				 * @see SWidget::IsConstructed
				 */
				FAttributeEntry& OnValueChanged(FAttributeValueChangedDelegate Callback);

			private:
				FSlateAttributeDescriptor& m_Descriptor;
				std::string m_ContainerName;
				int32_t m_AttributeIndex;
			};

			FAttributeEntry AddContainedAttribute(std::string AttributeName, OffsetType Offset, const FInvalidateWidgetReasonAttribute& ReasonGetter);

			FAttributeEntry AddContainedAttribute(std::string AttributeName, OffsetType Offset, FInvalidateWidgetReasonAttribute&& ReasonGetter);

		public:
			//change the invalidation Reason of an attribute defined in a base class
			void OverrideInvalidationReason(std::string AttributeName, const FInvalidateWidgetReasonAttribute& Reason);

			void OverrideInvalidationReason(std::string AttributeName, FInvalidateWidgetReasonAttribute&& Reason);

			//change the FAttributeValueChangedDelegate of an attribute defined in a base class
			void OverrideOnValueChanged(std::string AttributeName, ECallbackOverrideType OverrideType, FAttributeValueChangedDelegate call_back);

		private:
			FSlateAttributeDescriptor& m_Descriptor;

			std::string m_ContainerName;
		};
		struct FInitializer
		{
		private:
			friend class FSlateWidgetClassData;

			FInitializer(FSlateAttributeDescriptor& InDescriptor);

			FInitializer(FSlateAttributeDescriptor& InDescriptor, const FSlateAttributeDescriptor& ParentDescriptor);

			FInitializer(const FInitializer&) = delete;

			FInitializer& operator=(const FInitializer&) = delete;

		public:
			~FInitializer();

			struct FAttributeEntry
			{
				FAttributeEntry(FSlateAttributeDescriptor& descriptor, int32_t in_attribute_index);

				/*
				 * update the attribute after the prerequisite
				 * the order is guaranteed but other attributes may be updated in between
				 * no order is guaranteed if the prerequisite or this property is updated manually
				 */
				FAttributeEntry& UpdatePrerequisite(std::string PreRequisite) {};

				/*
				 * the attribute affect the visibility of the widget
				 * we only update the attributes that can change the visibility of the widget when the widget is collapsed
				 * attributes that affect visibility must have the visibility attribute as a prerequisite or the visibility attribute must have it as a prerequisite
				 */
				FAttributeEntry& AffectVisibility();

				/*
				 * notified when the attribute value changed
				 * it's preferable that you delay any action to the tick or paint function
				 * you are not allowed to make changes that would affect the swidget child order or it's visibility
				 * it will not be called when the swidget is in his construction phase
				 * @see SWidget::IsConstructed
				 */
				FAttributeEntry& OnValueChanged(FAttributeValueChangedDelegate call_back);

			private:
				FSlateAttributeDescriptor& m_Descriptor;
				int32_t m_AttributeIndex;
			};

			FAttributeEntry AddMemberAttribute(std::string AttributeName, OffsetType Offset, const FInvalidateWidgetReasonAttribute& ReasonGetter);

			FAttributeEntry AddMemberAttribute(std::string AttributeName, OffsetType Offset, FInvalidateWidgetReasonAttribute&& ReasonGetter);

			FContainerInitializer AddContainer(std::string container_name, OffsetType Offset);

		public:

		private:
			FSlateAttributeDescriptor& m_Descriptor;
		};
		//todo:implement function
		/* returns the attribute of a slate attribute that have the corresponding memory Offset */
		const FAttribute* FindMemberAttribute(OffsetType AttributeOffset) const;
	private:
		FAttribute* FindAttribute(std::string AttributeName);

		FInitializer::FAttributeEntry AddMemberAttribute(std::string AttributeName, OffsetType Offset, FInvalidateWidgetReasonAttribute ReasonGetter);
	private:

		std::vector<FAttribute> m_Attributes;

		std::vector<FContainer> m_Containers;
	};
	using FSlateAttributeInitializer = FSlateAttributeDescriptor::FInitializer;
	//slot attribute initializer ? 
	using FSlateWidgetSlotAttributeInitializer = FSlateAttributeDescriptor::FContainerInitializer;
	/*
 * add a TSlateAttribute to the descriptor
 * @param _Initializer the FSlateAttributeInitializer from the PrivateRegisterAttributes function
 * @param _Property the TSlateAttribute property
 * @param _Reason the EInvalidationWidgetReason or a static function/lambda that takes a const SWidget& and that returns the invalidation reason
 */
#define SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(_Initializer, _Name, _Property, _Reason) \
	_Initializer.AddMemberAttribute(_Name, STRUCT_OFFSET(PrivateThisType, _Property), FSlateAttributeDescriptor::FInvalidateWidgetReasonAttribute{_Reason})
} // namespace ZeroUI
