#pragma once

#define VH_STR(A) #A
#define VH_WSTR(A) L#A
#define VH_XSTR(A) VH_STR(A)
#define VH_XWSTR(A) VH_WSTR(A)
#define VH_CONCAT(A, B) A ## B

#define UNCOPYABLE(Class) \
  Class(const Class&) = delete; \
  Class& operator=(const Class&) = delete;

#define IF_KEYDOWN(event, code) if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == code)
#define IF_KEYDOWN_SYM(event, code) if (event->type == SDL_KEYDOWN && event->key.keysym.sym == code)

namespace vh
{

    template<typename T, typename F>
    void for_each(T t, F f)
    {
        std::for_each(t.begin(), t.end(), f);
    }

    template <typename T>
    void SafeDelete(T*& ptr)
    {
        if (ptr) delete ptr;
        ptr = nullptr;
    }

    template <typename T>
    void SafeDeleteArray(T*& ptr)
    {
        if (ptr) delete[] ptr;
        ptr = nullptr;
    }

} // namespace vh
