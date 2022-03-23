#include <iostream>
#include <stdexcept>
#include "arcade-interface/IDisplayModule.hpp"
#include "dl.hpp"
#include "coreImpl.hpp"

static void usage(const char *argv0)
{
    std::cerr << "Usage: " << argv0 << " <graphics-library>\n"
        "Starts the arcade with the given graphics library\n";
    std::exit(84);
}

int program(int argc, char **argv)
{
    if (argc != 2)
        usage(argv[0]);

    dl::Handle initGraphicsLib{argv[1]};
    if (!initGraphicsLib.isLoaded())
        throw std::runtime_error(std::string(initGraphicsLib.getLastError()));
    auto gEpitechArcadeGetDisplayModuleHandlePtr =
        reinterpret_cast<decltype(&gEpitechArcadeGetDisplayModuleHandle)>(
            initGraphicsLib.lookupSymbol("gEpitechArcadeGetDisplayModuleHandle"));
    if (gEpitechArcadeGetDisplayModuleHandlePtr == nullptr)
        throw std::runtime_error(std::string(initGraphicsLib.getLastError()));

    CoreImpl core;
    core.changeDisplayModule(gEpitechArcadeGetDisplayModuleHandlePtr());
    core.initLibraryLists();
    core.runMenu();
    core.runGame();
    return (0);
}

int main(int argc, char **argv)
{
    try {
        return program(argc, argv);
    } catch (std::exception &e) {
        std::cerr << "Error (stdexcept): " << e.what() << '\n';
        return 84;
    } catch (...) {
        std::cerr << "Error: Unknown exception !!!!!!\n";
        return 84;
    }
}
