#pragma once

#include "App/App.hpp"

class Names : public vh::Component
{
    COMPONENT_NAME(Names);

    static const size_t MAX_WORD_SIZE = 32;

public:
    Names()
        : vh::Component(vh::eTickFrequency::NEVER)
        , mDictionaryLoaded(false)
        , mMaleNames(nullptr)
        , mFemaleNames(nullptr)
        , mLastNames(nullptr)
        , mMaleNamesCnt(0)
        , mFemaleNamesCnt(0)
        , mLastNamesCnt(0)
    {
    }

    virtual void TickInit(uint32_t delta);
    virtual void TickClose(uint32_t delta);

private:
    bool mDictionaryLoaded;

    char* mMaleNames;
    char* mFemaleNames;
    char* mLastNames;

    size_t mMaleNamesCnt;
    size_t mFemaleNamesCnt;
    size_t mLastNamesCnt;

    void LoadDictionary();
    void DestroyDictionary();
};
