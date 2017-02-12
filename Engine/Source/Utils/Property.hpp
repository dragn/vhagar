#pragma once

/*
    Basic read/write property
    Generated a setter, a getter and a on-change MultiDelegate
*/
#define VH_PROPERTY_RW(TYPE, NAME) \
public: \
    void Set ## NAME(const TYPE& val) { m ## NAME = val; Trigger_ ## NAME ## _OnChange(); }; \
    const TYPE& Get ## NAME() const { return m ## NAME; } \
    vh::MultiDelegate<const TYPE&> NAME ## _OnChange; \
    void Trigger_ ## NAME ## _OnChange() { NAME ## _OnChange(m ## NAME); } \
private: \
    TYPE m ## NAME;
