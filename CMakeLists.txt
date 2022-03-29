cmake_minimum_required(VERSION 3.12)

macro(arcade_set_common_target_properties target_name)
  set_target_properties(${target_name}
    PROPERTIES
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF

      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/"
      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"

      PREFIX ""
  )

  target_include_directories(${target_name}
    PRIVATE "${CMAKE_SOURCE_DIR}/include"
  )
endmacro()

project(arcade CXX)

add_executable(arcade
  "src/core/main.cpp"
  "src/core/dl.cpp"
  "src/core/coreImpl.cpp"
  "src/core/menuGame.cpp"
)
target_link_libraries(arcade dl stdc++fs) # Required on old versions of glibc and GCC
arcade_set_common_target_properties(arcade)

add_library(arcade_pacman SHARED
  "src/games/pacman/main.cpp"
  "src/games/pacman/pacmanGameModule.cpp"
)
arcade_set_common_target_properties(arcade_pacman)

add_library(arcade_nibbler SHARED
  "src/games/nibbler/main.cpp"
  "src/games/nibbler/nibblerGameModule.cpp"
  "src/games/nibbler/arena.cpp"
  "src/games/nibbler/textures.cpp"
  "src/games/nibbler/nibblerUtils.cpp"
)
arcade_set_common_target_properties(arcade_nibbler)

add_library(arcade_ncurses SHARED
  "src/graphics/ncurses/main.cpp"
)
arcade_set_common_target_properties(arcade_ncurses)

add_library(arcade_sdl2 SHARED
  "src/graphics/sdl2/main.cpp"
)
arcade_set_common_target_properties(arcade_sdl2)

add_library(arcade_sfml SHARED
  "src/graphics/sfml/main.cpp"
  "src/graphics/sfml/sfmlDisplay.cpp"
  "src/graphics/sfml/sfmlRawTexture.cpp"
  "src/graphics/sfml/sfmlConstants.cpp"
)
target_link_libraries(arcade_sfml sfml-graphics sfml-system sfml-window)
target_link_libraries(arcade_sfml stdc++fs) # Required on old versions of GCC
arcade_set_common_target_properties(arcade_sfml)
