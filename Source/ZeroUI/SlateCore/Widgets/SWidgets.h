#pragma once

#include "Core.h"
#include "SlateCore/Widgets/SlateControlledConstruction.h"
#include "SlateCore/Types/ISlateMetaData.h"
namespace ZeroUI
{ 
	/**
	 * Abstract base class for Slate widgets.
	 *
	 * STOP. DO NOT INHERIT DIRECTLY FROM WIDGET!
	 *
	 * Inheritance:
	 *   Widget is not meant to be directly inherited. Instead consider inheriting from LeafWidget or Panel,
	 *   which represent intended use cases and provide a succinct set of methods which to override.
	 *
	 *   SWidget is the base class for all interactive Slate entities. SWidget's public interface describes
	 *   everything that a Widget can do and is fairly complex as a result.
	 *
	 * Events:
	 *   Events in Slate are implemented as virtual functions that the Slate system will call
	 *   on a Widget in order to notify the Widget about an important occurrence (e.g. a key press)
	 *   or querying the Widget regarding some information (e.g. what mouse cursor should be displayed).
	 *
	 *   Widget provides a default implementation for most events; the default implementation does nothing
	 *   and does not handle the event.
	 *
	 *   Some events are able to reply to the system by returning an FReply, FCursorReply, or similar
	 *   object.
	 */
	class SWidget : public FSlateControlledConstruction, public std::enable_shared_from_this<SWidget>
	{
		SLATE_DECLARE_WIDGET(SWidget, FSlateControlledConstruction)

	protected:
		/**
		 * A SlateAttribute that is member variable of a SWidget.
		 * @usage: TSlateAttribute<int32> MyAttribute1; TSlateAttribute<int32, EInvalidateWidgetReason::Paint> MyAttribute2; TSlateAttribute<int32, EInvalidateWidgetReason::Paint, TSlateAttributeComparePredicate<>> MyAttribute3;
		 */
	public:

		template<class WidgetType, typename RequiredArgsPayloadType>
		friend struct TSlateDecl;
	public:
		virtual ~SWidget() override;

		/** @return true if the widgets has any bound slate attribute. */
		bool HasRegisteredSlateAttribute() const { return m_bHasRegisteredSlateAttribute; }
		/** @return true if the widgets will update its registered slate attributes automatically or they need to be updated manually. */
		bool IsAttributesUpdatesEnabled() const { return m_bEnabledAttributesUpdate; }

		void AssignParentWidget(Ref<SWidget> InParent);

		ZMath::vec2 GetDesiredSize() const;
	protected:
		/**
		 * The system calls this method. It performs a breadth-first traversal of every visible widget and asks
		 * each widget to cache how big it needs to be in order to present all of its content.
		 */
		virtual void CacheDesiredSize(float InLayoutScaleMultiplier);

		/**
		 * Compute the ideal size necessary to display this widget. For aggregate widgets (e.g. panels) this size should include the
		 * size necessary to show all of its children. CacheDesiredSize() guarantees that the size of descendants is computed and cached
		 * before that of the parents, so it is safe to call GetDesiredSize() for any children while implementing this method.
		 *
		 * Note that ComputeDesiredSize() is meant as an aide to the developer. It is NOT meant to be very robust in many cases. If your
		 * widget is simulating a bouncing ball, you should just return a reasonable size; e.g. 160x160. Let the programmer set up a reasonable
		 * rule of resizing the bouncy ball simulation.
		 *
		 * @param  LayoutScaleMultiplier    This parameter is safe to ignore for almost all widgets; only really affects text measuring.
		 *
		 * @return The desired size.
		 */
		virtual ZMath::vec2 ComputeDesiredSize(float LayoutScaleMultiplier) const = 0;
	private:
		/**
		 * Explicitly set the desired size. This is highly advanced functionality that is meant
		 * to be used in conjunction with overriding CacheDesiredSize. Use ComputeDesiredSize() instead.
		 */
		void SetDesiredSize(const ZMath::vec2& InDesiredSize)
		{
			m_DesiredSize = InDesiredSize;
		}

	private:
		/** Metadata associated with this widget. */
		std::vector<Ref<ISlateMetaData>> m_MetaData;

		/** Pointer to this widgets parent widget.  If it is null this is a root widget or it is not in the widget tree */
		Weak<SWidget> m_ParentWidgetPtr;

		/* stores the ideal size this widget wants to be */
		std::optional<ZMath::vec2> m_DesiredSize;
		/** Is there at least one SlateAttribute currently registered. */
		uint8_t m_bHasRegisteredSlateAttribute : 1;
		uint8_t m_bEnabledAttributesUpdate : 1;
	};
}