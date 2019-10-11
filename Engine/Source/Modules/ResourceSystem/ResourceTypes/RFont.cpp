#include "Modules/VhModules_PCH.hpp"
#include "RFont.hpp"

template<>
bool vh::MResourceSystem::Load(const char* path, std::shared_ptr<RFont> fontRes)
{
    fontRes->mFontResPath = std::string(path);
    return true;
}

vh::RFont::~RFont()
{
    for (decltype(mFontInst)::value_type value : mFontInst)
    {
        TTF_CloseFont(value.second);
        LOG(INFO) << "Closing font: " << mFontResPath << ":" << value.first;
    }
    mFontInst.clear();
}

TTF_Font* vh::RFont::GetFont(uint32_t fontSize)
{
    decltype(mFontInst)::iterator it = mFontInst.find(fontSize);
    if (it == mFontInst.end())
    {
        std::string fullPath = std::string("Assets/") + mFontResPath;
        TTF_Font* font = TTF_OpenFont(fullPath.c_str(), fontSize);
        if (font == nullptr)
        {
            LOG(ERROR) << "Font loading failed: " << fullPath;
            return nullptr;
        }
        mFontInst[fontSize] = font;
    }
    return mFontInst[fontSize];
}
