#pragma once

#include "Modules/Console/MConsoleGUI.hpp"
#include "Modules/standard.hpp"
#include "Modules/World/ControlBehavior.hpp"

namespace vh {

    /*
          //                  \\
        MMMM PlayerController MMMM
          \\                  //

        PlayerController handles input events from human.
        PlayerController has a camera, that specifies what the player sees.
     */
    class MPlayerController : public Module
    {
        VH_MODULE(MPlayerController);

    public:
        MPlayerController();

        // Take possession of the object
        void Control(std::weak_ptr<Actor> actor);

        // Release previously possessed object
        void Release();

        virtual void HandleEvent(SDL_Event *event);

        virtual Ret TickInit(uint32_t delta) override;
        virtual Ret TickRun(uint32_t delta) override;

        std::weak_ptr<Actor> GetControlledActor()
        {
            return mActor;
        }

    private:
        MConsoleGUI* mConsole;
        std::weak_ptr<Actor> mActor;

        float mCameraTurnSpeed;

        void _HandleKey(uint32_t type, SDL_KeyboardEvent *event);
        void _HandleMouse(int32_t xrel, int32_t yrel);

        bool mPressed[128];
    };

} // namespace vh
