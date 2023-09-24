#pragma once
#include "Core.h"
#include "SlateCore/Layout/Visibility.h"
#include "SlateCore/Types/ISlateMetaData.h"
#include "Core/Misc/Attribute.h"

namespace ZeroUI
{
	/*
	 * slate widgets are constructed SNew and SAssignNew
	 *
	 * TSharedRef<SButton> MyButton = SNew(SButton)
	 * or
	 * TSharedPtr<SButton> MyButton;
	 * SAssignNew(MyButton, SButton);
	 *
	 * using SNew and SAssignNew ensures that widgets are populated
	 */
#define SNew(WidgetType, ...) \
	MakeTDecl<WidgetType>(#WidgetType, __FILE__, __LINE__, RequiredArgs::MakeRequiredArgs(__VA_ARGS__)) <<= WidgetType::FArguments()

#define SAssignNew( ExposeAs, WidgetType, ... ) \
	MakeTDecl<WidgetType>( #WidgetType, __FILE__, __LINE__, RequiredArgs::MakeRequiredArgs(__VA_ARGS__) ) . Expose( ExposeAs ) <<= WidgetType::FArguments()

#define SArgumentNew( InArgs, WidgetType, ... ) \
	MakeTDecl<WidgetType>( #WidgetType, __FILE__, __LINE__, RequiredArgs::MakeRequiredArgs(__VA_ARGS__) ) <<= InArgs

	/**
	 * Widget authors can use SLATE_BEGIN_ARGS and SLATE_END_ARS to add support
	 * for widget construction via SNew and SAssignNew.
	 * e.g.
	 *
	 *    SLATE_BEGIN_ARGS( SMyWidget )
	 *         , _PreferredWidth( 150.0f )
	 *         , _ForegroundColor( FLinearColor::White )
	 *         {}
	 *
	 *         SLATE_ATTRIBUTE(float, PreferredWidth)
	 *         SLATE_ATTRIBUTE(FSlateColor, ForegroundColor)
	 *    SLATE_END_ARGS()
	 */
#define SLATE_BEGIN_ARGS(WidgetType) \
	public:\
	struct FArguments : public TSlateBaseNamedArgs<WidgetType> \
	{ \
		typedef FArguments WidgetArgsType;\
		inline FArguments()
	/**
	 * Just like SLATE_BEGIN_ARGS but requires the user to implement the New() method in the .CPP.
	 * Allows for widget implementation details to be entirely reside in the .CPP file.
	 * e.g.
	 *   SMyWidget.h:
	 *   ----------------------------------------------------------------
	 *    SLATE_USER_ARGS( SMyWidget )
	 *    {}
	 *    SLATE_END_ARGS()
	 *
	 *    virtual void DoStuff() = nullptr;
	 *
	 *   SMyWidget.cpp:
	 *   ----------------------------------------------------------------
	 *   namespace Implementation{
	 *   class SMyWidget : public SMyWidget{
	 *     void Construct( const FArguments& InArgs )
	 *     {
	 *         SUserWidget::Construct( SUSerWidget::FArguments()
	 *         [
	 *             SNew(STextBlock) .Text( "Implementation Details!" )
	 *         ] );
	 *     }
	 *
	 *     virtual void DoStuff() override {}
	 *
	 *     // Truly private. All handlers can be inlined (less boilerplate).
	 *     // All private members are truly private.
	 *     private:
	 *     FReply OnButtonClicked
	 *     {
	 *     }
	 *     TSharedPtr<SButton> MyButton;
	 *   }
	 *   }
	 */
#define SLATE_END_ARGS() \
	};


#define SLATE_PRIVATE_ATTRIBUTE_VARIABLE(AttrType, AttrName) \
		TAttribute<AttrType> _##AttrName

#define SLATE_PRIVATE_ATTRIBUTE_FUNCTION(AttrType, AttrName) \
		WidgetArgsType& AttrName(TAttribute<AttrType> InAttribute) \
		{ \
			_##AttrName = std::move(InAttribute);\
			return static_cast<WidgetArgsType*>(this)->Me();\
		}
		/*
		 * bind attribute with delegate to a global function
		 * note : we use a template here to avoid 'typename' issues when hosting attributes inside templated classes
		 */
#define SLATE_PRIVATE_ARGUMENT_VARIABLE(ArgType, ArgName) \
		ArgType _##ArgName

#define SLATE_PRIVATE_ARGUMENT_FUNCTION(ArgType, ArgName) \
		WidgetArgsType& ArgName(ArgType InArg) \
		{ \
			_##ArgName = InArg;\
			return static_cast<WidgetArgsType*>(this)->Me();\
		}
/**
 * Use this macro to add a attribute to the declaration of your widget.
 * An attribute can be a value or a function.
 */
#define SLATE_ATTRIBUTE( AttrType, AttrName ) \
		SLATE_PRIVATE_ATTRIBUTE_VARIABLE( AttrType, AttrName ); \
		SLATE_PRIVATE_ATTRIBUTE_FUNCTION( AttrType, AttrName )

/**
 * Use this macro to declare a slate argument.
 * Arguments differ from attributes in that they can only be values
 */
#define SLATE_ARGUMENT( ArgType, ArgName ) \
		SLATE_PRIVATE_ARGUMENT_VARIABLE( ArgType, ArgName ); \
		SLATE_PRIVATE_ARGUMENT_FUNCTION ( ArgType, ArgName )

/*
 * use this macro to declare a slate argument
 * arguments differ from attributes in that they can only be values
 */
#define SLATE_STYLE_ARGUMENT(ArgType, ArgName) \
		const ArgType* _##ArgName; \
		WidgetArgsType& ArgName(const ArgType* in_arg) \
		{ \
			_##ArgName = in_arg; \
			return static_cast<WidgetArgsType*>(this)->Me(); \
		}
/*
 * use this macro between SLATE_BEGIN_ARGS and SLATE_END_ARGS
 * in order to add support for slots with the construct pattern
 */
#define SLATE_SLOT_ARGUMENT(SlotType, SlotName) \
		std::vector<typename SlotType::FSlotArguments> _##SlotName; \
		WidgetArgsType& operator + (typename SlotType::FSlotArguments& SlotToAdd) \
		{ \
			_##SlotName.push_back(std::move(SlotToAdd)); \
			return static_cast<WidgetArgsType*>(this)->Me(); \
		} \
		WidgetArgsType& operator + (typename SlotType::FSlotArguments&& SlotToAdd) \
		{ \
			_##SlotName.push_back(std::move(SlotToAdd)); \
			return static_cast<WidgetArgsType*>(this)->Me(); \
		}

#define SLATE_PRIVATE_SLOT_BEGIN_ARGS(SlotType, SlotParentType) \
	{\
		using WidgetArgsType = SlotType::FSlotArguments;\
		using SlotParentType::FSlotArguments::FSlotArguments;

#define SLATE_SLOT_BEGIN_ARGS(SlotType, SlotParentType) \
		public:\
		struct FSlotArguments : public SlotParentType::FSlotArguments \
		SLATE_PRIVATE_SLOT_BEGIN_ARGS(SlotType, SlotParentType)

#define SLATE_SLOT_BEGIN_ARGS_OneMixin(SlotType, SlotParentType, Mixin1) \
		public:\
		struct FSlotArguments : public SlotParentType::FSlotArguments, public Mixin1::FSlotArgumentsMixin \
		SLATE_PRIVATE_SLOT_BEGIN_ARGS(SlotType, SlotParentType)

#define SLATE_SLOT_BEGIN_ARGS_TwoMixins(SlotType, SlotParentType, Mixin1, Mixin2) \
		public:\
		struct FSlotArguments : public SlotParentType::FSlotArguments, public Mixin1::FSlotArgumentsMixin, public Mixin2::FSlotArgumentsMixin \
		SLATE_PRIVATE_SLOT_BEGIN_ARGS(SlotType, SlotParentType

#define SLATE_SLOT_END_ARGS() \
	};

	struct TAlwaysValidWidget
	{
		TAlwaysValidWidget()
			: m_widget() //null widget
		{
		}

		std::shared_ptr<SWidget> m_widget;
	};
	/**
	 * We want to be able to do:
	 * SNew( ContainerWidget )
	 * .SomeContentArea()
	 * [
	 *   // Child widgets go here
	 * ]
	 *
	 * NamedSlotProperty is a helper that will be returned by SomeContentArea().
	 */
	template<class DeclarationType>
	struct NamedSlotProperty
	{
		NamedSlotProperty( DeclarationType& InOwnerDeclaration, TAlwaysValidWidget& ContentToSet )
			: OwnerDeclaration( InOwnerDeclaration )
			, SlotContent(ContentToSet)
		{}

		DeclarationType & operator[]( const TSharedRef<SWidget>& InChild )
		{
			SlotContent.Widget = InChild;
			return OwnerDeclaration;
		}

		DeclarationType & OwnerDeclaration;
		TAlwaysValidWidget & SlotContent;
	};
/**
 * Use this macro to add support for named slot properties such as Content and Header. See NamedSlotProperty for more details.
 *
 * NOTE: If you're using this within a widget class that is templated, then you might have to specify a full name for the declaration.
 *       For example: SLATE_NAMED_SLOT( typename SSuperWidget<T>::Declaration, Content )
 */
#define SLATE_NAMED_SLOT( DeclarationType, SlotName ) \
		NamedSlotProperty< DeclarationType > SlotName() \
		{ \
			return NamedSlotProperty< DeclarationType >( *this, _##SlotName ); \
		} \
		TAlwaysValidWidget _##SlotName; \

#define SLATE_DEFAULT_SLOT( DeclarationType, SlotName ) \
		SLATE_NAMED_SLOT(DeclarationType, SlotName) ; \
		DeclarationType & operator[]( const TSharedRef<SWidget>& InChild ) \
		{ \
			_##SlotName.Widget = InChild; \
			return static_cast<WidgetArgsType*>(this)->Me(); \
		}

		/**
		 * Use this macro to add event handler support to the declarative syntax of your widget.
		 * It is expected that the widget has a delegate called of type 'EventDelegateType' that is
		 * named 'EventName'.
		 */
#define SLATE_EVENT(DelegateName, EventName)\
		WidgetArgsType& EventName(const DelegateName& InDelegate) \
		{ \
			_##EventName = InDelegate; \
			return static_cast<WidgetArgsType*>(this)->Me();\
		} \
		\
		WidgetArgsType& EventName(DelegateName&& InDelegate) \
		{ \
			_##EventName = InDelegate; \
			return static_cast<WidgetArgsType*>(this)->Me();\
		} \
		/* set event delegate to a global function */\
		/* note: we use a template here to avoid 'typename' issues when hosting attributes inside templated classes */\
		template<typename StaticFuncPtr, typename... VarTypes> \
		WidgetArgsType& EventName##_Static(StaticFuncPtr InFunc, VarTypes... Vars )	\
		{ \
			_##EventName = DelegateName::CreateStatic(InFunc, Vars...); \
			return static_cast<WidgetArgsType*>(this)->Me(); \
		} \
		DelegateName _##EventName;

	struct FSlateBaseNamedArgs
	{
		FSlateBaseNamedArgs() = default;

		SLATE_PRIVATE_ATTRIBUTE_VARIABLE(EVisibility, Visibility) = EVisibility::Visible;
		SLATE_PRIVATE_ATTRIBUTE_VARIABLE(bool, IsEnabled) = true;
		SLATE_PRIVATE_ATTRIBUTE_VARIABLE(ZMath::vec2, RenderTransformPivot) = ZMath::vec2(0.0f, 0.0f);

		std:vector<Ref<ISlateMetaData>> MetaData;
	};

	template<typename WidgetType>
	struct TSlateBaseNamedArgs : public FSlateBaseNamedArgs
	{
		typedef typename WidgetType::FArguments WidgetArgsType;

		/** Used by the named argument pattern as a safe way to 'return *this' for call-chaining purposes. */
		WidgetArgsType& Me()
		{
			return *(static_cast<WidgetArgsType*>(this));
		}

		/** Add metadata to this widget. */
		WidgetArgsType& AddMetaData(Ref<ISlateMetaData> InMetaData)
		{
			MetaData.Add(InMetaData);
			return Me();
		}

		/** Add metadata to this widget - convenience method - 1 argument */
		template<typename MetaDataType, typename Arg0Type>
		WidgetArgsType& AddMetaData(Arg0Type InArg0)
		{
			MetaData.Add(MakeShared<MetaDataType>(InArg0));
			return Me();
		}

		/** Add metadata to this widget - convenience method - 2 arguments */
		template<typename MetaDataType, typename Arg0Type, typename Arg1Type>
		WidgetArgsType& AddMetaData(Arg0Type InArg0, Arg1Type InArg1)
		{
			MetaData.Add(MakeShared<MetaDataType>(InArg0, InArg1));
			return Me();
		}
	};

	namespace RequiredArgs
	{
		struct T0RequiredArgs
		{
			T0RequiredArgs()
			{
			}

			template<class WidgetType>
			void CallConstruct(const Ref<WidgetType>& OnWidget, const typename WidgetType::FArguments& WithNamedArgs) const
			{
				// YOUR WIDGET MUST IMPLEMENT void Construct(const FArguments& InArgs)
				OnWidget->Construct(WithNamedArgs);
			}
		};

		template<typename Arg0Type>
		struct T1RequiredArgs
		{
			T1RequiredArgs(Arg0Type&& InArg0)
				: Arg0(InArg0)
			{
			}

			template<class WidgetType>
			void CallConstruct(const Ref<WidgetType>& OnWidget, const typename WidgetType::FArguments& WithNamedArgs) const
			{
				// YOUR WIDGET MUST IMPLEMENT void Construct(const FArguments& InArgs)
				OnWidget->Construct(WithNamedArgs, Forward<Arg0Type>(Arg0));
			}

			Arg0Type& Arg0;
		};

		template<typename Arg0Type, typename Arg1Type>
		struct T2RequiredArgs
		{
			T2RequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1)
				: Arg0(InArg0)
				, Arg1(InArg1)
			{
			}

			template<class WidgetType>
			void CallConstruct(const Ref<WidgetType>& OnWidget, const typename WidgetType::FArguments& WithNamedArgs) const
			{
				// YOUR WIDGET MUST IMPLEMENT Construct(const FArguments& InArgs)
				OnWidget->Construct(WithNamedArgs, Forward<Arg0Type>(Arg0), Forward<Arg1Type>(Arg1));
			}

			Arg0Type& Arg0;
			Arg1Type& Arg1;
		};

		template<typename Arg0Type, typename Arg1Type, typename Arg2Type>
		struct T3RequiredArgs
		{
			T3RequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1, Arg2Type&& InArg2)
				: Arg0(InArg0)
				, Arg1(InArg1)
				, Arg2(InArg2)
			{
			}

			template<class WidgetType>
			void CallConstruct(const Ref<WidgetType>& OnWidget, const typename WidgetType::FArguments& WithNamedArgs) const
			{
				// YOUR WIDGET MUST IMPLEMENT Construct(const FArguments& InArgs)
				OnWidget->Construct(WithNamedArgs, Forward<Arg0Type>(Arg0), Forward<Arg1Type>(Arg1), Forward<Arg2Type>(Arg2));
			}

			Arg0Type& Arg0;
			Arg1Type& Arg1;
			Arg2Type& Arg2;
		};

		template<typename Arg0Type, typename Arg1Type, typename Arg2Type, typename Arg3Type>
		struct T4RequiredArgs
		{
			T4RequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1, Arg2Type&& InArg2, Arg3Type&& InArg3)
				: Arg0(InArg0)
				, Arg1(InArg1)
				, Arg2(InArg2)
				, Arg3(InArg3)
			{
			}

			template<class WidgetType>
			void CallConstruct(const Ref<WidgetType>& OnWidget, const typename WidgetType::FArguments& WithNamedArgs) const
			{
				// YOUR WIDGET MUST IMPLEMENT Construct(const FArguments& InArgs)
				OnWidget->Construct(WithNamedArgs, Forward<Arg0Type>(Arg0), Forward<Arg1Type>(Arg1), Forward<Arg2Type>(Arg2), Forward<Arg3Type>(Arg3));
			}

			Arg0Type& Arg0;
			Arg1Type& Arg1;
			Arg2Type& Arg2;
			Arg3Type& Arg3;
		};

		template<typename Arg0Type, typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
		struct T5RequiredArgs
		{
			T5RequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1, Arg2Type&& InArg2, Arg3Type&& InArg3, Arg4Type&& InArg4)
				: Arg0(InArg0)
				, Arg1(InArg1)
				, Arg2(InArg2)
				, Arg3(InArg3)
				, Arg4(InArg4)
			{
			}

			template<class WidgetType>
			void CallConstruct(const Ref<WidgetType>& OnWidget, const typename WidgetType::FArguments& WithNamedArgs) const
			{
				// YOUR WIDGET MUST IMPLEMENT Construct(const FArguments& InArgs)
				OnWidget->Construct(WithNamedArgs, Forward<Arg0Type>(Arg0), Forward<Arg1Type>(Arg1), Forward<Arg2Type>(Arg2), Forward<Arg3Type>(Arg3), Forward<Arg4Type>(Arg4));
			}

			Arg0Type& Arg0;
			Arg1Type& Arg1;
			Arg2Type& Arg2;
			Arg3Type& Arg3;
			Arg4Type& Arg4;
		};

		FORCEINLINE T0RequiredArgs MakeRequiredArgs()
		{
			return T0RequiredArgs();
		}

		template<typename Arg0Type>
		T1RequiredArgs<Arg0Type&&> MakeRequiredArgs(Arg0Type&& InArg0)
		{
			return T1RequiredArgs<Arg0Type&&>(Forward<Arg0Type>(InArg0));
		}

		template<typename Arg0Type, typename Arg1Type>
		T2RequiredArgs<Arg0Type&&, Arg1Type&&> MakeRequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1)
		{
			return T2RequiredArgs<Arg0Type&&, Arg1Type&&>(Forward<Arg0Type>(InArg0), Forward<Arg1Type>(InArg1));
		}

		template<typename Arg0Type, typename Arg1Type, typename Arg2Type>
		T3RequiredArgs<Arg0Type&&, Arg1Type&&, Arg2Type&&> MakeRequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1, Arg2Type&& InArg2)
		{
			return T3RequiredArgs<Arg0Type&&, Arg1Type&&, Arg2Type&&>(Forward<Arg0Type>(InArg0), Forward<Arg1Type>(InArg1), Forward<Arg2Type>(InArg2));
		}

