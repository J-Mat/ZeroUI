#pragma once

#include "Core.h"
#include "ApplicationCore/GenericPlatform/GenericApplicationMessageHandler.h"
#include "SlateCore/Application/SlateApplicationBase.h"

namespace ZeroUI
{
	class FSlateApplication
	: public FSlateApplicationBase
	, public FGenericApplicationMessageHandler
	{
	public:

		/** Virtual destructor. */
		virtual ~FSlateApplication();
	};
}