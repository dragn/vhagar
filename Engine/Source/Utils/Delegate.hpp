#pragma once

namespace vh
{

class IFunction
{
public:
    virtual void Call() = 0;
};

template<typename M>
class Function : public IFunction
{
public:
    Function(M method) : mMethod(method) {}
    virtual void Call() override { mMethod(); }

private:
    M mMethod;
};

template<typename T, typename M>
class BindFunction : public IFunction
{
public:
    BindFunction(T t, M method) : mThis(t), mMethod(method) {}
    virtual void Call() override { if (mThis != nullptr) (mThis->*mMethod)(); }

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
    IFunction* mFunc;
};

} // namespace vh
