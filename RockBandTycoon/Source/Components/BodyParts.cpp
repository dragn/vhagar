#include "PCH.hpp"
#include "BodyParts.hpp"

void LoadBodyParts(const char* prefix, std::vector<BodyPart>& vec)
{
    std::string dir = "Assets/Images/BodyParts/";
    std::stringstream name;
    std::string fullpath;

    for (uint32_t idx = 0; ; ++idx)
    {
        name.str(std::string()); name.clear();
        name << prefix << idx;
        fullpath = dir;
        fullpath.append(name.str());
        fullpath.append(".png");
        SDL_Surface* img = IMG_Load(fullpath.c_str());
        if (img == nullptr) break;
        LOG(INFO) << "add bodypart " << name.str();
        vec.push_back(BodyPart(name.str(), img));
    }
}

void BodyParts::TickInit(uint32_t delta)
{
    LoadBodyParts("hair_", mHairList);
    LoadBodyParts("face_", mFaceList);
    LoadBodyParts("guitar_body_", mBodyList);
    LoadBodyParts("guitar_hands_", mHandsList);
    LoadBodyParts("legs_", mLegsList);

    srand(clock());

    FinishInit();
}

void BodyParts::TickClose(uint32_t delta)
{
    mHairList.clear();
    mFaceList.clear();
    mBodyList.clear();
    mHandsList.clear();
    mLegsList.clear();

    FinishClose();
}

SDL_Surface* GetBodyPart(const std::string& name, const std::vector<BodyPart>& list)
{
    for (const BodyPart& part : list)
    {
        if (part.GetName() == name)
        {
            return part.GetImg();
        }
    }

    LOG(WARNING) << "Body part not found: " << name;

    return nullptr;
}

SDL_Surface* BodyParts::GetHairImg(const std::string& name) const
{
    return GetBodyPart(name, mHairList);
}

SDL_Surface* BodyParts::GetFaceImg(const std::string& name) const
{
    return GetBodyPart(name, mFaceList);
}

SDL_Surface* BodyParts::GetBodyImg(const std::string& name) const
{
    return GetBodyPart(name, mBodyList);
}

SDL_Surface* BodyParts::GetHandsImg(const std::string& name) const
{
    return GetBodyPart(name, mHandsList);
}

SDL_Surface* BodyParts::GetLegsImg(const std::string& name) const
{
    return GetBodyPart(name, mLegsList);
}

SDL_Surface* GetRandomBodyPart(std::string& outName, const std::vector<BodyPart>& list)
{
    size_t idx = rand() % list.size();
    outName = list[idx].GetName();
    return list[idx].GetImg();
}

SDL_Surface* BodyParts::GetRandomHair(std::string& outName) const
{
    return GetRandomBodyPart(outName, mHairList);
}

SDL_Surface* BodyParts::GetRandomFace(std::string& outName) const
{
    return GetRandomBodyPart(outName, mFaceList);
}

SDL_Surface* BodyParts::GetRandomBody(std::string& outName) const
{
    return GetRandomBodyPart(outName, mBodyList);
}

SDL_Surface* BodyParts::GetRandomHands(std::string& outName) const
{
    return GetRandomBodyPart(outName, mHandsList);
}

SDL_Surface* BodyParts::GetRandomLegs(std::string& outName) const
{
    return GetRandomBodyPart(outName, mLegsList);
}
