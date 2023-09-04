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

		/** Called by a window when an OLE Drag and Drop operation occurred on a non-game thread */
		void DeferDragDropOperation(const FDeferredWindowsDragDropOperation& DeferredDragDropOperation);


		/**
		 * Adds a Windows message handler with the application instance.
		 *
		 * @param MessageHandler The message handler to register.
		 * @see RemoveMessageHandler
		 */
		virtual void AddMessageHandler(IWindowsMessageHandler& InMessageHandler);

		/**
		 * Removes a Windows message handler with the application instance.
		 *
		 * @param MessageHandler The message handler to register.
		 * @see AddMessageHandler
		 */
		virtual void RemoveMessageHandler(IWindowsMessageHandler& InMessageHandler);

	public:

		// GenericApplication overrides

		virtual void SetMessageHandler(const TSharedRef< class FGenericApplicationMessageHandler >& InMessageHandler) override;
#if WITH_ACCESSIBILITY
		virtual void SetAccessibleMessageHandler(const TSharedRef<FGenericAccessibleMessageHandler>& InAccessibleMessageHandler) override;
#endif
		virtual void PumpMessages(const float TimeDelta) override;
		virtual void ProcessDeferredEvents(const float TimeDelta) override;
		virtual TSharedRef< FGenericWindow > MakeWindow() override;
		virtual void InitializeWindow(const TSharedRef< FGenericWindow >& Window, const TSharedRef< FGenericWindowDefinition >& InDefinition, const TSharedPtr< FGenericWindow >& InParent, const bool bShowImmediately) override;

	protected:
		friend LRESULT WindowsApplication_WndProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

		/** Windows callback for message processing (forwards messages to the FWindowsApplication instance). */
		static LRESULT CALLBACK AppWndProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

		/** Processes a single Windows message. */
		int32_t ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam);

		/** Processes a deferred Windows message. */
		int32 ProcessDeferredMessage(const FDeferredWindowsMessage& DeferredMessage);

		/** Processes deferred drag and drop operations. */
		void ProcessDeferredDragDropOperation(const FDeferredWindowsDragDropOperation& Op);

		/** Hidden constructor. */
		FWindowsApplication(const HINSTANCE HInstance, const HICON IconHandle);

		void ApplyLowLevelMouseFilter();
		void RemoveLowLevelMouseFilter();

		static LRESULT CALLBACK HandleLowLevelMouseFilterHook(int nCode, WPARAM wParam, LPARAM lParam);

		HHOOK LowLevelMouseFilterHook;
		bool bLowLevelMouseFilterIsApplied = false;

	private:

		/** Registers the Windows class for windows and assigns the application instance and icon */
		static bool RegisterClass(const HINSTANCE HInstance, const HICON HIcon);

		/** Defers a Windows message for later processing. */
		void DeferMessage(TSharedPtr<FWindowsWindow>& NativeWindow, HWND InHWnd, uint32 InMessage, WPARAM InWParam, LPARAM InLParam, int32 MouseX = 0, int32 MouseY = 0, uint32 RawInputFlags = 0);


		HINSTANCE InstanceHandle;

		bool bMinimized;

		bool bUsingHighPrecisionMouseInput;

		bool bIsMouseAttached;

		bool bForceActivateByMouse;

		bool bForceNoGamepads;

		bool bConsumeAltSpace;

		std::vector<FDeferredWindowsMessage> DeferredMessages;


		/** Registered Windows message handlers. */
		std::vector<IWindowsMessageHandler*> MessageHandlers;

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