#pragma once

#include "Modules/ResourceSystem/Resource.hpp"

namespace vh
{

    class RSkyBox_GL : public Resource
    {
    public:
        RSkyBox_GL(const Utils::CubeMap& cubeMap) : mCubeMap(cubeMap) {}

    private:
        Utils::CubeMap mCubeMap;
    };

} // namespace vh