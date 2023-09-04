#pragma once
#include "Core.h"
#include "ApplicationCore/GenericPlatform/GenericWindow.h"

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

		/** Destructor. */
		~FWindowsWindow();

		/** Create a new FWin32Window. */
		static Ref<FWindowsWindow> Make();

		/**
		 * Gets the Window's handle.
		 *
		 * @return The window's HWND handle.
		 */
		HWND GetHWnd() const;

		void Initialize( class FWindowsApplication* const Application, const Ref<FGenericWindowDefinition>& InDefinition, HINSTANCE InHInstance, const Ref<FWindowsWindow>& InParent, const bool bShowImmediately );


		virtual float GetDPIScaleFactor() const override
		{
			return m_DPIScaleFactor;
		}

		virtual void SetDPIScaleFactor(float Value) override
		{
			m_DPIScaleFactor = Value;
		}


		float GetAspectRatio() const { return m_AspectRatio; }

	public:
		// FGenericWindow interface
		virtual void* GetOSWindowHandle() const  override { return m_HWnd; }

	private:

		/** Protect the constructor; only TSharedRefs of this class can be made. */
		FWindowsWindow();



	private:
		/** The application that owns this window. */
		FWindowsApplication* m_OwningApplication;

		/** The window's handle. */
		HWND m_HWnd;

		/** Store the window region size for querying whether a point lies within the window. */
		int32_t m_RegionWidth;
		int32_t m_RegionHeight;

		/** The mode that the window is in (windowed, fullscreen, windowedfullscreen ) */
		EWindowMode::Type m_WindowMode;

		/** Current aspect ratio of window's client area */
		float m_AspectRatio;

		/**
		 * Ratio of pixels to SlateUnits in this window.
		 * E.g. DPIScale of 2.0 means there is a 2x2 pixel square for every 1x1 SlateUnit.
		 */
		float m_DPIScaleFactor;

		/** when true the window is responsible for its own size adjustments in response to a DPI change */
		bool bHandleManualDPIChanges = false;
	};
}