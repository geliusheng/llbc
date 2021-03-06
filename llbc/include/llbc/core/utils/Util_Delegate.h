/**
 * @file    Util_Delegate.h
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2013/04/30
 * @version 1.0
 *
 * @brief
 */
#ifdef __LLBC_CORE_UTILS_UTIL_DELEGATE_IMPL_H__

#define LLBC_DELEGATE_CAT(x, y)   LLBC_DELEGATE_CAT_I(x, y)
#define LLBC_DELEGATE_CAT_I(x, y) x ## y

#define LLBC_IDELEGATE     LLBC_DELEGATE_CAT(LLBC_IDelegate, LLBC_DELEGATE_N)
#define LLBC_DELEGATE      LLBC_DELEGATE_CAT(LLBC_Delegate,  LLBC_DELEGATE_N)
#define LLBC_FUNC_DELEGATE LLBC_DELEGATE_CAT(LLBC_FuncDelegate, LLBC_DELEGATE_N)

#if LLBC_DELEGATE_N == 0
#define LLBC_IDELEGATE_TEMPLATE 
#define LLBC_DELEGATE_BASE          LLBC_IDELEGATE
#define LLBC_DELEGATE_TEMPLATE      template <typename ObjectType>
#define LLBC_DELEGATE_ARG           void
#define LLBC_DELEGATE_VALUE 
#define LLBC_FUNC_DELEGATE_TEMPLATE LLBC_IDELEGATE_TEMPLATE
#elif LLBC_DELEGATE_N == 1
#define LLBC_IDELEGATE_TEMPLATE     template <typename Arg1>
#define LLBC_DELEGATE_BASE          LLBC_IDELEGATE<Arg1>
#define LLBC_DELEGATE_TEMPLATE      template <typename ObjectType, typename Arg1>
#define LLBC_DELEGATE_ARG           Arg1 arg1
#define LLBC_DELEGATE_VALUE         arg1
#define LLBC_FUNC_DELEGATE_TEMPLATE LLBC_IDELEGATE_TEMPLATE
#elif LLBC_DELEGATE_N == 2
#define LLBC_IDELEGATE_TEMPLATE     template <typename Arg1, typename Arg2>
#define LLBC_DELEGATE_BASE          LLBC_IDELEGATE<Arg1, Arg2>
#define LLBC_DELEGATE_TEMPLATE      template <typename ObjectType, typename Arg1, typename Arg2>
#define LLBC_DELEGATE_ARG           Arg1 arg1, Arg2 arg2
#define LLBC_DELEGATE_VALUE         arg1, arg2
#define LLBC_FUNC_DELEGATE_TEMPLATE LLBC_IDELEGATE_TEMPLATE
#elif LLBC_DELEGATE_N == 3
#define LLBC_IDELEGATE_TEMPLATE     template <typename Arg1, typename Arg2, typename Arg3>
#define LLBC_DELEGATE_BASE          LLBC_IDELEGATE<Arg1, Arg2, Arg3>
#define LLBC_DELEGATE_TEMPLATE      template <typename ObjectType, typename Arg1, typename Arg2, typename Arg3>
#define LLBC_DELEGATE_ARG           Arg1 arg1, Arg2 arg2, Arg3 arg3
#define LLBC_DELEGATE_VALUE         arg1, arg2, arg3
#define LLBC_FUNC_DELEGATE_TEMPLATE LLBC_IDELEGATE_TEMPLATE
#elif LLBC_DELEGATE_N == 4
#define LLBC_IDELEGATE_TEMPLATE     template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
#define LLBC_DELEGATE_BASE          LLBC_IDELEGATE<Arg1, Arg2, Arg3, Arg4>
#define LLBC_DELEGATE_TEMPLATE      template <typename ObjectType, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
#define LLBC_DELEGATE_ARG           Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4
#define LLBC_DELEGATE_VALUE         arg1, arg2, arg3, arg4
#define LLBC_FUNC_DELEGATE_TEMPLATE LLBC_IDELEGATE_TEMPLATE
#elif LLBC_DELEGATE_N == 5
#define LLBC_IDELEGATE_TEMPLATE     template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
#define LLBC_DELEGATE_BASE          LLBC_IDELEGATE<Arg1, Arg2, Arg3, Arg4, Arg5>
#define LLBC_DELEGATE_TEMPLATE      template <typename ObjectType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
#define LLBC_DELEGATE_ARG           Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5
#define LLBC_DELEGATE_VALUE         arg1, arg2, arg3, arg4, arg5
#define LLBC_FUNC_DELEGATE_TEMPLATE LLBC_IDELEGATE_TEMPLATE
#elif LLBC_DELEGATE_N == 6
#define LLBC_IDELEGATE_TEMPLATE     template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
#define LLBC_DELEGATE_BASE          LLBC_IDELEGATE<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>
#define LLBC_DELEGATE_TEMPLATE      template <typename ObjectType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
#define LLBC_DELEGATE_ARG           Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6
#define LLBC_DELEGATE_VALUE         arg1, arg2, arg3, arg4, arg5, arg6
#define LLBC_FUNC_DELEGATE_TEMPLATE LLBC_IDELEGATE_TEMPLATE
#elif LLBC_DELEGATE_N == 7
#define LLBC_IDELEGATE_TEMPLATE     template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
#define LLBC_DELEGATE_BASE          LLBC_IDELEGATE<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>
#define LLBC_DELEGATE_TEMPLATE      template <typename ObjectType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
#define LLBC_DELEGATE_ARG           Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7
#define LLBC_DELEGATE_VALUE         arg1, arg2, arg3, arg4, arg5, arg6, arg7
#define LLBC_FUNC_DELEGATE_TEMPLATE LLBC_IDELEGATE_TEMPLATE
#endif

