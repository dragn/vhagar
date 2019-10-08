#include "Vhagar_PCH.hpp"
#include "SkyBoxBehavior.hpp"
#include "Core/App.hpp"

using namespace vh;

namespace
{
const GLfloat vertexData[] = { -1, -1, -1, -1, -1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1, -1, 1, -1, 1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, -1, -1, 1, -1, -1, 1, 1, -1, 1, -1, -1, -1, -1, 1, -1, -1, 1, -1, -1, -1, 1, -1, 1, 1, -1, 1, 1, 1, -1, -1, 1, 1, 1, -1 };
const GLuint vertexDataSize = 72;
const GLuint indexData[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 0, 2, 19, 3, 5, 20, 6, 8, 21, 9, 11, 22, 12, 14, 23, 15, 17 };
const GLuint indexDataSize = 36;
}

void vh::SkyBoxBehavior::SetupPayload(typename SkyBox::Payload* payload)
{
    payload->progId = mResource->mShaderId;
    payload->info = mResource->mGLInfo;
}

