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
    this->rawHandle.reset();

    // Clear any error that may have occured previously
    dlerror();

    this->rawHandle.reset(fileName.size() != 0 ? dlopen(fileName.c_str(), this->getDlopenFlags()) : dlopen(NULL, this->getDlopenFlags()));
    if (!this->updateError())
        return;
    this->fileName = fileName;
}

void *dl::Handle::lookupSymbol(std::string_view symbolName)
{
    if (this->rawHandle == nullptr) {
        this->lastError = "Handle does not point to any library";
        return nullptr;
    }

    // Clear any error that may have occured previously
    dlerror();
    
    auto symbolAddr = dlsym(this->rawHandle.get(), symbolName.data());
    return this->updateError() ? symbolAddr : nullptr;
}

