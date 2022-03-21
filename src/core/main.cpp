#include <iostream>

static void usage(const char *argv0)
{
    std::cerr << "Usage: " << argv0 << " <graphics-library>\n"
        "Starts the arcade with the given graphics library\n";
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        usage(argv[0]);
}
