#pragma once

#include "Modules/ResourceSystem/Resource.hpp"
#include "Modules/ResourceSystem/MResourceSystem.hpp"
#include "Modules/standard.hpp"

namespace vh
{
    /*
          //      \\
        RRRR Font RRRR
          \\      //
    */
    class RFont : public Resource
    {
        friend MResourceSystem;

    public:
        virtual ~RFont();

        TTF_Font* GetFont(uint32_t fontSize);

    private:
        std::string mFontResPath;
        std::unordered_map<uint32_t, TTF_Font*> mFontInst;
    };

    template<> bool MResourceSystem::Load(const char* path, std::shared_ptr<RFont> font);
} // namespace vh
