#include "Vhagar.hpp"
#include <glm/gtx/transform.hpp>

using namespace vh;

class CustomOverlay : public Overlay
{
public:
    CustomOverlay(Uint32 shift) : mShift(shift) {};

    virtual void render(glm::mat4 projection, glm::mat4 view) {
        if (!isReadyToRender) return;

        glDisable(GL_CULL_FACE);
        glUseProgram(programID);

        Utils::putUniformVec4(programID, "uBounds", bounds);
        Utils::putUniformFloat(programID, "uTime", (mShift + SDL_GetTicks()) / 1000.0f);

        if (texId > 0) glBindTexture(GL_TEXTURE_2D, texId);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);

        glEnable(GL_CULL_FACE);
    }

protected:
    virtual const char* GetShader()
    {
        return "Flow";
    }
    Uint32 mShift;
};

class MyApp : public vh::App
{
public:
    MyApp() : mSpawned(false) {}

    void OnTick()
    {
        if (!mSpawned)
        {
            Renderer* render = GetComponent<Renderer>();
            if (render && render->IsRunning())
            {
                CustomOverlay* co = new CustomOverlay(3000);
                co->setBounds(Rect{0.25f, 0.25f, 1.5f, 1.5f});
                render->AddObject(co);

                /*
                co = new CustomOverlay(0);
                co->setBounds(Rect{0.25f, 0.25f, 1.5f, 1.5f});
                render->AddObject(co);

                co = new CustomOverlay(1500);
                co->setBounds(Rect{0.2f, 0.2f, 1.6f, 1.6f});
                render->AddObject(co);
                */

                mSpawned = true;
            }
        }
    }

private:
    virtual void HandleEvent(SDL_Event *event)
    {
        vh::App::HandleEvent(event);

        if (event->type == SDL_KEYUP)
        {
            SDL_Keycode k = event->key.keysym.sym;
            if (k == SDLK_ESCAPE)
            {
                event->type = SDL_QUIT;
                SDL_PushEvent(event);

            }
        }
    }

    bool mSpawned;
};

int main(int argc, char ** argv) {
    MyApp app;

    app.AddComponent<Renderer>();

    app.Run();
}
