#pragma once

// C++ wrapper for libdl

#include <string>
#include <string_view>
#include <dlfcn.h>

namespace dl {
    enum class ResolvePolicyTime {
        lazy = RTLD_LAZY,
        now = RTLD_NOW,
    };

    enum class ResolvePolicyOption {
        none = 0,
        global = RTLD_GLOBAL,
        local = RTLD_LOCAL,
        no_delete = RTLD_NODELETE,
        no_load = RTLD_NOLOAD,
        deep_bind = RTLD_DEEPBIND,
    };
    
    class Handle {
        void *rawHandle = nullptr;
        std::string lastError;
        std::string fileName;

        bool updateError();
    public:
        Handle(std::string fileName)
        {
            this->load(fileName);
        }

        ~Handle()
        {
            this->close();
        }

        void load(std::string file_name);
        void *lookupSymbol(std::string_view symbolName);
        void close();
        
        dl::ResolvePolicyTime resolvePolicyTime = dl::ResolvePolicyTime::now;
        dl::ResolvePolicyOption resolvePolicyOptions = dl::ResolvePolicyOption::none;
        
        int getDlopenFlags() const
        {
            return static_cast<int>(this->resolvePolicyOptions) | static_cast<int>(this->resolvePolicyTime);
        }

        bool isLoaded() const
        {
            return this->rawHandle != nullptr;
        }

        std::string_view getLastError() const
        {
            return this->lastError;
        }
    };
};
