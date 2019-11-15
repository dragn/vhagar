#pragma once

#include "Delegate.hpp"

/*
    Basic read/write property
    Generates a setter, a getter and a on-change MultiDelegate
*/
#define VH_PROPERTY_RW(TYPE, NAME) \
public: \
    void Set ## NAME(TYPE const& val) { m ## NAME = val; Trigger_ ## NAME ## _OnChange(); }; \
    TYPE const& Get ## NAME() const { return m ## NAME; } \
    vh::MultiDelegate<TYPE const&> NAME ## _OnChange; \
    void Trigger_ ## NAME ## _OnChange() { NAME ## _OnChange(m ## NAME); } \
private: \
    TYPE m ## NAME;

/*
    Basic read-only property
    Generates a getter
*/
#define VH_PROPERTY_RO(TYPE, NAME) \
public: \
    TYPE const& Get ## NAME() const { return m ## NAME; } \
private: \
    TYPE m ## NAME;
