#pragma once

class SWidget;

namespace SlateAttributePrivate
{
	struct FSlateAttributeNoInvalidationReason
	{
		static constexpr EInvalidateWidgetReason GetInvalidationReason(const SWidget&) { return EInvalidateWidgetReason::None; }
	};

	enum class ESlateAttributeType : uint8_t {
		Member = 0,//member of a sWidget(are not allowed to move)
		Managed = 1,//external to the sWidget, global variable or member that can moved
		Contained = 2//inside a FSlot or other Container that use dynamic memory(always attached to one and only one sWidget)
	};

	class ISlateAttributeContainer
	{
	public:
		virtual SWidget& GetContainerWidget() const = 0;

		virtual std::string GetContainerName() const = 0;

		virtual uint32_t GetContainerSortOrder() const = 0;

	protected:

		/* implement to SlateAttribute.cpp */
		void RemoveContainerWidget(SWidget& Widget);

		void UpdateContainerSortOrder(SWidget& Widget);
		/* implement to SlateAttribute.cpp */
	};

	class ISlateAttributeGetter
	{
	public:
		struct FUpdateAttributeResult
		{
			FUpdateAttributeResult(EInvalidateWidgetReason InInvalidationReason)
				: m_InvalidationReason(InInvalidationReason)
				, m_bInvalidationRequested(true)
			{}

			FUpdateAttributeResult()
				: m_InvalidationReason(EInvalidateWidgetReason::None)
				, m_bInvalidationRequested(false)
			{}

			EInvalidateWidgetReason m_InvalidationReason;
			bool m_bInvalidationRequested;
		};

		virtual FUpdateAttributeResult UpdateAttribute(const SWidget& Widget) = 0;

		virtual const FSlateAttributeBase& GetAttribute() const = 0;

		virtual void SetAttribute(FSlateAttributeBase&) = 0;

		virtual FDelegateHandle GetDelegateHandle() const = 0;

		virtual ~ISlateAttributeGetter() = default;
	};

	struct FSlateAttributeImpl : public FSlateAttributeBase
	{
	protected:
		void ProtectedUnregisterAttribute(SWidget& Widget, ESlateAttributeType AttributeType) const;

		void ProtectedRegisterAttribute(SWidget& Widget, ESlateAttributeType AttributeType, Scope<ISlateAttributeGetter>&& Wrapper);

		void ProtectedInvalidate_Widget(SWidget& Widget, ESlateAttributeType AttributeType, EInvalidateWidgetReason InvalidationReason) const;

		bool ProtectedIsBound(const SWidget& Widget, ESlateAttributeType AttributeType) const;

		//ISlateAttributeGetter* protected_find_getter(const SWidget& Widget, ESlateAttributeType AttributeType) const;

		ISlateAttributeGetter* ProtectedFindGetter(const SWidget& Widget, ESlateAttributeType AttributeType) const;

		FDelegateHandle ProtectedFindGetterHandle(const SWidget& Widget, ESlateAttributeType AttributeType) const;

		void ProtectedRegisterAttribute(ISlateAttributeContainer& Container, ESlateAttributeType AttributeType, Scope<ISlateAttributeGetter>&& Wrapper);

		void ProtectedUnregisterAttribute(ISlateAttributeContainer& Container, ESlateAttributeType AttributeType) const;

		void ProtectedInvalidateWidget(ISlateAttributeContainer& Container, ESlateAttributeType AttributeType, EInvalidateWidgetReason InvalidationReason) const;

		FDelegateHandle ProtectedFindGetterHandle(const ISlateAttributeContainer& Container, ESlateAttributeType AttributeType) const;

		void ProtectedUpdateNow(SWidget& Widget, ESlateAttributeType AttributeType);
	};
}
