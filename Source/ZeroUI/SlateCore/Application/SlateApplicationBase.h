#pragma once

#include "Core.h"

namespace ZeroUI
{
	class FSlateRenderer;
	/**
	 * Base class for Slate applications.
	 *
	 * This class currently serves a temporary workaround for solving SlateCore dependencies to FSlateApplication.
	 * It should probably be removed once FSlateApplication has been refactored into SlateCore.
	 */
	class FSlateApplicationBase
	{
		friend class SWidget;
	public:

		FSlateApplicationBase();
		virtual ~FSlateApplicationBase() { }

	};	
}
