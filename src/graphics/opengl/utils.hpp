/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** utils
*/

#include <string.h>

namespace ogl {
    class Utils {
        public:
        static void copyMemory(void *dest, const void *src, size_t size) { memcpy(dest, src, size); };
    };
}