#pragma once

namespace Meyers
{
    template <typename T>
    class SingletonHolder
    {
    public:
        SingletonHolder(const SingletonHolder&) = delete;
        SingletonHolder& operator=(const SingletonHolder&) = delete;

        static T& instance()
        {
            static T unique_instance;

            return unique_instance;
        }
    private:
        SingletonHolder() = default;
    };

}
