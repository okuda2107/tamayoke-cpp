get_filename_component(TOOLCHAIN_DIR "${CMAKE_TOOLCHAIN_FILE}" DIRECTORY)
set(ROOT_DIR "${TOOLCHAIN_DIR}/..")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

set(CMAKE_INCLUDE_PATH
    "/usr/include/SDL2"
    "/usr/include/SOIL"
    "/usr/include/GL"
    "/usr/include/rapidjson"
    "/usr/include/AL"
)

# set(CMAKE_LIBRARY_PATH
#     "${CMAKE_SOURCE_DIR}/exlibs/xxx""
# )

set(CMAKE_LIBRARIES
    "SDL2"
    "SDL2_image"
    "SDL2_mixer"
    "SDL2_net"
    "SDL2_ttf"
    "SOIL"
    "GL"
    "GLEW"
    "openal"
    "alut"
)

set(DLL_FILES
    "/usr/lib/x86_64-linux-gnu/libSDL2.so"
    "/usr/lib/x86_64-linux-gnu/libalut.so"
)
