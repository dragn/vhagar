#include "PCH.hpp"
#include "GameScript.hpp"
#include "Views/GameView.hpp"
#include "GUI2D/GUI2D.hpp"
#include "ScriptStates.hpp"
#include "Views/WaitDialogView.hpp"

using namespace std::chrono_literals;

class GameScriptObj
{
public:
    GameScriptObj(GameProfile** profile)
        : mProfilePtr(profile)
        , mProfile(*profile)
        , mGui(vh::App::Get<gui::GUI2D>())
    {
        CHECK(mGui);
        CHECK(mProfilePtr);
        CHECK(mProfile);
    }

    void DoRun();
    void MOTG();
    void DoGameView(GameView* view);
    void WaitForDestroy(gui::View* view);

    bool IsDone();

private:
    GameProfile** mProfilePtr;
    GameProfile* mProfile;
    gui::GUI2D* mGui;

    void OnDestroy() { mDestroyed = true; mCv.notify_all(); }
    bool mDestroyed;

    std::condition_variable mCv;
    std::mutex mMtx;
};

void GameScriptObj::DoRun()
{
    LOG(INFO) << "GameScript started";

    while (!IsDone())
    {
        gui::View* view = mGui->GetView();
        if (view != nullptr && strcmp(view->GetName(), "GameView") == 0)
        {
            DoGameView(reinterpret_cast<GameView*>(view));
        }

        // TODO replace with condition_variable::wait_for
        std::this_thread::sleep_for(100ms);
    }

    LOG(INFO) << "GameScript finished";
}

void GameScriptObj::MOTG()
{
    // Check if we need to show MOTG
    if (mProfile->GetScriptState("MOTG"))
    {
        std::string dayStr = "DAY ";
        dayStr.append(std::to_string(mProfile->GetDay()));
        dayStr.append("\n<Message of the day>");
        DialogView* dialog = mGui->MakeModal<DialogView>("MOTG test");
        dialog->AddOption("Proceed").Set([this] ()
        {
            mProfile->SetScriptState("MOTG", false);
        });
        WaitForDestroy(dialog);
    }
}

void GameScriptObj::DoGameView(GameView* view)
{
    view->SetBackground("Assets/Images/garage.png");
    MOTG();
}

void GameScriptObj::WaitForDestroy(gui::View* view)
{
    std::unique_lock<std::mutex> lk(mMtx);
    mDestroyed = false;
    view->OnDestroy.Add(this, &GameScriptObj::OnDestroy);
    mCv.wait(lk, [this] { return mDestroyed; });
}

bool GameScriptObj::IsDone()
{
    return *mProfilePtr == nullptr;
}

void GameScript(GameProfile** profile)
{
    GameScriptObj obj(profile);
    obj.DoRun();
}
