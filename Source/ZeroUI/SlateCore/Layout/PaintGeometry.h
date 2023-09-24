#pragma once

#include "Core.h"
#include "SlateCore/Rendering/SlateRenderTransform.h"
#include "SlateCore/Rendering/SlateLayoutTransform.h"

/**
 * A Paint geometry contains the window-space (draw-space) info to draw an element on the screen.
 * 
 * It contains the size of the element in the element's local space along with the 
 * transform needed to position the element in window space.
 * 
 * DrawPosition/DrawSize/DrawScale are maintained for legacy reasons, but are deprecated:
 * 
 *		The DrawPosition and DrawSize are already positioned and
 *		scaled for immediate consumption by the draw code.
 *
 *		The DrawScale is only applied to the internal aspects of the draw primitives. e.g. Line thickness, 3x3 grid margins, etc.
 */

namespace ZeroUI
{
	struct FPaintGeometry
	{
		/**
		 * !!! DEPRECATED!!! Drawing should only happen in local space to ensure render transforms work.
		 *	WARNING: This legacy member represents is LAYOUT space, and does not account for render transforms.
		*
		* Render-space position at which we will draw.
		*
		*
		*/
		ZMath::vec2 m_draw_position;

		/**
		 * !!! DEPRECATED!!! Drawing should only happen in local space to ensure render transforms work.
		 *	WARNING: This legacy member represents is LAYOUT space, and does not account for render transforms.
		*
		* Only affects the draw aspects of individual draw elements. e.g. line thickness, 3x3 grid margins 
		*/
		float m_draw_scale;

		/** Get the Size of the geometry in local space. Must call CommitTransformsIfUsingLegacyConstructor() first if legacy ctor is used. */
		const ZMath::vec2& GetLocalSize() const { return m_local_size; }

		/*Access the final render transform, must call commit transforms if using legacy constructor() first if legacy ctor is used*/
		const FSlateRenderTransform& GetAccumulatedRenderTransform() const { return AccumulatedRenderTransform; }

		/*
		* support mutable geometries constructed in window space, and possibly mutated later, as all legacy members are public
		* in these cases we defer creating of the render transform and local size until rendering time to ensure that all member changes have
		* finished
		* waning : legacy usage does not support render transforms
		*/
		void CommitTransformsIfUsingLegacyConstructor() const
		{
			if (!bUsingLegacyConstructor) return;

			AccumulatedRenderTransform = FSlateRenderTransform(m_draw_scale, m_draw_position);

			//todo:implement FSlateLayoutTransform's other function
			//FSlateLayoutTransform accumulated_layout_transform = FSlateLayoutTransform(m_draw_scale, glm::vec2(m_draw_position));
			//m_local_size =
			//todo:implement TransformVector function
		}

		bool HasRenderTransform() const { return m_b_has_render_transform; }

	private:
		/*mutable to support legacy constructors, doesn't account for render transforms*/
		mutable glm::vec2 m_draw_size;

		/*mutable to support legacy constructors*/
		mutable glm::vec2 m_local_size;

		/*
		* final render transform for drawing, transform from local space to window space for the draw element
		* mutable to support legacy constructors
		*/
		mutable FSlateRenderTransform AccumulatedRenderTransform;

		//support legacy constructors
		uint8_t bUsingLegacyConstructor : 1;

		uint8_t m_b_has_render_transform : 1;

		public:
		/*default ctor*/
		FPaintGeometry()
			: m_draw_position(0.0f, 0.0f)
			, m_draw_scale(1.0f)
			, m_draw_size(0.0f, 0.0f)
			, m_local_size(0.0f, 0.0f)
			, AccumulatedRenderTransform()
			, bUsingLegacyConstructor(true)
			, m_b_has_render_transform(false)
		{
		}

		/*
		 * creates and initializes a new instance
		 *
		 * @param InLocalSize the size in local space
		 * @param InAccumulatedLayoutTransform the accumulated layout transform(from an FGeometry)
		 * @param InAccumulatedRenderTransform the accumulated render transform(from an FGeometry)
		 */
		FPaintGeometry(const FSlateLayoutTransform& in_accumulated_layout_transform, const FSlateRenderTransform& in_accumulated_render_transform, const glm::vec2& in_local_size, bool b_in_has_render_transform)
			: m_draw_position(in_accumulated_layout_transform.GetTranslation())//absoulte position
			, m_draw_scale(in_accumulated_layout_transform.GetScale())//scale
			, m_draw_size(0.0f, 0.0f)//todo:??, why this is 0.0
			, m_local_size(in_local_size)
			, AccumulatedRenderTransform(in_accumulated_render_transform)
			, bUsingLegacyConstructor(false)
			, m_b_has_render_transform(b_in_has_render_transform)
		{}

		/*deprecated, this is legacy and should be removed*/
		FPaintGeometry(glm::vec2 in_draw_position, glm::vec2 in_draw_size, float in_draw_scale)
			: m_draw_position(in_draw_position)
			, m_draw_scale(in_draw_scale)
			, m_draw_size(in_draw_size)
			, m_local_size(0.0f, 0.0f)
			, AccumulatedRenderTransform()
			, bUsingLegacyConstructor(true)
			, m_b_has_render_transform(false)
		{
		}

		/*
		* special case method to append a layout transform to paint geometry
		* this is used in cases where the FGeometry was arranged in desktop space
		* and we need to undo the root desktop translation to get into window space
		* if you find yourself wanting to use this function, ask someone if there's a better way
		* 
		* @param LayoutTransform an additional layout transform to append to this paint geometry
		*/
		void AppendTransform(const FSlateLayoutTransform& layout_transform)
		{
		}

		/*
		* special case method to replace the render transform on a paint geometry
		* 
		* @param RenderTransform an additional layout transform to append to this paint geometry
		*/
		void SetRenderTransform(const FSlateRenderTransform& render_transform)
		{
			AccumulatedRenderTransform = render_transform;
		}
	};
}