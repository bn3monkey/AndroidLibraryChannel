//
// Created by hansono_bn3monkey on 2024-10-22.
//

#ifndef ANDROIDNATIVELIBRARYCHANNEL_SERVERLIBRARYLISTENER_HPP
#define ANDROIDNATIVELIBRARYCHANNEL_SERVERLIBRARYLISTENER_HPP

#include <mutex>

template<typename ...Args>
class ServerLibraryListener
{
public:
    ServerLibraryListener() : _listener(nullptr) {}
    ServerLibraryListener(const ServerLibraryListener& other) = delete;
    virtual ~ServerLibraryListener() {}

    void add(void (*listener)(Args... args))
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _listener = listener;
        }
    }
    void clear() {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _listener = nullptr;
        }
    }

    void operator() (Args... args)
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            if (_listener)
            {
                _listener(std::forward<Args>(args)...);
            }
        }
    }

private:
    std::mutex _mtx;
    void (*_listener)(Args... args);
};

template<>
class ServerLibraryListener<void>
{
public:
    ServerLibraryListener() : _listener(nullptr) {}
    ServerLibraryListener(const ServerLibraryListener& other) = delete;
    virtual ~ServerLibraryListener() {}

    void add(void (*listener)())
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _listener = listener;
        }
    }
    void clear() {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _listener = nullptr;
        }
    }

    void operator() ()
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            if (_listener)
            {
                _listener();
            }
        }
    }

private:
    std::mutex _mtx;
    void (*_listener)();
};
#endif //ANDROIDNATIVELIBRARYCHANNEL_SERVERLIBRARYLISTENER_HPP
