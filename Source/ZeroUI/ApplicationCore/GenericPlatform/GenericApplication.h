
#include "Core.h"

namespace ZeroUI
{
	class FSlateApplication;
	class ICursor;

	/**
	* Enumerates available modifier keys for input gestures.
	*/
	namespace EModifierKey
	{
		typedef uint8 Type;

		/** No key. */
		const Type None	= 0;

		/** Ctrl key (Command key on Mac, Control key on Windows). */
		const Type Control = 1 << 0;

		/** Alt key. */
		const Type Alt = 1 << 1;

		/** Shift key. */
		const Type Shift = 1 << 2;

		/** Cmd key (Control key on Mac, Win key on Windows) */
		const Type Command = 1 << 3;

		FORCEINLINE EModifierKey::Type FromBools(const bool bControl, const bool bAlt, const bool bShift, const bool bCommand)
		{
			EModifierKey::Type ModifierMask = EModifierKey::None;
			if (bControl)	ModifierMask |= EModifierKey::Control;
			if (bAlt)		ModifierMask |= EModifierKey::Alt;
			if (bShift)		ModifierMask |= EModifierKey::Shift;
			if (bCommand)	ModifierMask |= EModifierKey::Command;

			return ModifierMask;
		}
	};


	namespace EPopUpOrientation
	{
		enum Type
		{
			Horizontal,
			Vertical,
		};
	}


	/**
	 * FModifierKeysState stores the pressed state of keys that are commonly used as modifiers
	 */
	class FModifierKeysState
	{

	public:

		/**
		 * Constructor.  Events are immutable once constructed.
		 *
		 * @param  bInIsLeftShiftDown  True if left shift is pressed
		 * @param  bInIsRightShiftDown  True if right shift is pressed
		 * @param  bInIsLeftControlDown  True if left control is pressed
		 * @param  bInIsRightControlDown  True if right control is pressed
		 * @param  bInIsLeftAltDown  True if left alt is pressed
		 * @param  bInIsRightAltDown  True if right alt is pressed
		 */
		FModifierKeysState( const bool bInIsLeftShiftDown,
							const bool bInIsRightShiftDown,
							const bool bInIsLeftControlDown,
							const bool bInIsRightControlDown,
							const bool bInIsLeftAltDown,
							const bool bInIsRightAltDown,
							const bool bInIsLeftCommandDown,
							const bool bInIsRightCommandDown,
							const bool bInAreCapsLocked)
			: m_bIsLeftShiftDown( bInIsLeftShiftDown ),
			m_bIsRightShiftDown( bInIsRightShiftDown ),
			m_bIsLeftControlDown( bInIsLeftControlDown ),
			m_bIsRightControlDown( bInIsRightControlDown ),
			m_bIsLeftAltDown( bInIsLeftAltDown ),
			m_bIsRightAltDown( bInIsRightAltDown ),
			m_bIsLeftCommandDown( bInIsLeftCommandDown ),
			m_bIsRightCommandDown( bInIsRightCommandDown ),
			m_bAreCapsLocked( bInAreCapsLocked )
		{
		}

		FModifierKeysState()
			: m_bIsLeftShiftDown(false),
			m_bIsRightShiftDown(false),
			m_bIsLeftControlDown(false),
			m_bIsRightControlDown(false),
			m_bIsLeftAltDown(false),
			m_bIsRightAltDown(false),
			m_bIsLeftCommandDown(false),
			m_bIsRightCommandDown(false),
			m_bAreCapsLocked(false)
		{
		}

		/**
		 * Returns true if either shift key was down when this event occurred
		 *
		 * @return  True if shift is pressed
		 */
		bool IsShiftDown() const
		{
			return m_bIsLeftShiftDown || m_bIsRightShiftDown;
		}

		/**
		 * Returns true if left shift key was down when this event occurred
		 *
		 * @return  True if left shift is pressed
		 */
		bool IsLeftShiftDown() const
		{
			return m_bIsLeftShiftDown;
		}

		/**
		 * Returns true if right shift key was down when this event occurred
		 *
		 * @return  True if right shift is pressed
		 */
		bool IsRightShiftDown() const
		{
			return m_bIsRightShiftDown;
		}

		/**
		 * Returns true if either control key was down when this event occurred
		 *
		 * @return  True if control is pressed
		 */
		bool IsControlDown() const
		{
			return m_bIsLeftControlDown || m_bIsRightControlDown;
		}

		/**
		 * Returns true if left control key was down when this event occurred
		 *
		 * @return  True if left control is pressed
		 */
		bool IsLeftControlDown() const
		{
			return m_bIsLeftControlDown;
		}

		/**
		 * Returns true if right control key was down when this event occurred
		 *
		 * @return  True if right control is pressed
		 */
		bool IsRightControlDown() const
		{
			return m_bIsRightControlDown;
		}

		/**
		 * Returns true if either alt key was down when this event occurred
		 *
		 * @return  True if alt is pressed
		 */
		bool IsAltDown() const
		{
			return m_bIsLeftAltDown || m_bIsRightAltDown;
		}

		/**
		 * Returns true if left alt key was down when this event occurred
		 *
		 * @return  True if left alt is pressed
		 */
		bool IsLeftAltDown() const
		{
			return m_bIsLeftAltDown;
		}

		/**
		 * Returns true if right alt key was down when this event occurred
		 *
		 * @return  True if right alt is pressed
		 */
		bool IsRightAltDown() const
		{
			return m_bIsRightAltDown;
		}

