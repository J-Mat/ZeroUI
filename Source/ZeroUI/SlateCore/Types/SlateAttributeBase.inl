#pragma onces

namespace SlateAttributePrivate
{
	//ContainerType is SWidget
	//InObjectType is color or scale etc
	//InComparePredicateType is TSlateAttributeComparePredicate
	template<typename ContainerType, typename InObjectType, typename InInvalidationReasonPredicate, typename InComparePredicateType,
	ESlateAttributeType InAttributeType>
	struct TSlateAttributeBase : public FSlateAttributeImpl
	{
	public:
		template<typename AttributeMemberType>
		friend struct TSlateMemberAttributeRef;

		using ObjectType = InObjectType;
		using FInvalidationReasonPredicate = InInvalidationReasonPredicate;
		using FGetter = typename TAttribute<ObjectType>::FGetter;//there is core, have a delegate
		using FComparePredicate = InComparePredicateType;

		static EInvalidateWidgetReason GetInvalidationReason(const SWidget& Widget) { return FInvalidationReasonPredicate::GetInvalidationReason(Widget); }
		static EInvalidateWidgetReason GetInvalidationReason(const ISlateAttributeContainer& Container) { return FInvalidationReasonPredicate::GetInvalidationReason(Container.GetContainer_Widget()); }
		//use == to compare LHS and RHS
		static bool IdenticalTo(const SWidget& Widget, const ObjectType& LHS, const ObjectType& RHS) { return FComparePredicate::Identical_To(Widget, LHS, RHS); }
		static bool IdenticalTo(const ISlateAttributeContainer& Container, const ObjectType& LHS, const ObjectType& RHS) { return FComparePredicate::Identical_To(Container.GetContainerWidget(), LHS, RHS); }
	public:
		TSlateAttributeBase()
			: m_Value()
		{
		}

		TSlateAttributeBase(const ObjectType& InValue)
			: m_Value(InValue)
		{
		}

		TSlateAttributeBase(ObjectType&& InValue)
			: m_Value(std::move(InValue))
		{
		}
		TSlateAttributeBase(SWidget& Widget, const ObjectType& InValue)
			: m_Value(InValue)
		{
		}

		TSlateAttributeBase(SWidget& Widget, ObjectType&& InValue)
			: m_Value(std::move(InValue))
		{
		}

	public:
		//return slate attribute cached value, if the slate attribute is bound, the value will be cached at the end of the every frame
		const ObjectType& Get() const
		{
			return m_Value;
		}

		/*
		 * unbind the slate attribute and set it's value, it may invalidate the Widget if the value is different
		 * return true if the value is considered different and an invalidation occurred
		 */
		bool Set(ContainerType& Widget, const ObjectType& NewValue)
		{
			const bool bIsIdentical = IdenticalTo(Widget, m_Value, NewValue);

			//unregister attribute and mark different flag on Widget
			ProtectedUnregisterAttribute(Widget, InAttributeType);

			if(!bIsIdentical)
			{
				m_Value = NewValue;
				ProtectedInvalidateWidget(Widget, InAttributeType, GetInvalidationReason(Widget));
			}

			return !bIsIdentical;
		}

		/*
		 * unbind the slate attribute and set it's value, it may invalidate the widget if the value is different
		 * return true if the value is considered different an invalidation occurred
		 */
		bool Set(ContainerType& widget, ObjectType&& new_value)
		{
			const bool b_Is_Identical = Identical_To(widget, m_Value, new_value);//check equal
			//todo:implement ProtectedUnregisterAttribute
			//may be register some getter, need to unbind
			protected_unregister_attribute(widget, InAttributeType);
			if(!b_Is_Identical)
			{
				m_Value = std::move(m_Value);
				//todo:implement Protected Invalidate Widget
			}

			return !b_Is_Identical;
		}

		bool Assign(ContainerType& widget, const TAttribute<ObjectType>& OtherAttribute)
		{
			//OtherAttribute bind a function
			if (OtherAttribute.IsBound())
			{
				return AssignBinding(widget, OtherAttribute.Get_Binding());
			}//OtherAttribute does not bind a function, but assign a value
			else if (OtherAttribute.IsSet())
			{
				return Set(widget, OtherAttribute.Get());
			}
			else
			{
				return false;
			}
		}