		template<typename Arg0Type, typename Arg1Type, typename Arg2Type, typename Arg3Type>
		T4RequiredArgs<Arg0Type&&, Arg1Type&&, Arg2Type&&, Arg3Type&&> MakeRequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1, Arg2Type&& InArg2, Arg3Type&& InArg3)
		{
			return T4RequiredArgs<Arg0Type&&, Arg1Type&&, Arg2Type&&, Arg3Type&&>(Forward<Arg0Type>(InArg0), Forward<Arg1Type>(InArg1), Forward<Arg2Type>(InArg2), Forward<Arg3Type>(InArg3));
		}

		template<typename Arg0Type, typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
		T5RequiredArgs<Arg0Type&&, Arg1Type&&, Arg2Type&&, Arg3Type&&, Arg4Type&&> MakeRequiredArgs(Arg0Type&& InArg0, Arg1Type&& InArg1, Arg2Type&& InArg2, Arg3Type&& InArg3, Arg4Type&& InArg4)
		{
			return T5RequiredArgs<Arg0Type&&, Arg1Type&&, Arg2Type&&, Arg3Type&&, Arg4Type&&>(Forward<Arg0Type>(InArg0), Forward<Arg1Type>(InArg1), Forward<Arg2Type>(InArg2), Forward<Arg3Type>(InArg3), Forward<Arg4Type>(InArg4));
		}
	};

	template<typename WidgetType>
	struct TWidgetAllocator
	{
		static Ref<WidgetType> PrivateAllocatedWidget()
		{
			return Ref<WidgetType>();
		}
	};
	/*
	 * utility class used during widget instantiation
	 * performs widget allocation and construction(important)
	 * ensures that debug info is set correctly
	 * returns Ref to widget
	 *
	 * @see SNew
	 * @see SAssignNew
	 */
	template<class WidgetType, typename RequiredArgsPayloadType>
	struct TSlateDecl
	{
		TSlateDecl(const char* InType, const char* InFile, int32_t OnLine, RequiredArgsPayloadType&& InRequiredArgs)
			: _Widget(TWidgetAllocator<WidgetType>::PrivateAllocatedWidget())//this is for allocate a new swidget in the heap
			, _RequiredArgs(InRequiredArgs)
		{
			// Debug
		}

		/**
		 * Initialize OutVarToInit with the widget that is being constructed.
		* @see SAssignNew
		*/
		template<class ExposeAsWidgetType>
		TSlateDecl& Expose( Ref<ExposeAsWidgetType>& OutVarToInit )
		{
			OutVarToInit = _Widget;
			return *this;
		}

		/**
		 * Initialize OutVarToInit with the widget that is being constructed.
		* @see SAssignNew
		*/
		template<class ExposeAsWidgetType>
		TSlateDecl& Expose( Ref<ExposeAsWidgetType>& OutVarToInit )
		{
			OutVarToInit = _Widget;
			return *this;
		}

		const Ref<WidgetType> _Widget;
		RequiredArgsPayloadType& _RequiredArgs;
	};

	template<typename WidgetType, typename RequiredArgsPayloadType>
	TSlateDecl<WidgetType, RequiredArgsPayloadType> MakeTDecl(const char* Intype, const char* InFile, int32_t OnLine, RequiredArgsPayloadType&& InRequiredArgs)
	{
		return TSlateDecl<WidgetType, RequiredArgsPayloadType>(Intype, InFile, OnLine, std::forward<RequiredArgsPayloadType>(InRequiredArgs));
	}
}



