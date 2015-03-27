#pragma once

namespace Simple
{
    template <typename T>
    class SingletonHolder
    {
    public:
        SingletonHolder(const SingletonHolder&) = delete;
        SingletonHolder& operator=(const SingletonHolder&) = delete;
        static T& instance()
        {
            if (!instance_)
                instance_ = new T();

            return *instance_;
        }

    private:
        SingletonHolder() = default;

        static T* instance_;
    };

    template <typename T>
    T* SingletonHolder<T>::instance_ = nullptr;

}

