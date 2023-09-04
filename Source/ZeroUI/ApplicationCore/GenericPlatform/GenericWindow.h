// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"

struct FGenericWindowDefinition;
namespace ZeroUI
{
	/**
	 * Modes that an FGenericWindow can be in
	 */
	namespace EWindowMode
	{
		enum Type
		{
			/** The window is in true fullscreen mode */
			Fullscreen,
			/** The window has no border and takes up the entire area of the screen */
			WindowedFullscreen,
			/** The window has a border and may not take up the entire screen area */
			Windowed,

			/** The total number of supported window modes */
			NumWindowModes
		};

		static inline Type ConvertIntToWindowMode(int32_t InWindowMode)
		{
			Type WindowMode = Windowed;
			switch (InWindowMode)
			{
			case 0:
				WindowMode = Fullscreen;
				break;
			case 1:
				WindowMode = WindowedFullscreen;
				break;
			case 2:
			default:
				WindowMode = Windowed;
				break;
			}
			return WindowMode;
		}
	}


	enum class EWindowDrawAttentionRequestType
	{
		/**
		 * Indicates that the attention-drawing behavior continues until the
		 * application or window is activated.
		 */
		UntilActivated,

		/**
		 * Indicates that the attention-drawing behavior, if any, should stop.
		 */
		Stop,
	};

	/**
	 * Parameters for drawing attention to a window or application. Some
	 * parameters may only be used on certain platforms.
	 */
	struct FWindowDrawAttentionParameters
	{
		FWindowDrawAttentionParameters() = default;

		explicit FWindowDrawAttentionParameters(
			EWindowDrawAttentionRequestType InRequestType
		)
			: RequestType(InRequestType)
		{
		}

		EWindowDrawAttentionRequestType RequestType = EWindowDrawAttentionRequestType::UntilActivated;
	};

	class FGenericWindow
	{
	public:

		FGenericWindow();

		virtual ~FGenericWindow();

		/** Gets the OS Window handle in the form of a void pointer for other API's */
		virtual void* GetOSWindowHandle() const;

		/**
		 * @return ratio of pixels to SlateUnits in this window.
		 * E.g. DPIScale of 2.0 means there is a 2x2 pixel square for every 1x1 SlateUnit.
		 */
		virtual float GetDPIScaleFactor() const;

		/** sets a new DPI scale factor */
		virtual void SetDPIScaleFactor(const float Factor);

	protected:

		Ref< FGenericWindowDefinition > m_Definition;
	};
}