		/**
		 * Returns true if either command key was down when this event occurred
		 *
		 * @return  True if command is pressed
		 */
		bool IsCommandDown() const
		{
			return m_bIsLeftCommandDown || m_bIsRightCommandDown;
		}

		/**
		 * Returns true if left command key was down when this event occurred
		 *
		 * @return  True if left command is pressed
		 */
		bool IsLeftCommandDown() const
		{
			return m_bIsLeftCommandDown;
		}
		
		/**
		 * Returns true if right command key was down when this event occurred
		 *
		 * @return  True if right command is pressed
		 */
		bool IsRightCommandDown() const
		{
			return m_bIsRightCommandDown;
		}


		/**
		 * @return  true if the Caps Lock key has been toggled to the enabled state.
		 */
		bool AreCapsLocked() const
		{
			return m_bAreCapsLocked;
		}

		/**
		 * @param ModifierKeys the modifier keys to test to see if they are pressed.  Returns true if no modifiers are specified.
		 * @return true if all modifier keys are pressed specified in the modifier keys.
		 */
		bool AreModifersDown(EModifierKey::Type ModiferKeys) const
		{
			bool AllModifersDown = true;

			if ( (ModiferKeys & EModifierKey::Shift) == EModifierKey::Shift )
			{
				AllModifersDown &= IsShiftDown();
			}
			if ( (ModiferKeys & EModifierKey::Command) == EModifierKey::Command )
			{
				AllModifersDown &= IsCommandDown();
			}
			if ( (ModiferKeys & EModifierKey::Control) == EModifierKey::Control )
			{
				AllModifersDown &= IsControlDown();
			}
			if ( (ModiferKeys & EModifierKey::Alt) == EModifierKey::Alt )
			{
				AllModifersDown &= IsAltDown();
			}

			return AllModifersDown;
		}

		/**
		 * @return true if any modifier key is down
		 */
		bool AnyModifiersDown() const
		{
			return IsControlDown() || IsCommandDown() || IsShiftDown() || IsAltDown();
		}

	private:

		/** True if the left shift key was down when this event occurred. */
		bool m_bIsLeftShiftDown;

		/** True if the right shift key was down when this event occurred. */
		bool m_bIsRightShiftDown;

		/** True if the left control key was down when this event occurred. */
		bool m_bIsLeftControlDown;

		/** True if the right control key was down when this event occurred. */
		bool m_bIsRightControlDown;

		/** True if the left alt key was down when this event occurred. */
		bool m_bIsLeftAltDown;

		/** True if the right alt key was down when this event occurred. */
		bool m_bIsRightAltDown;

		/** True if the left command key was down when this event occurred. */
		bool m_bIsLeftCommandDown;

		/** True if the right command key was down when this event occurred. */
		bool m_bIsRightCommandDown;

		/** True if the Caps Lock key has been toggled to the enabled state. */
		bool m_bAreCapsLocked;
	};

	/**
	 * Generic platform application interface
	*/
	class GenericApplication 
	{
	public:

	GenericApplication( const Ref< ICursor >& InCursor )
		: m_Cursor( InCursor )
		, m_MessageHandler( MakeShareable( new FGenericApplicationMessageHandler() ) )
	{

	}
	virtual ~GenericApplication() {}

	virtual void SetMessageHandler( const Ref< FGenericApplicationMessageHandler >& InMessageHandler ) { m_MessageHandler = InMessageHandler; }

	Ref< FGenericApplicationMessageHandler > GetMessageHandler() { return m_MessageHandler; }

	virtual void PollGameDeviceState( const float TimeDelta ) { }

	virtual void PumpMessages( const float TimeDelta ) { }

	virtual void ProcessDeferredEvents( const float TimeDelta ) { }

	virtual void Tick ( const float TimeDelta ) { }

	virtual Ref< FGenericWindow > MakeWindow() { return MakeShareable( new FGenericWindow() ); }

	virtual void InitializeWindow( const Ref< FGenericWindow >& Window, const Ref< FGenericWindowDefinition >& InDefinition, const Ref< FGenericWindow >& InParent, const bool bShowImmediately ) { }

	virtual void SetCapture( const Ref< FGenericWindow >& InWindow ) { }

	virtual void* GetCapture( void ) const { return NULL; }

	virtual FModifierKeysState GetModifierKeys() const  { return FModifierKeysState(); }

	/** @return true if the system cursor is currently directly over a slate window. */
	virtual bool IsCursorDirectlyOverSlateWindow() const { return true; }

	/** @return Native window under the mouse cursor. */
	virtual Ref< FGenericWindow > GetWindowUnderCursor() { return Ref< FGenericWindow >( nullptr ); }

	virtual bool IsMinimized() const { return false; }

	virtual void SetHighPrecisionMouseMode( const bool Enable, const Ref< FGenericWindow >& InWindow ) { };

	virtual bool IsUsingHighPrecisionMouseMode() const { return false; }

	virtual FPlatformRect GetWorkArea( const FPlatformRect& CurrentWindow ) const
	{
		FPlatformRect OutRect;
		OutRect.Left = 0;
		OutRect.Top = 0;
		OutRect.Right = 0;
		OutRect.Bottom = 0;

		return OutRect;
	}
public:

	const Ref< ICursor > m_Cursor;

protected:

	Ref< class FGenericApplicationMessageHandler > m_MessageHandler;
};
}
