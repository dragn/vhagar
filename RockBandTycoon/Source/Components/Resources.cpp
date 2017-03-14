#include "PCH.hpp"
#include "Resources.hpp"

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

void Resources::LoadItems(const char* filename, std::vector<ItemRes>& vec)
{
    std::string path = "Assets/";
    path.append(filename);

    std::fstream file(path, std::ios_base::in);
    if (file.is_open())
    {
        std::string name, img, cost;

        while (!file.eof())
        {
            name = "";
            while (name.empty() && !file.eof()) std::getline(file, name);
            if (file >> img >> cost)
            {
                LOG(INFO) << "add item '" << name << "'";
                std::string imgPath = "Assets/Images/Items/";
                imgPath.append(img);
                SDL_Surface* surf = IMG_Load(imgPath.c_str());
                if (surf == nullptr)
                {
                    LOG(ERROR) << "could not load image: " << name;
                }
                else
                {
                    vec.push_back(ItemRes(name, img, stoi(cost)));
                }
            }
        }
        file.close();
    }
    else
    {
        LOG(ERROR) << "Could not load items from " << filename;
    }
}

void Resources::TickInit(uint32_t delta)
{
    LoadBodyParts("hair_", mHairList);
    LoadBodyParts("face_", mFaceList);
    LoadBodyParts("guitar_body_", mGuitarBodyList);
    LoadBodyParts("guitar_hands_", mHandsList);
    LoadBodyParts("legs_", mLegsList);
    LoadBodyParts("drummer_body_", mDrumBodyList);

    srand(clock());

    FinishInit();
}

void Resources::TickClose(uint32_t delta)
{
    mHairList.clear();
    mFaceList.clear();
    mGuitarBodyList.clear();
    mHandsList.clear();
    mLegsList.clear();

    for (const std::pair<std::string, SDL_Surface*> p : mItemImgCache)
    {
        LOG(INFO) << "Releasing img for item " << p.first;
        SDL_FreeSurface(p.second);
    }
    mItemImgCache.clear();

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

SDL_Surface* Resources::GetHairImg(const std::string& name) const
{
    return GetBodyPart(name, mHairList);
}

SDL_Surface* Resources::GetFaceImg(const std::string& name) const
{
    return GetBodyPart(name, mFaceList);
}

SDL_Surface* Resources::GetGuitarBodyImg(const std::string& name) const
{
    return GetBodyPart(name, mGuitarBodyList);
}

SDL_Surface* Resources::GetHandsImg(const std::string& name) const
{
    return GetBodyPart(name, mHandsList);
}

SDL_Surface* Resources::GetLegsImg(const std::string& name) const
{
    return GetBodyPart(name, mLegsList);
}

SDL_Surface* GetRandomBodyPart(std::string& outName, const std::vector<BodyPart>& list)
{
    size_t idx = rand() % list.size();
    outName = list[idx].GetName();
    return list[idx].GetImg();
}


SDL_Surface* Resources::GetItemImg(const std::string& name)
{
    if (name.empty()) return nullptr;

    auto img = mItemImgCache.find(name);

    if (img == mItemImgCache.end())
    {
        std::string path = "Assets/Images/Items/";
        path += name;
        SDL_Surface* surf = IMG_Load(path.c_str());
        CHECK(surf);
        mItemImgCache[name] = surf;
        return surf;
    }

    return img->second;
}

void Resources::GetGuitars(std::vector<ItemRes>& outList) const
{
    for (const ItemRes& res : mGuitarList)
    {
        outList.push_back(res);
    }
}

Looks Resources::GetRandomLooks(eBandSlot::Type type)
{
    std::string hair, face, body, hands, legs;
    GetRandomBodyPart(hair, mHairList);
    GetRandomBodyPart(face, mFaceList);

    switch (type)
    {
    case eBandSlot::Guitar:
    case eBandSlot::Bass:
        GetRandomBodyPart(body, mGuitarBodyList);
        GetRandomBodyPart(hands, mHandsList);
        GetRandomBodyPart(legs, mLegsList);
        break;

    case eBandSlot::Drums:
        GetRandomBodyPart(body, mDrumBodyList);
        break;
    }

    return Looks(hair, face, body, hands, legs);
}

void Resources::GetBassGuitars(std::vector<ItemRes>& outList) const
{
    // TODO
}

void Resources::GetDrums(std::vector<ItemRes>& outList) const
{
    // TODO
}

SDL_Surface* Resources::GetBodyImg(const std::string& name) const
{
    auto it = std::find_if(mGuitarBodyList.begin(), mGuitarBodyList.end(), [&name] (const BodyPart& bp) { return bp.GetName() == name; });
    if (it != mGuitarBodyList.end()) return it->GetImg();

    it = std::find_if(mDrumBodyList.begin(), mDrumBodyList.end(), [&name] (const BodyPart& bp) { return bp.GetName() == name; });
    if (it != mDrumBodyList.end()) return it->GetImg();

    return nullptr;
}
