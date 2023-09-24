#pragma once

#include "ZMath.h"

namespace ZeroUI
{
	/**
	 * Inverts a transform from space A to space B so it transforms from space B to space A.
	 * By default attempts to call a member function on the transform type.
	 * 
	 * @param Transform Input transform from space A to space B.
	 * @return Inverted transform from space B to space A.
	 */
	template<typename TransformType>
	inline auto Inverse(const TransformType& transform) -> decltype(transform.Inverse())
	{
		return transform.Inverse();//call member function
	}

	/**
	 * Inverts a transform from space A to space B so it transforms from space B to space A.
	 * Specialization for uniform scale.
	 * 
	 * @param Transform Input transform from space A to space B.
	 * @return Inverted transform from space B to space A.
	 */
	inline float Inverse(float Scale)
	{
		return 1.0f / Scale;
	}

	inline ZMath::vec2 Inverse(const ZMath::vec2& Transform)
	{
		return -Transform;
	}

	/**
	 * Generic implementation of TransformPoint. Attempts to use a member function of the TransformType.
	 */
	template <typename TransformType, typename PositionType>
	inline PositionType TransformPoint(const TransformType& Transform, const PositionType& Point)
	{
		return Transform.TransformPoint(Point);
	}

	inline ZMath::vec2 TransformPoint(const ZMath::vec2& Transform, const ZMath::vec2& Point)
	{
		return Transform + Point;
	}

	inline ZMath::vec2 TransformPoint(float Transform, const ZMath::vec2& Point)
	{
		return Transform * Point;
	}

	/**
	 * Generic implementation of TransformVector. Attempts to use a member function of the TransformType.
	 */
	template <typename TransformType, typename VectorType>
	inline VectorType TransformVector(const TransformType& Transform, const VectorType& Vector)
	{
		return Transform.TransformVector(Vector);
	}

	inline const ZMath::vec2& TransformVector(const ZMath::vec2& Transform, const ZMath::vec2& Vector)
	{
		return Vector;
	}

	inline ZMath::vec2 TransformVector(float Transform, const ZMath::vec2& Vector)
	{
		return Transform * Vector;
	}

	/** Provides default logic (used by TransformCast) to convert one transform type to another via a conversion ctor. */
	template<class TransformType>
	struct TransformConverter
	{
		/** Efficient NULL conversion. */
		static const TransformType& Convert(const TransformType& Transform)
		{
			return Transform;
		}
		/** 
		 * Default Conversion via a conversion ctor.
		 * Note we are not using perfect forwarding here. Our types don't generally support move operations, nor do they make sense.
		 * VS 2013 seems to have trouble resolving the specializations below in the presence of perfect forwarding semantics.
		 */
		template<typename OtherTransformType>
		static TransformType Convert(const OtherTransformType& Transform)
		{
			return TransformType(Transform);
		}
	};

	/**
	 * Casts one TransformType to ResultType using rules laid out by TransformConverter<>::Convert<>().
	 * 
	 * Return type uses decltype to support classes that can return by const-ref more efficiently than returning a new value.
	 */
	template<typename ResultType, typename TransformType>
	inline auto TransformCast(const TransformType& Transform) -> decltype(TransformConverter<ResultType>::Convert(Transform))
	{
		return TransformConverter<ResultType>::Convert(Transform);
	}

	/**
	 * Provides default rules defining the result of concatenating two types. By default, nothing is supported
	* because the code cannot know in general what two types result in after concatenation.
	*/
	template<typename TransformTypeA, typename TransformTypeB>
	struct ConcatenateRules
	{
	};
	/** Partial specialization for concatenating two of the same types. Always results in the same type being returned. */
	template<typename TransformType>
	struct ConcatenateRules<TransformType, TransformType>
	{
		typedef TransformType ResultType;
	};

	/**
	 * Concatenates two transforms. Uses TransformCast<> to convert them first.
	 * If more efficient means are available to concatenate two transforms, provide a non-template overload (or possibly a specialization).
	 * Concatenation is performed in left to right order, so the output space of LHS must match the input space of RHS.
	 *
	 * @param LHS Transformation that goes from space A to space B
	 * @param RHS Transformation that goes from space B to space C.
	 * @return a new transform representing the transformation from the input space of LHS to the output space of RHS.
	 */
	template<typename TransformTypeA, typename TransformTypeB>
	inline typename ConcatenateRules<TransformTypeA, TransformTypeB>::ResultType Concatenate(const TransformTypeA& LHS, const TransformTypeB& RHS)
	{
		typedef typename ConcatenateRules<TransformTypeA, TransformTypeB>::ResultType ReturnType;
		// If you get a compiler error here about "no member function Concatenate found for TransformType" you know
		// your transform type doesn't support a Concatenate method. Either add one or provide an overload of Concatenate that does this.
		return Concatenate(TransformCast<ReturnType>(LHS), TransformCast<ReturnType>(RHS));
	}

	template<typename TransformType>
	inline auto Concatenate(const TransformType& LHS, const TransformType& RHS) -> decltype(LHS.concatenate(RHS))
	{
		return LHS.Concatenate(RHS);
	}
}