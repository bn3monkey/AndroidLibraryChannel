//
// Created by hansono_bn3monkey on 2024-10-22.
//

#ifndef ANDROIDNATIVELIBRARYCHANNEL_DATASOURCELIBRARYLISTENER_HPP
#define ANDROIDNATIVELIBRARYCHANNEL_DATASOURCELIBRARYLISTENER_HPP

#include <mutex>

template<typename ...Args>
class DataSourceLibraryListener
{
public:
    DataSourceLibraryListener() : _user_context(nullptr), _listener(nullptr), _releaseUserContext(nullptr) {}
    DataSourceLibraryListener(const DataSourceLibraryListener& other) = delete;
    virtual ~DataSourceLibraryListener() {
        clear();
    }

    void add(void (*listener)(void* user_context, Args... args))
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _user_context = nullptr;
            _listener = listener;

        }
    }
    void add(void* user_context, void (*listener)(void* user_context, Args... args), void (*releaseUserContext)(void* user_context))
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _user_context = user_context;
            _listener = listener;
            _releaseUserContext = releaseUserContext;
        }
    }
    void clear() {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            if (_releaseUserContext)
            {
                if (_user_context) {
                    _releaseUserContext(_user_context);
                }
            }
            _releaseUserContext = nullptr;
            _listener = nullptr;
            _user_context = nullptr;
        }
    }

    void operator() (Args... args)
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            if (_listener)
            {
                _listener(_user_context, std::forward<Args>(args)...);
            }
        }
    }

private:
    std::mutex _mtx;
    void* _user_context {nullptr};
    void (*_listener)(void* user_context, Args... args);
    void (*_releaseUserContext)(void* user_context);
};

template<>
class DataSourceLibraryListener<void>
{
public:
    DataSourceLibraryListener() : _user_context(nullptr), _listener(nullptr), _releaseUserContext(nullptr) {}
    DataSourceLibraryListener(const DataSourceLibraryListener& other) = delete;
    virtual ~DataSourceLibraryListener() {
        clear();
    }

    void add(void (*listener)(void* user_context))
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _user_context = nullptr;
            _listener = listener;
            _releaseUserContext = nullptr;
        }
    }
    void add(void* user_context, void (*listener)(void* user_context), void (*releaseUserContext)(void* user_context))
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            _user_context = user_context;
            _listener = listener;
            _releaseUserContext = releaseUserContext;
        }
    }
    void clear() {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            if (_releaseUserContext)
            {
                if (_user_context) {
                    _releaseUserContext(_user_context);
                }
            }
            _releaseUserContext = nullptr;
            _listener = nullptr;
            _user_context = nullptr;
        }
    }

    void operator() ()
    {
        {
            std::lock_guard<std::mutex> lock(_mtx);
            if (_listener)
            {
                _listener(_user_context);
            }
        }
    }

private:
    std::mutex _mtx;
    void* _user_context;
    void (*_listener)(void* user_context);
    void (*_releaseUserContext)(void* user_context);
};
#endif //ANDROIDNATIVELIBRARYCHANNEL_DataSourceLibraryListener_HPP
