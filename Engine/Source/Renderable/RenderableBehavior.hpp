#pragma once

#include "App/App.hpp"
#include "Actor/ActorBehavior.hpp"
#include "Renderer/Renderer.hpp"
#include "Resource/ResourceSystem.hpp"

namespace vh
{

template<eRenderBlockType TYPE, typename RENDERABLE_TYPE>
class RenderableBehavior : public ActorBehavior
{
public:
    RenderableBehavior()
    {
        // -- create default resource object if default constructor exists
        mResource = std::make_shared<RENDERABLE_TYPE>();
    }

    RenderableBehavior(std::shared_ptr<RENDERABLE_TYPE>& resource)
    {
        // -- use existing resource object
        CHECK(resource) << "resource is null";

        mResource = resource;
    }

    RenderableBehavior(const char* resourceName)
    {
        // -- request resource from resource system
        CHECK(resourceName) << "resource name is null";

        ResourceSystem* rs = App::Get<ResourceSystem>();
        if (rs)
        {
            mResource = rs->GetResource<RENDERABLE_TYPE>(resourceName);
            if (!mResource)
            {
                LOG(WARNING) << "Could not load resource: " << resourceName;
            }
        }
    }

    virtual void StartPlay() override
    {
        mRenderer = App::Get<Renderer>();

        if (mResource == nullptr)
        {
            LOG(ERROR) << "renderable is not set!";
        }
        else
        {
            mRenderer->AddLoadTask(mResource);
        }
    }

    virtual void EndPlay() override
    {
        if (mResource != nullptr)
        {
            mRenderer->AddReleaseTask(mResource);
        }
    }

    virtual void Tick(uint32_t delta) override
    {
        // start rendering as soon as it's loaded (in render thread)
        if (mResource && mResource->IsLoaded())
        {
            RenderBlock* block = mRenderer->GetBufferHandler().AllocateNewBlock();
            if (block == nullptr) return;

            block->flags = GetFlags();
            block->type = TYPE;
            SetupPayload(reinterpret_cast<typename RENDERABLE_TYPE::Payload*>(&block->payload));
        }
    }

    virtual uint16_t GetFlags()
    {
        return eRenderBlockFlags::Active;
    }

    virtual void SetupPayload(typename RENDERABLE_TYPE::Payload* payload) = 0;

    std::shared_ptr<RENDERABLE_TYPE> Get() const
    {
        return mResource;
    }

protected:
    Renderer* mRenderer;
    std::shared_ptr<RENDERABLE_TYPE> mResource;
};

} // namespace vh
