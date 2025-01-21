#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include <utility>
#include <stdexcept>

namespace hyper::framework
{
    template <typename CreatorType>
    class Factory
    {
    public:
        Factory(const Factory &) = delete;
        Factory &operator=(const Factory &) = delete;

        static Factory &instance()
        {
            static Factory instance;
            return instance;
        }

        template <typename F>
        void register_type(const std::string &key, F &&creator)
        {
            if (!creators_.try_emplace(key, std::forward<F>(creator)).second)
            {
                throw std::runtime_error("Type already registered with key: " + key);
            }
        }

        template <typename... Args>
        decltype(auto) create(const std::string &key, Args &&...args) const
        {
            auto it = creators_.find(key);
            if (it == creators_.end())
            {
                throw std::runtime_error("No creator registered with key: " + key);
            }
            return it->second(std::forward<Args>(args)...);
        }

    private:
        Factory() = default;

        std::unordered_map<std::string, CreatorType> creators_;
    };
}
