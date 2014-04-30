#!/bin/sh

if [ $# -ne 1 ]
then
    echo "usage: `basename $0` <param_count>"
    exit 1
fi

param_cnt=$1

for i in `seq $param_cnt`
do
    meta_1=$meta_1", typename P$i"
    meta_4=$meta_4", a$i"
    meta_5=$meta_5", P$i"
    meta_6=$meta_6", typename FP$i"
    meta_7=$meta_7", P$i a$i"
    if [ $i -ne $param_cnt ]
    then
       meta_2=$meta_2"P$i, "
       meta_3=$meta_3"P$i a$i, "
       meta_8=$meta_8"a$i, "
    else
       meta_2=$meta_2"P$i"
       meta_3=$meta_3"P$i a$i"
       meta_8=$meta_8"a$i"
    fi

done

template_1="typename R$meta_1"
template_2="R ($meta_2)"
template_3="$meta_3"
template_4="obj_$meta_4"
template_5="const void *$meta_5"
template_6="typename FR$meta_6"
template_7="typename FR, class FT$meta_6"
template_8="R (*func)($meta_2)"
template_9="const void *$meta_7"
template_10="$meta_8"
template_11="R$meta_5"
template_12="R (*)($meta_2)"
template_13="typename R, class T$meta_1"
template_14="const void *o$meta_7"
template_15="R, T$meta_5"
template_16="R (T::*)($meta_2)"
template_17="R (T::*func)($meta_2)"

echo "\
template <$template_1>
class Function<$template_2> {
public:
    Function() :
        func_(0), obj_(0) {}
    Function(NullFunction) :
        func_(0), obj_(0) {}
    Function(const Function &rhs) :
        func_(rhs.func_), obj_(rhs.obj_) {}

    Function &operator=(NullFunction)
        { func_ = 0; obj_ = 0; return *this; }
    Function &operator=(const Function &rhs)
        { func_ = rhs.func_; obj_ = rhs.obj_; return *this; }

    inline R operator()($template_3) const
    {
        return (*func_)($template_4);
    }

    BRICKRED_SAFE_BOOL_TYPE(Function)
    operator SafeBoolType() const
    {
        return func_ != 0 ? &Function::SafeBoolTypeNotNull : 0;
    }

private:
    typedef R (*FunctionType)($template_5);
    Function(FunctionType func, const void *obj) :
        func_(func), obj_(obj) {}

    template <$template_6>
    friend class FreeFunctionFactory$param_cnt;
    template <$template_7>
    friend class MemberFunctionFactory$param_cnt;
    template <$template_7>
    friend class ConstMemberFunctionFactory$param_cnt;

    FunctionType func_;
    const void *obj_;
};

template <$template_1>
void operator==(const Function<$template_2> &, const Function<$template_2> &);
template <$template_1>
void operator!=(const Function<$template_2> &, const Function<$template_2> &);

template <$template_1>
class FreeFunctionFactory$param_cnt {
private:
    template <$template_8>
    static R wrapper($template_9)
    {
        return (*func)($template_10);
    }

public:
    template <$template_8>
    inline static Function<$template_2> bind()
    {
        return Function<$template_2>(&FreeFunctionFactory$param_cnt::wrapper<func>, 0);
    }
};

template <$template_1>
inline FreeFunctionFactory$param_cnt<$template_11> _getFunctionFactory($template_12)
{
    return FreeFunctionFactory$param_cnt<$template_11>();
}

template <$template_13>
class MemberFunctionFactory$param_cnt {
private:
    template <$template_17>
    static R wrapper($template_14)
    {
        T *obj = const_cast<T *>(static_cast<const T *>(o));
        return (obj->*func)($template_10);
    }

public:
    template <$template_17>
    inline static Function<$template_2> bind(T *o)
    {
        return Function<$template_2>(&MemberFunctionFactory$param_cnt::wrapper<func>,
                                static_cast<const void *>(o));
    }
};

template <$template_13>
inline MemberFunctionFactory$param_cnt<$template_15> _getFunctionFactory($template_16)
{
    return MemberFunctionFactory$param_cnt<$template_15>();
}

template <$template_13>
class ConstMemberFunctionFactory$param_cnt {
private:
    template <$template_17 const>
    static R wrapper($template_14)
    {
        const T *obj = static_cast<const T *>(o);
        return (obj->*func)($template_10);
    }

public:
    template <$template_17 const>
    inline static Function<$template_2> bind(const T *o)
    {
        return Function<$template_2>(&ConstMemberFunctionFactory$param_cnt::wrapper<func>,
                              static_cast<const void *>(o));
    }
};

template <$template_13>
inline ConstMemberFunctionFactory$param_cnt<$template_15> _getFunctionFactory($template_16 const)
{
    return ConstMemberFunctionFactory$param_cnt<$template_15>();
}"

exit 0
