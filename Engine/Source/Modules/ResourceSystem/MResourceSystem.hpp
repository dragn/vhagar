#pragma once

#include "Modules/ResourceSystem/Resource.hpp"
#include "Modules/standard.hpp"

namespace vh
{

    class MResourceSystem : public Module
    {
        VH_MODULE(MResourceSystem);

    public:
        MResourceSystem() : Module() {}

        template<typename T>
        bool Load(const char* path, std::shared_ptr<T> res);

        template<typename T>
        bool Save(const char* path, std::shared_ptr<const T> res);

        template<typename T>
        std::shared_ptr<T> GetResource(const char* name)
        {
            auto iter = mStorage.find(name);
            if (iter == mStorage.end())
            {
                std::shared_ptr<T> ptr = std::make_shared<T>();
                if (!ptr)
                {
                    LOG(FATAL) << "Unable to allocate resource";
                    return nullptr;
                }
                mStorage[std::string(name)] = ptr;
                if (Load<T>(name, ptr))
                {
                    LOG(INFO) << "Loaded resource " << name;
                    return ptr;
                }
                else
                {
                    LOG(ERROR) << "Unable to load resource " << name;
                    return nullptr;
                }
            }
            else
            {
                // FIXME no type safety!!
                return std::static_pointer_cast<T>(iter->second);
            }
        }

        virtual Ret TickInit(uint32_t delta) override;
        virtual Ret TickClose(uint32_t delta) override;

        std::string GetFullPath(const char* relPath);

    private:
        std::unordered_map<std::string, std::shared_ptr<Resource>> mStorage;

        std::string mCurrDir;
    };

    template<> bool MResourceSystem::Load(const char* path, std::shared_ptr<Actor> actor);
    template<> bool MResourceSystem::Save(const char* path, std::shared_ptr<const Actor> actor);

} // namespace vh
