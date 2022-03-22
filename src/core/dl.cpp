#include "dl.hpp"
#include <dlfcn.h>

bool dl::Handle::updateError()
{
    char *newDlerror = dlerror();
    if (newDlerror != nullptr) {
        this->lastError = newDlerror;
        return false;
    }
    return true;
}

void dl::Handle::load(std::string fileName)
{
    this->close();

    // Clear any error that may have occured previously
    dlerror();

    this->handle = fileName.size() != 0 ? dlopen(fileName.c_str(), this->getDlopenFlags()) : dlopen(NULL, this->getDlopenFlags());
    if (!this->updateError())
        return;
    this->fileName = fileName;
}

void *dl::Handle::lookupSymbol(std::string_view symbolName)
{
    if (this->handle == nullptr) {
        this->lastError = "Handle does not point to any library";
        return nullptr;
    }

    // Clear any error that may have occured previously
    dlerror();
    
    auto symbolAddr = dlsym(this->handle, symbolName.data());
    return this->updateError() ? symbolAddr : nullptr;
}

void dl::Handle::close()
{
    if (this->handle != nullptr)
        dlclose(this->handle);
    this->handle = nullptr;
}
