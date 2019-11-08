#pragma once

#include "Modules/ResourceSystem/Resource.hpp"

namespace vh
{

    class SkyBox : public Resource
    {
    public:
        SkyBox(const Utils::CubeMap& cubeMap) : mCubeMap(cubeMap) {}

    private:
        Utils::CubeMap mCubeMap;
    };

} // namespace vh