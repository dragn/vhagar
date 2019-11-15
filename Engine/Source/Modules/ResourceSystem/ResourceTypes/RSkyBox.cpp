#include "Modules/VhModules_PCH.hpp"
#include "RSkyBox.hpp"

template<>
bool vh::MResourceSystem::Load(const char* path, std::shared_ptr<RSkyBox> skyBox)
{
    std::ifstream file(GetFullPath(path));
    Utils::CubeMap cubeMap;

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    if (lines.size() != 6)
    {
        LOG(FATAL) << "Resource file format error: " << path;
        return false;
    }

    skyBox->mCubeMap.pos_x = lines[0];
    skyBox->mCubeMap.neg_x = lines[1];
    skyBox->mCubeMap.pos_y = lines[2];
    skyBox->mCubeMap.neg_y = lines[3];
    skyBox->mCubeMap.pos_z = lines[4];
    skyBox->mCubeMap.neg_z = lines[5];

    return true;
}
