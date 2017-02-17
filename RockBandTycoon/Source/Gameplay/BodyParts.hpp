#pragma once

#include "App/App.hpp"
#include "BodyPart.hpp"

class BodyPart
{
    friend class BodyParts;

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
    std::string mName = "";
    SDL_Surface* mImg = nullptr;
};

class BodyParts : public vh::Component
{
    COMPONENT_NAME(BodyParts);

public:
    BodyParts() : Component(vh::eTickFrequency::NEVER) {}

    virtual void TickInit(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    SDL_Surface* GetHairImg(const std::string& name) const;
    SDL_Surface* GetFaceImg(const std::string& name) const;
    SDL_Surface* GetBodyImg(const std::string& name) const;
    SDL_Surface* GetHandsImg(const std::string& name) const;
    SDL_Surface* GetLegsImg(const std::string& name) const;

    SDL_Surface* GetRandomHair(std::string& name) const;
    SDL_Surface* GetRandomFace(std::string& name) const;
    SDL_Surface* GetRandomBody(std::string& name) const;
    SDL_Surface* GetRandomHands(std::string& name) const;
    SDL_Surface* GetRandomLegs(std::string& name) const;

private:
    std::vector<BodyPart> mHairList;
    std::vector<BodyPart> mFaceList;
    std::vector<BodyPart> mHandsList;
    std::vector<BodyPart> mBodyList;
    std::vector<BodyPart> mLegsList;
};
