#pragma once

#include "App/App.hpp"
#include "Actor/ActorBehavior.hpp"
#include "Renderer/Renderer.hpp"

namespace vh
{

template<eRenderBlockType TYPE, typename RENDERABLE_TYPE>
class RenderableBehavior : public ActorBehavior
{
public:
    RenderableBehavior(Actor* owner) : ActorBehavior(owner) {}

    virtual void StartPlay() override
    {
        mRenderer = App::Get<Renderer>();

        if (mRenderable == nullptr)
        {
            LOG(ERROR) << "renderable is not set!";
        }
        else
        {
            mRenderer->AddLoadTask(mRenderable);
        }
    }

    virtual void EndPlay() override
    {
        if (mRenderable != nullptr)
        {
            mRenderer->AddReleaseTask(mRenderable);
        }
    }

    virtual void Tick(uint32_t delta) override
    {
        // start rendering as soon as it's loaded (in render thread)
        if (mRenderable && mRenderable->IsLoaded())
        {
            RenderBlock* block = mRenderer->GetBufferHandler().AllocateNewBlock();
            if (block == nullptr) return;

            block->flags = eRenderBlockFlags::Active;
            block->flags |= eRenderBlockFlags::Interpolated;
            block->type = TYPE;
            SetupPayload(reinterpret_cast<typename RENDERABLE_TYPE::Payload*>(&block->payload));
        }
    }

    virtual void SetupPayload(typename RENDERABLE_TYPE::Payload* payload) = 0;

    void Set(RENDERABLE_TYPE* renderable)
    {
        mRenderable = renderable;
    }

    RENDERABLE_TYPE* Get() const
    {
        return mRenderable;
    }

protected:
    Renderer* mRenderer;
    RENDERABLE_TYPE* mRenderable = nullptr;
};

} // namespace vh
