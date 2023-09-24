#pragma once

#include <string>

namespace ZeroUI
{
	struct FKey
	{
		FKey()
		{
		}

		FKey(const std::string& InName)
			: m_KeyName(InName)
		{}

	private:
		std::string m_KeyName;
	};

	struct EKeys
	{
		static const FKey Invalid;

		static const FKey LeftMouseButton;
		static const FKey RightMouseButton;
		static const FKey MiddleMouseButton;
		static const FKey ThumbMouseButton;
		static const FKey ThumbMouseButton2;
	};
} // namespace 
