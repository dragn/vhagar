#pragma once

namespace vh
{
    class RTexture : public Resource
    {
        friend MResourceSystem;

    public:
        virtual ~RTexture();

    private:
        RTexture();

        SDL_Surface* mSurface;
        GLuint mTexId = 0;
    };

    template<> bool MResourceSystem::Load(const char* path, std::shared_ptr<RTexture> font);
} // namespace vh
