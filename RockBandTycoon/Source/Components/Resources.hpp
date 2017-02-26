#pragma once

#include "App/App.hpp"
#include "Gameplay/Looks.hpp"

class BodyPart
{
    friend class Resources;

public:
    BodyPart() : mName(""), mImg(nullptr) {}
    BodyPart(const std::string& name, SDL_Surface* img) : mName(name), mImg(img) {}
    BodyPart(BodyPart&& other) : mName(""), mImg(nullptr)
    {
        std::swap(mName, other.mName);
        std::swap(mImg, other.mImg);
    }
    ~BodyPart()
    {
        if (mImg != nullptr)
        {
            LOG(INFO) << "Releasing img for body part: " << mName;
            SDL_FreeSurface(mImg);
            mImg = nullptr;
        }
    }

    const std::string& GetName() const { return mName; }
    SDL_Surface* GetImg() const { return mImg; }

private:
    std::string mName;
    SDL_Surface* mImg;
};

class ItemRes
{
public:
    ItemRes() {};
    ItemRes(const std::string& name, const std::string& img, int32_t cost)
        : mName(name), mImg(img), mCost(cost)
    {
    }
    const std::string& GetName() const { return mName; }
    const std::string& GetImg() const { return mImg; }

private:
    std::string mName;
    int32_t mCost;
    std::string mImg;
};

class Resources : public vh::Component
{
    COMPONENT_NAME(Resources);

public:
    Resources() : Component(vh::eTickFrequency::NEVER) {}

    virtual void TickInit(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    SDL_Surface* GetHairImg(const std::string& name) const;
    SDL_Surface* GetFaceImg(const std::string& name) const;
    SDL_Surface* GetBodyImg(const std::string& name) const;
    SDL_Surface* GetHandsImg(const std::string& name) const;
    SDL_Surface* GetLegsImg(const std::string& name) const;

    Looks GetRandomLooks();

    SDL_Surface* GetItemImg(const std::string& name);

    void GetGuitars(std::vector<ItemRes>& outList) const;
    void GetBassGuitars(std::vector<ItemRes>& outList) const;
    void GetDrums(std::vector<ItemRes>& outList) const;

private:
    std::vector<BodyPart> mHairList;
    std::vector<BodyPart> mFaceList;
    std::vector<BodyPart> mHandsList;
    std::vector<BodyPart> mBodyList;
    std::vector<BodyPart> mLegsList;

    std::vector<ItemRes> mGuitarList;
    std::vector<ItemRes> mBassList;

    std::unordered_map<std::string, SDL_Surface*> mItemImgCache;

    void LoadItems(const char* filename, std::vector<ItemRes>& vec);
};
