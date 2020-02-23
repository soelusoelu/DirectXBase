#pragma once

#include <mutex>
#include <cassert>

class SingletonFinalizer {
public:
    typedef void(*FinalizerFunc)();
    static void addFinalizer(FinalizerFunc func);
    static void finalize();
};

template<typename T>
class Singleton final {
public:
    static T& instance() {
        std::call_once(mInitFlag, create);
        assert(mInstance);
        return *mInstance;
    }


private:
    static void create() {
        mInstance = new T;
        SingletonFinalizer::addFinalizer(&Singleton<T>::destroy);
    }

    static void destroy() {
        delete mInstance;
        mInstance = nullptr;
    }

    static T* mInstance;
    static std::once_flag mInitFlag;

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};

template <typename T> std::once_flag Singleton<T>::mInitFlag;
template <typename T> T* Singleton<T>::mInstance = nullptr;
