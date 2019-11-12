#pragma once

#include "Core/Actor/Actor.hpp"
#include "Core/Actor/ActorFactory.hpp"
#include "Core/App/Module.hpp"
#include "Core/standard.hpp"

namespace vh
{
    /*

       MWorld module interface.

       MWorld is a module that provides a container for Actors - dynamic entities
       consisting of Behaviors.

   */
    class MWorld : public Module
    {
        VH_MODULE(MWorld);

    public:
        MWorld();

        virtual Ret TickInit(uint32_t delta) override;
        virtual Ret TickRun(uint32_t delta) override;
        virtual Ret TickClose(uint32_t delta) override;

        void DestroyActor(ActorID id)
        {
            decltype(mActors)::iterator it = std::find_if(mActors.begin(), mActors.end(), [id](const std::shared_ptr<Actor>& actor)
                {
                    return actor->mId == id;
                });
            if (it != mActors.end())
            {
                std::shared_ptr<Actor>& pActor = *it;
                CHECK(pActor.unique()) << "Attempt to delete locked actor";
                pActor->EndPlay();
                if (pActor != mActors.back())
                {
                    std::swap(*it, mActors.back());
                }
                mActors.pop_back();
            }
        }

        // TODO Switch to visitor pattern
        const std::vector<std::shared_ptr<Actor>>& GetActors() const
        {
            return mActors;
        }

        std::weak_ptr<Actor> GetActorByName(const std::string& name) const
        {
            for (const std::shared_ptr<Actor>& actor : mActors)
            {
                if (actor->GetName() == name)
                {
                    return std::weak_ptr<Actor>(actor);
                }
            }
            return std::weak_ptr<Actor>();
        }

        std::weak_ptr<Actor> CreateActor(const std::string& name);

        void ClearWorld();

        struct BehaviorResponse
        {
            std::shared_ptr<Actor> mActor;
            std::shared_ptr<ActorBehavior> mBehavior;
            ModuleID mModuleID;
        };

        template<typename BEHAVIOR_TYPE>
        void RegisterBehavior(const std::shared_ptr<Actor>& actor, const std::shared_ptr<BEHAVIOR_TYPE>& behavior)
        {
            // register modules responses
            for (std::unique_ptr<Module>& module : mModules)
            {
                if (module->RespondsTo<BEHAVIOR_TYPE>())
                {
                    BehaviorResponse br;
                    br.mActor = actor;
                    br.mBehavior = behavior;
                    br.mModuleID = module::GetID();
                    mBehaviorResponses.push_back(br);
                }
            }
        }

        void UnregisterBehaviors(const std::shared_ptr<Actor>& actor)
        {
            mBehaviorResponses.erase(std::remove_if(mBehaviorResponses.begin(), mBehaviorResponses.end(),
                [&actor](const BehaviorResponse& response)
            {
                return response.mActor == actor;
            }), mBehaviorResponses.end());
        }

    private:
        std::vector<std::shared_ptr<Actor>> mActors;
        ActorFactory mActorFactory;

        ActorID GenerateActorID() { return mActorID++; }
        ActorID mActorID = 0;

        std::vector<BehaviorResponse> mBehaviorResponses;

        UNCOPYABLE(MWorld);
    };

} // namespace vh
