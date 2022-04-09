cmake_minimum_required(VERSION 3.12)

set(CMAKE_BUILD_TYPE Debug)

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
  "src/games/nibbler/phase.cpp"
  "src/games/nibbler/initPhase.cpp"
  "src/games/nibbler/gamePhase.cpp"
  "src/games/nibbler/deathPhase.cpp"
  "src/games/nibbler/arena.cpp"
  "src/games/nibbler/levels.cpp"
  "src/games/nibbler/textures.cpp"
  "src/games/nibbler/score.cpp"
  "src/games/nibbler/nibblerUtils.cpp"
)
arcade_set_common_target_properties(arcade_nibbler)

add_library(arcade_ncurses SHARED
  "src/graphics/ncurses/main.cpp"
  "src/graphics/ncurses/ncursesDisplay.cpp"
  "src/graphics/ncurses/ncursesTexture.cpp"
)
target_link_libraries(arcade_ncurses curses)
arcade_set_common_target_properties(arcade_ncurses)

add_library(arcade_sdl2 SHARED
  "src/graphics/sdl2/main.cpp"
  "src/graphics/sdl2/sdlDisplay.cpp"
  "src/graphics/sdl2/sdlRawTexture.cpp"
  "src/graphics/sdl2/sdlConstants.cpp"
)
target_link_libraries(arcade_sdl2 SDL2 SDL2_image SDL2_ttf)
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

find_package(Freetype REQUIRED)

add_library(arcade_opengl SHARED
  "src/graphics/opengl/main.cpp"
  "src/graphics/opengl/oglDisplay.cpp"
  "src/graphics/opengl/shader.cpp"
  "src/graphics/opengl/texture.cpp"
  "src/graphics/opengl/font.cpp"
  "src/graphics/opengl/oglConstants.cpp"
)
target_link_libraries(arcade_opengl sfml-graphics sfml-system sfml-window GL GLU GLEW ${FREETYPE_LIBRARIES} png)
target_include_directories(arcade_opengl PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(arcade_opengl stdc++fs) # Required on old versions of GCC
set_target_properties(arcade_opengl PROPERTIES EXCLUDE_FROM_ALL TRUE)
arcade_set_common_target_properties(arcade_opengl)
