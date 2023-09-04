#pragma once

#include "Core.h"

namespace ZeroUI
{
	class FGenericWindow;

	namespace EMouseButtons
	{
		enum Type
		{
			Left = 0,
			Middle,
			Right,
			Thumb01,
			Thumb02,

			Invalid
		};
	}

	/*interface that defines how to handle interaction with a user via hardware input and output*/
	class FGenericApplicationMessageHandler
	{
	public:

		virtual ~FGenericApplicationMessageHandler() {}

		virtual bool ShouldProcessUserInputMessages( const Ref< FGenericWindow >& PlatformWindow ) const
		{
			return false;
		}

		virtual bool OnKeyChar( const TCHAR Character, const bool IsRepeat )
		{
			return false;
		}

		virtual bool OnKeyDown( const int32_t KeyCode, const uint32_t CharacterCode, const bool IsRepeat ) 
		{
			return false;
		}

		virtual bool OnKeyUp( const int32_t KeyCode, const uint32_t CharacterCode, const bool IsRepeat )
		{
			return false;
		}

		virtual void OnInputLanguageChanged()
		{
		}

		virtual bool OnMouseDown( const Ref< FGenericWindow >& Window, const EMouseButtons::Type Button )
		{
			return false;
		}

		virtual bool OnMouseDown( const Ref< FGenericWindow >& Window, const EMouseButtons::Type Button, const ZMath::vec2 CursorPos )
		{
			return false;
		}

		virtual bool OnMouseUp( const EMouseButtons::Type Button )
		{
			return false;
		}

		virtual bool OnMouseUp( const EMouseButtons::Type Button, const ZMath::vec2 CursorPos )
		{
			return false;
		}

		virtual bool OnMouseDoubleClick( const Ref< FGenericWindow >& Window, const EMouseButtons::Type Button )
		{
			return false;
		}

		virtual bool OnMouseDoubleClick( const Ref< FGenericWindow >& Window, const EMouseButtons::Type Button, const ZMath::vec2 CursorPos )
		{
			return false;
		}

		virtual bool OnMouseWheel( const float Delta )
		{
			return false;
		}

		virtual bool OnMouseWheel( const float Delta, const ZMath::vec2 CursorPos )
		{
			return false;
		}

		virtual bool OnMouseMove()
		{
			return false;
		}

		virtual bool OnRawMouseMove( const int32_t X, const int32_t Y )
		{
			return false;
		}

		virtual bool OnCursorSet()
		{
			return false;
		}
	};
}
