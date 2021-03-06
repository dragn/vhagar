#pragma once

#include "Utils/standard.hpp"

namespace vh
{

template<typename... Args>
class IFunction
{
public:
    virtual ~IFunction() {}
    virtual void Call(Args... args) = 0;
};

template<typename M, typename... Args>
class Function : public IFunction<Args...>
{
    template<typename... _Args>
    friend class MultiDelegate;

public:
    Function(M method) : mMethod(method) {}
    virtual void Call(Args... args) override { mMethod(args...); }

private:
    M mMethod;
};

template<typename T, typename M, typename... Args>
class BindFunction : public IFunction<Args...>
{
    template<typename... _Args>
    friend class MultiDelegate;

public:
    BindFunction(T t, M method) : mThis(t), mMethod(method) {}
    virtual void Call(Args... args) override { if (mThis != nullptr) (mThis->*mMethod)(args...); }

private:
    T mThis;
    M mMethod;
};

template<typename T, typename M, typename PARAM1, typename... Args>
class BindParamFunction : public IFunction<Args...>
{
    template<typename... _Args>
    friend class MultiDelegate;

public:
    BindParamFunction(T t, M method, PARAM1 param1) : mThis(t), mMethod(method), mParam1(param1) {}
    virtual void Call(Args... args) override { if (mThis != nullptr) (mThis->*mMethod)(args..., mParam1); }

private:
    T mThis;
    M mMethod;
    PARAM1 mParam1;
};

class Delegate
{
public:
    template<typename M>
    void Set(M m)
    {
        mFunc.reset(new Function<M>(m));
    }

    template<typename T, typename M>
    void Set(T t, M m)
    {
        mFunc.reset(new BindFunction<T, M>(t, m));
    }

    virtual void operator()()
    {
        if (mFunc) mFunc->Call();
    }

private:
    std::unique_ptr<IFunction<>> mFunc;
};

template<typename... Args>
class MultiDelegate
{
public:
    MultiDelegate() {};
    virtual ~MultiDelegate()
    {
    };

    template<typename M>
    void Add(M m)
    {
        mList.push_back(new Function<M, Args...>(m));
    }

    template<typename M>
    void Remove(M m)
    {
        auto func = mList.begin();
        while (func != mList.end())
        {
            const Function<M, Args...>* f = reinterpret_cast<Function<M, Args...>*>(*func);
            if (f->mMethod == m)
            {
                func = mList.erase(func);
            }
            else
            {
                ++func;
            }
        }
    }

    template<typename T, typename M>
    void Add(T t, M m)
    {
        mList.push_back(new BindFunction<T, M, Args...>(t, m));
    }

    template<typename T, typename M, typename P1>
    void Add(T t, M m, P1 p)
    {
        mList.push_back(new BindParamFunction<T, M, P1, Args...>(t, m, p));
    }

    template<typename T, typename M>
    void Remove(T t, M m)
    {
        auto func = mList.begin();
        while (func != mList.end())
        {
            const BindFunction<T, M, Args...>* f = reinterpret_cast<BindFunction<T, M, Args...>*>(*func);
            if (f->mThis == t && f->mMethod == m)
            {
                func = mList.erase(func);
            }
            else
            {
                ++func;
            }
        }
    }

    virtual void operator()(Args... args)
    {
        for (auto f : mList)
        {
            f->Call(args...);
        }
    }

private:
    std::list<IFunction<Args...>*> mList;
};

} // namespace vh
