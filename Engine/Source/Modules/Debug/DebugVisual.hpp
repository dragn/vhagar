#pragma once

#include "Modules/standard.hpp"

namespace vh
{
    class MRenderer3D_Old;

    struct DebugLabel
    {
        V3 pos;
        const char* text;
    };

    class DebugVisual
    {
    public:
        DebugVisual() : mGLBuf(0) {}
        ~DebugVisual();

        std::vector<DebugLabel>& GetLabels() { return mLabels; };

        void Init();
        void Destroy();
        void Render(const MRenderer3D_Old* renderer);

    private:
        std::vector<DebugLabel> mLabels;

        GLuint mProgramId;
        GLuint mGLBuf;

        GLint uidColor;
        GLuint uidMVP;
    };

} // namespace vh
