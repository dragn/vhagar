#pragma once

#include "Modules/standard.hpp"

namespace vh
{
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

    private:
        std::vector<DebugLabel> mLabels;

        GLuint mProgramId;
        GLuint mGLBuf;

        GLint uidColor;
        GLuint uidMVP;
    };

} // namespace vh