/**
 * \brief Interface for delegate.
 */
LLBC_IDELEGATE_TEMPLATE
class LLBC_IDELEGATE
{
public:
    virtual ~LLBC_IDELEGATE() {  }

public:
    virtual void Invoke(LLBC_DELEGATE_ARG) = 0;
};

/**
 * \brief Delegate class encapsulation.
 *        Storage object and it's method.
 */
LLBC_DELEGATE_TEMPLATE
class LLBC_DELEGATE : public LLBC_DELEGATE_BASE
{
    typedef void (ObjectType::*Method)(LLBC_DELEGATE_ARG);
public:
    LLBC_DELEGATE(ObjectType *object, Method method):
        _object(object), _method(method) {  }

    virtual ~LLBC_DELEGATE() {  }

public:
    virtual void Invoke(LLBC_DELEGATE_ARG) { (_object->*_method)(LLBC_DELEGATE_VALUE); }

private:
    ObjectType *_object;
    Method _method;
};

/**
 * \brief Function specific delegate class encapsulation.
 *        Storage function.
 */
LLBC_FUNC_DELEGATE_TEMPLATE
class LLBC_FUNC_DELEGATE : public LLBC_DELEGATE_BASE
{
    typedef void (*Function)(LLBC_DELEGATE_ARG);
public:
    explicit LLBC_FUNC_DELEGATE(Function function):
        _function(function) {  }

    virtual ~LLBC_FUNC_DELEGATE() {  }

public:
    virtual void Invoke(LLBC_DELEGATE_ARG) { (*_function)(LLBC_DELEGATE_VALUE); }

private:
    Function _function;
};

#undef LLBC_DELEGATE_CAT
#undef LLBC_DELEGATE_CAT_I
#undef LLBC_IDELEGATE
#undef LLBC_DELEGATE
#undef LLBC_IDELEGATE_TEMPLATE
#undef LLBC_DELEGATE_BASE
#undef LLBC_DELEGATE_TEMPLATE
#undef LLBC_DELEGATE_ARG
#undef LLBC_DELEGATE_VALUE
#undef LLBC_FUNC_DELEGATE
#undef LLBC_FUNC_DELEGATE_TEMPLATE

#endif // __LLBC_CORE_UTILS_UTIL_DELEGATE_IMPL_H__
