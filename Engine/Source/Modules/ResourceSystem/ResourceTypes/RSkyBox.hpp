#pragma once

#include "Modules/ResourceSystem/MResourceSystem.hpp"

namespace vh
{
    /*
          //        \\
        RRRR SkyBox RRRR
          \\        //
    */
    class RSkyBox : public Resource
    {
        friend class MResourceSystem;

    public:
        Utils::CubeMap const& GetCubeMap() const { return mCubeMap; }

    private:
        Utils::CubeMap mCubeMap;
    };

    template<> bool MResourceSystem::Load(const char* path, std::shared_ptr<RSkyBox> skyBox);

} // namespace vh
