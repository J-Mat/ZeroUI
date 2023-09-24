#pragma once

#include "Core.h"
#include "ApplicationCore/GenericPlatform/GenericApplication.h"


namespace ZeroUI
{
	class FGenericWindow;
	class FWindowsWindow;
	class FGenericApplicationMessageHandler;

	struct FDeferredWindowsMessage
	{
		FDeferredWindowsMessage(const Ref<FWindowsWindow>& InNativeWindow, HWND InHWnd, uint32_t InMessage, WPARAM InWParam, LPARAM InLParam, int32_t InX = 0, int32_t InY = 0, uint32_t InRawInputFlags = 0)
			: NativeWindow(InNativeWindow)
			, hWND(InHWnd)
			, Message(InMessage)
			, wParam(InWParam)
			, lParam(InLParam)
			, X(InX)
			, Y(InY)
			, RawInputFlags(InRawInputFlags)
		{ }

		/** Native window that received the message */
		Weak<FWindowsWindow> NativeWindow;

		/** Window handle */
		HWND hWND;

		/** Message code */
		uint32_t Message;

		/** Message data */
		WPARAM wParam;
		LPARAM lParam;

		/** Mouse coordinates */
		int32_t X;
		int32_t Y;
		uint32_t RawInputFlags;
	};

	/**
 * Windows-specific application implementation.
 */
	class FWindowsApplication
		: public GenericApplication
	{
	public:

		/**
		 * Static: Creates a new Win32 application
		 *
		 * @param InstanceHandle Win32 instance handle.
		 * @param IconHandle Win32 application icon handle.
		 * @return New application object.
		 */
		static FWindowsApplication* CreateWindowsApplication(const HINSTANCE InstanceHandle, const HICON IconHandle);

		/** Virtual destructor. */
		virtual ~FWindowsApplication();

	public:
		virtual void PumpMessages(const float TimeDelta) override;
		virtual void ProcessDeferredEvents(const float TimeDelta) override;
		virtual Ref< FGenericWindow > MakeWindow() override;
		virtual void InitializeWindow(const Ref< FGenericWindow >& Window, const Ref< FGenericWindowDefinition >& InDefinition, const Ref< FGenericWindow >& InParent, const bool bShowImmediately) override;

	protected:
		friend LRESULT WindowsApplication_WndProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

		/** Windows callback for message processing (forwards messages to the FWindowsApplication instance). */
		static LRESULT CALLBACK AppWndProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

		/** Processes a single Windows message. */
		int32_t ProcessMessage(HWND hwnd, uint32_t  msg, WPARAM wParam, LPARAM lParam);

		/** Processes a deferred Windows message. */
		int32_t  ProcessDeferredMessage(const FDeferredWindowsMessage& DeferredMessage);

		/** Hidden constructor. */
		FWindowsApplication(const HINSTANCE HInstance, const HICON IconHandle);

	private:

		/** Registers the Windows class for windows and assigns the application instance and icon */
		static bool RegisterClass(const HINSTANCE HInstance, const HICON HIcon);

		/** Defers a Windows message for later processing. */
		void DeferMessage(Ref<FWindowsWindow>& NativeWindow, HWND InHWnd, uint32_t  InMessage, WPARAM InWParam, LPARAM InLParam, int32_t  MouseX = 0, int32_t  MouseY = 0, uint32_t  RawInputFlags = 0);


		HINSTANCE m_InstanceHandle;

		bool bMinimized;

		bool bUsingHighPrecisionMouseInput;

		bool bIsMouseAttached;

		bool bForceActivateByMouse;

		bool bForceNoGamepads;

		bool bConsumeAltSpace;

		std::vector<FDeferredWindowsMessage> DeferredMessages;

		std::vector<Ref<FWindowsWindow>> Windows;

		struct EModifierKey
		{
			enum Type
			{
				LeftShift,		// VK_LSHIFT
				RightShift,		// VK_RSHIFT
				LeftControl,	// VK_LCONTROL
				RightControl,	// VK_RCONTROL
				LeftAlt,		// VK_LMENU
				RightAlt,		// VK_RMENU
				CapsLock,		// VK_CAPITAL
				Count,
			};
		};
		/** Cached state of the modifier keys. True if the modifier key is pressed (or toggled in the case of caps lock), false otherwise */
		bool ModifierKeyState[EModifierKey::Count];

		int32_t bAllowedToDeferMessageProcessing;

		/** True if we are in the middle of a windows modal size loop */
		bool bInModalSizeLoop;
	};
}