		//from TAttribute to construct TSlateAttributeBase
		bool Assign(ContainerType& widget, TAttribute<ObjectType>&& OtherAttribute)
		{
			//OtherAttribute bind a function
			if (OtherAttribute.IsBound())
			{
				return AssignBinding(widget, OtherAttribute.Get_Binding());
			}//OtherAttribute does not bind a function, but assign a value
			else if(OtherAttribute.IsSet())
			{
				return Set(widget, OtherAttribute.Get());
			}
			else
			{
				return false;
			}
		}

	private:
		bool AssignBinding(ContainerType& Widget, const FGetter& Getter)
		{
			//from widget meta data(FSlateAttributeMetaData) to query whether this slate attribute is registered?
			//compare getter
			const FDelegateHandle Previous_Getter_Handler = ProtectedFindGetterHandle(Widget, InAttributeType);

			//if(Previous_Getter_Handler != getter.GetHandle())
			//{
			//	//todo:write a construct wrapper
			//	return true;
			//}

			if (Previous_Getter_Handler != Getter.GetHandle())
			{
				ConstructWrapper(Widget, std::move(Getter));

				return true;
			}

			return false;
		}

		bool AssignBinding(ContainerType& widget, FGetter&& getter)
		{
		}

		void ConstructWrapper(ContainerType& widget, const FGetter& getter)
		{
			//make a FSlateAttributeGetterWrapper<TSlateAttributeBase>
			std::unique_ptr<ISlateAttributeGetter> wrapper = Make_Unique_Getter(*this, getter);
			ProtectedRegisterAttribute(widget, InAttributeType, std::move(wrapper));
			//update now on bind
		}

		template<typename SlateAttributeType>
		class FSlateAttributeGetterWrapper : public ISlateAttributeGetter
		{
		public:
			using ObjectType = typename SlateAttributeType::ObjectType;
			using FGetter = typename TAttribute<ObjectType>::FGetter;
			using FComparePredicate = typename SlateAttributeType::FComparePredicate;

			FSlateAttributeGetterWrapper() = delete;

			FSlateAttributeGetterWrapper(const FSlateAttributeGetterWrapper&) = delete;

			FSlateAttributeGetterWrapper& operator = (const FSlateAttributeGetterWrapper&) = delete;

			virtual ~FSlateAttributeGetterWrapper() override = default;

		public:
			FSlateAttributeGetterWrapper(SlateAttributeType& InOwningAttribute, const FGetter& InGetterDelegate)
				: m_Getter(InGetterDelegate)
				, m_Attribute(&InOwningAttribute)
			{
			}

			FSlateAttributeGetterWrapper(SlateAttributeType& InOwningAttribute, FGetter& InGetterDelegate)
				: m_Getter(std::move(InGetterDelegate))
				, m_Attribute(&InOwningAttribute)
			{
			}

			virtual FDelegateHandle GetDelegateHandle() const override
			{
				return m_Getter.GetHandle();
			}

			FUpdateAttributeResult UpdateAttribute(const SWidget& Widget) override
			{
				return FUpdateAttributeResult();
			}

			const FSlateAttributeBase& GetAttribute() const override
			{
				return *m_Attribute;
			}

			void SetAttribute(FSlateAttributeBase&) override
			{
				//
			}

		private:
			//getter function to fetch the new value of the slate attribute
			FGetter m_Getter;
			//the slate attribute of the sWidget owning the value
			SlateAttributeType* m_Attribute;
		};
	private:
		static std::unique_ptr<ISlateAttributeGetter> MakeUniqueGetter(TSlateAttributeBase& Attribute, const FGetter& Getter)
		{
			return std::make_unique<FSlateAttributeGetterWrapper<TSlateAttributeBase>>(Attribute, Getter);
		}

		static std::unique_ptr<ISlateAttributeGetter> MakeUniqueGetter(TSlateAttributeBase& Attribute, FGetter&& Getter)
		{
			return std::make_unique<FSlateAttributeGetterWrapper<TSlateAttributeBase>>(Attribute, std::move(Getter));
		}
		ObjectType m_Value;
	};
}