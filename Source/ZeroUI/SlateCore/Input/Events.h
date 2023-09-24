#pragma once

#include "Core.h"
#include "Core/InputCore/InputCoreTypes.h"
#include "ApplicationCore/GenericPlatform/GenericApplication.h"

namespace ZeroUI
{
	struct FVirtualPointerPosition
	{
		FVirtualPointerPosition()
			: m_CurrentCursorPosition(ZMath::vec2(0.0f, 0.0f))
			, m_LastCursorPosition(ZMath::vec2(0.0f, 0.0f))
		{}

		FVirtualPointerPosition(const ZMath::vec2& InCurrentCursorPosition, const ZMath::vec2& InLastCursorPosition)
			: m_CurrentCursorPosition(InCurrentCursorPosition)
			, m_LastCursorPosition(InLastCursorPosition)
		{}

		ZMath::vec2 m_CurrentCursorPosition;
		ZMath::vec2 m_LastCursorPosition;
	};
	class FWidgetPath;

	struct FInputEvent
	{
	public:
		FInputEvent()
			: m_ModifierKeys(FModifierKeysState())
			, m_bIsRepeat(false)
			, m_UserIndex(0)
			, m_EventPath(nullptr)
		{}

		/*
		 * constructor, events are immutable once constructed
		 *
		 * @param InModifierKeys modifier key state for this event
		 * @param bInIsRepeat true if this key is an auto-repeated keystoke
		 */
		FInputEvent(const FModifierKeysState& InModifierKeys, const int32_t InUserIndex, const bool bInIsRepeate)
			: m_ModifierKeys(InModifierKeys)
			, m_bIsRepeat(bInIsRepeate)
			, m_UserIndex(InUserIndex)
			, m_EventPath(nullptr)
		{}

		virtual ~FInputEvent() {}

		/*set the widget path along which this event will be routed*/
		void SetEventPath(const FWidgetPath& InEventPath)
		{
			m_EventPath = &InEventPath;
		}

		/*
		 * returns the index of the user that generated this event
		 */
		uint32_t GetUserIndex() const
		{
			return m_UserIndex;
		}
	protected:
		//state of modifier keys when this event happened
		FModifierKeysState m_ModifierKeys;//shift tab caps lock

		//true if this key was auto-repeated
		bool m_bIsRepeat;

		//the index of the user that caused the event
		uint32_t m_UserIndex;

		//events are sent along paths, see (GetEventPath)
		const FWidgetPath* m_EventPath;
	};

	/*
	 * FKeyEvent describes a key action (keyboard/controller key/button pressed or released)
	 * it is passed to event handlers dealing with key input
	 */

	struct FKeyEvent : public FInputEvent
	{
	public:
		FKeyEvent()
			: FInputEvent(FModifierKeysState(), 0, false)
			, m_Key()
			, m_CharacterCode(0)
			, m_KeyCode(0)
		{}
		/*
		 * constructor, events are immutable once constructed
		 *
		 * @param InKeyName character name
		 * @param InModifierKeys modifier key state for this event
		 * @param bInIsRepeat true if this key is an auto-repeated keystroke
		 */
		FKeyEvent(const FKey InKey,
					const FModifierKeysState& InModifierKeys,
					const uint32_t InUserIndex,
					const bool bInIsRepeat,
					const uint32_t InCharacterCode,
					const uint32_t InKeyCode)
		: FInputEvent(InModifierKeys, InUserIndex, bInIsRepeat)
		, m_Key(InKey)
		, m_CharacterCode(InCharacterCode)
		, m_KeyCode(InKeyCode)
		{}
	private:
		//name of the key that was pressed
		FKey m_Key;

		//the character code of the key that was pressed, only applicable to typed character keys, 0 otherwise
		uint32_t m_CharacterCode;

		//original key code received from hardware before any conversion/mapping
		uint32_t m_KeyCode;//note:original
	};

	/*
	 * FPointerEvent describes a mouse or touch action (e.g. Press, Release, Move, etc)
	 * it is passed to event handlers dealing with pointer-based input
	 */
	struct FPointerEvent
		: public FInputEvent
	{
	public:
		/*
		 * Events are immutable once constructed
		 */
		FPointerEvent(
			uint32_t InPointerIndex,
			const ZMath::vec2& InScreenSpacePosition,
			const ZMath::vec2& InLastscreenSpacePosition,
			const FModifierKeysState& in_modifier_keys
		)
			: FInputEvent(in_modifier_keys, 0, false)
			, m_ScreenSpacePosition(InScreenSpacePosition)
			, m_LastScreenSpacePosition(InLastscreenSpacePosition)
			, m_CursorDelta(InScreenSpacePosition - InLastscreenSpacePosition)
			, m_PointerIndex(InPointerIndex)
		{}

		FPointerEvent(
			uint32_t InPointerIndex,
			const ZMath::vec2& InScreenSpacePosition,
			const ZMath::vec2& InLastscreenSpacePosition,
			const std::set<FKey>& InPressedButtons,
			FKey InEffectingButton,
			float InWheelDelta,
			const FModifierKeysState& in_modifier_keys
		)
			: FInputEvent(in_modifier_keys, 0, false)
			, m_ScreenSpacePosition(InScreenSpacePosition)
			, m_LastScreenSpacePosition(InLastscreenSpacePosition)
			, m_CursorDelta(InScreenSpacePosition - InLastscreenSpacePosition)
			, m_PressedButtons(&InPressedButtons)
			, m_EffectingButton(InEffectingButton)
			, m_PointerIndex(InPointerIndex)
		{}

		FPointerEvent(
			uint32_t InUserIndex,
			uint32_t InPointerIndex,
			const ZMath::vec2& InScreenSpacePosition,
			const ZMath::vec2& InLastscreenSpacePosition,
			const std::set<FKey>& InPressedButtons,
			FKey InEffectingButton,
			float InWheelDelta,
			const FModifierKeysState& in_modifier_keys
		)
			: FInputEvent(in_modifier_keys, InUserIndex, false)
			, m_ScreenSpacePosition(InScreenSpacePosition)
			, m_LastScreenSpacePosition(InLastscreenSpacePosition)
			, m_CursorDelta(InScreenSpacePosition - InLastscreenSpacePosition)
			, m_PressedButtons(&InPressedButtons)
			, m_EffectingButton(InEffectingButton)
			, m_PointerIndex(InPointerIndex)
		{}

		/*returns the position of the cursor in screen space*/
		const ZMath::vec2 get_screen_space_position() const { return m_ScreenSpacePosition; }

		template<typename PointerEventType>
		static PointerEventType make_translated_event(const PointerEventType& in_pointer_event, const FVirtualPointerPosition& virtual_position)
		{
			PointerEventType new_event = in_pointer_event;
			new_event.m_ScreenSpacePosition = virtual_position.m_CurrentCursorPosition;
			new_event.m_LastScreenSpacePosition = virtual_position.m_LastCursorPosition;

			return new_event;
		}
	private:
		ZMath::vec2 m_ScreenSpacePosition;//screen space position
		ZMath::vec2 m_LastScreenSpacePosition;
		ZMath::vec2 m_CursorDelta;


		const std::set<FKey>* m_PressedButtons;
		FKey m_EffectingButton;

		//todo:implement TSet<FKey>
		//todo:implement FKey
		uint32_t m_PointerIndex;

		//todo:implement other information and members
	};
}