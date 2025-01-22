#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include <utility>
#include <stdexcept>
#include <concepts>

namespace hyper::framework
{
    template <typename CreatorType>
    class Factory
    {
    public:
        Factory(const Factory &) = delete;
        Factory &operator=(const Factory &) = delete;
        Factory(Factory &&) = delete;
        Factory &operator=(Factory &&) = delete;

        static Factory &instance()
        {
            static Factory instance;
            return instance;
        }

        template <typename F>
        /*requires std::invocable<F>*/
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
            if (auto it = creators_.find(key); it != creators_.end())
            {
                return std::invoke(it->second, std::forward<Args>(args)...);
            }
            else
            {
                throw std::runtime_error("No creator registered with key: " + key);
            }
        }

    private:
        Factory() = default;
        ~Factory() = default;

        std::unordered_map<std::string, CreatorType> creators_;
    };
}
