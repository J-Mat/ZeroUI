// Copyright Epic Games, Inc. All Rights Reserved.

#include "Visibility.h"

namespace ZeroUI
{
	/* Static initialization
	*****************************************************************************/

	const EVisibility EVisibility::Visible(EVisibility::VIS_Visible);
	const EVisibility EVisibility::Collapsed(EVisibility::VIS_Collapsed);
	const EVisibility EVisibility::Hidden(EVisibility::VIS_Hidden);
	const EVisibility EVisibility::HitTestInvisible(EVisibility::VIS_HitTestInvisible);
	const EVisibility EVisibility::SelfHitTestInvisible(EVisibility::VIS_SelfHitTestInvisible);
	const EVisibility EVisibility::All(EVisibility::VIS_All);


	/* FVisibility interface
	*****************************************************************************/

	std::string EVisibility::ToString( ) const
	{
		static const std::string VisibleString("Visible");
		static const std::string HiddenString("Hidden");
		static const std::string CollapsedString("Collapsed");
		static const std::string HitTestInvisibleString("HitTestInvisible");
		static const std::string SelfHitTestInvisibleString("SelfHitTestInvisible");

		if (Value == VIS_Visible)
		{
			return VisibleString;
		}

		if (Value == VIS_Collapsed)
		{
			return CollapsedString;
		}

		if (Value == VIS_Hidden)
		{
			return HiddenString;
		}

		if (Value == VIS_HitTestInvisible)
		{
			return HitTestInvisibleString;
		}

		if (Value == VIS_SelfHitTestInvisible)
		{
			return SelfHitTestInvisibleString;
		}

		return std::string();
	}
}