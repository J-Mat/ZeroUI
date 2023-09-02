#pragma once
#include "Core.h"
#include "GenericPlatform/GenericWindow.h"

namespace ZeroUI
{
	/**
	 * A platform specific implementation of FNativeWindow.
	 *
	 * Native Windows provide platform-specific backing for and are always owned by an SWindow.
	 */
	class FWindowsWindow
		: public FGenericWindow
	{
	public:

	public:

		/** Destructor. */
		~FWindowsWindow();

		/** Create a new FWin32Window. */
		static TSharedRef<FWindowsWindow> Make();

		/**
		 * Gets the Window's handle.
		 *
		 * @return The window's HWND handle.
		 */
		HWND GetHWnd() const;

		void Initialize( class FWindowsApplication* const Application, const TSharedRef<FGenericWindowDefinition>& InDefinition, HINSTANCE InHInstance, const TSharedPtr<FWindowsWindow>& InParent, const bool bShowImmediately );

		/**
		* Sets the window text (usually the title but can also be text content for things like controls).
		*
		* @param Text The window's title or content text
		*/
		bool IsRegularWindow() const;

		/**
		 * Sets the window region to specified dimensions.
		 *
		 * @param Width The width of the window region (in pixels).
		 * @param Height The height of the window region (in pixels).
		 */
		void AdjustWindowRegion(int32 Width, int32 Height);

		/** @return	Gives the native window a chance to adjust our stored window size before we cache it off. */
		virtual void AdjustCachedSize(FVector2D& Size) const override;

		virtual float GetDPIScaleFactor() const override
		{
			return DPIScaleFactor;
		}

		virtual void SetDPIScaleFactor(float Value) override
		{
			DPIScaleFactor = Value;
		}

		/** determines whether or not this window does its own DPI management */
		virtual bool IsManualManageDPIChanges() const override;

		/** call with a true argument if this window need to do its custom size management in response to DPI variations */
		virtual void SetManualManageDPIChanges(const bool bManualDPIChanges) override;

		/** Called when our parent window is minimized (which will in turn cause us to become minimized). */
		void OnParentWindowMinimized();

		/** Called when our parent window is restored (which will in turn cause us to become restored). */
		void OnParentWindowRestored();

		/** Called by the owning application when the level of transparency support has changed */
		void OnTransparencySupportChanged(EWindowTransparency NewTransparency);

		float GetAspectRatio() const { return AspectRatio; }

		/** @return True if the window is enabled */
		bool IsEnabled();

	public:
		// FGenericWindow interface
		virtual void* GetOSWindowHandle() const  override { return HWnd; }

	private:

		/** Protect the constructor; only TSharedRefs of this class can be made. */
		FWindowsWindow();

		void UpdateVisibility();


	private:

		/** The application that owns this window. */
		FWindowsApplication* OwningApplication;

		/** The window's handle. */
		HWND HWnd;

		/** Store the window region size for querying whether a point lies within the window. */
		int32 RegionWidth;
		int32 RegionHeight;

		/** The mode that the window is in (windowed, fullscreen, windowedfullscreen ) */
		EWindowMode::Type WindowMode;

		/** Current aspect ratio of window's client area */
		float AspectRatio;

		/** Whether the window is currently shown */
		bool bIsVisible;

		/** Whether the window is yet to have its first Show() call. This is set false after first Show(). */
		bool bIsFirstTimeVisible;

		/** We cache the min/max state for any Minimize, Maximize, or Restore calls that were made before the first Show */
		bool bInitiallyMinimized;
		bool bInitiallyMaximized;

		/**
		 * Ratio of pixels to SlateUnits in this window.
		 * E.g. DPIScale of 2.0 means there is a 2x2 pixel square for every 1x1 SlateUnit.
		 */
		float DPIScaleFactor;

		/** when true the window is responsible for its own size adjustments in response to a DPI change */
		bool bHandleManualDPIChanges = false;
	};
}