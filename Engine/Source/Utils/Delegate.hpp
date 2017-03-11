#pragma once

#include <list>

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

class Delegate
{
public:
    Delegate() : mFunc(nullptr) {};
    ~Delegate() { if (mFunc != nullptr) delete mFunc; }

    template<typename M>
    void Set(M m)
    {
        if (mFunc != nullptr) delete mFunc;
        mFunc = new Function<M>(m);
    }

    template<typename T, typename M>
    void Set(T t, M m)
    {
        if (mFunc != nullptr) delete mFunc;
        mFunc = new BindFunction<T, M>(t, m);
    }

    virtual void operator()()
    {
        if (mFunc) mFunc->Call();
    }

private:
    IFunction<>* mFunc;
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
