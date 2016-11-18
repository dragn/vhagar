#pragma once

#include <unordered_map>

namespace vh
{

template<typename KEY, typename TYPE>
class Factory
{
    typedef TYPE* (*FUNC)();

public:
    void Register(const KEY& name, FUNC func)
    {
        mRegistry[name] = func;
    }

    TYPE* Create(const KEY& name)
    {
        return mRegistry[name]();
    }

protected:
    template<typename T>
    static TYPE* _Create()
    {
        return new T();
    }

private:
    std::unordered_map<KEY, FUNC> mRegistry;
};

} // namespace vh
