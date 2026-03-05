add_library(engine STATIC)

file(GLOB_RECURSE CXX_FILES CONFIGURE_DEPENDS
    "src/**/*.cpp"
    "src/*.cpp"
)

file(GLOB_RECURSE PRIVATE_HEADERS CONFIGURE_DEPENDS
    "src/**/*.h"
)

file(GLOB_RECURSE PUBLIC_HEAKERS CONFIGURE_DEPENDS
    "include/**/*.h"
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

target_sources(engine
    PRIVATE
        ${CXX_FILES}

    PRIVATE
        FILE_SET privateHeaders
        TYPE HEADERS
        BASE_DIRS
            "src"
        FILES
            ${PRIVATE_HEADERS}

    PUBLIC
        FILE_SET publicHeaders
        TYPE HEADERS
        BASE_DIRS
            "include"
        FILES
            ${PUBLIC_HEADERS}
)

target_link_libraries(engine
    PRIVATE
        SDL2::SDL2main
    PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL2_ttf
        SDL2_net::SDL2_net
        GLEW::glew
)

# OpenAL
if (OPENAL_INCLUDE_DIR AND OPENAL_LIBRARY AND OPENAL_DLL)
    # find_packageでパス解決しようとすると，alut側のinclude pathの解決ができないので，手動で指定
    target_include_directories(engine
        PUBLIC
            ${OPENAL_INCLUDE_DIR}
    )
    target_link_libraries(engine
        PRIVATE
            ${OPENAL_LIBRARY}
    )
    foreach(dll IN LISTS OPENAL_DLL)
        get_filename_component(dll_name "${dll}" NAME_WE)
        add_library(${dll_name} SHARED IMPORTED)
        set_target_properties(${dll_name} PROPERTIES
            IMPORTED_LOCATION "${dll}"
            IMPORTED_IMPLIB ${OPENAL_LIBRARY}
            INTERFACE_INCLUDE_DIRECTORIES ${OPENAL_INCLUDE_DIR}
        )
        target_link_libraries(engine PRIVATE ${dll_name})
    endforeach()
else()
    message(WARNING "not set OPENAL_INCLUDE_PATH, or _LIBRARY, or _DLL")
endif()

# alut
if (ALUT_INCLUDE_DIR AND ALUT_LIBRARY AND ALUT_DLL)
    # find_packageでパス解決しようとすると，alut側のinclude pathの解決ができないので，手動で指定
    target_include_directories(engine
        PUBLIC
            ${ALUT_INCLUDE_DIR}
    )
    target_link_libraries(engine
        PRIVATE
            ${ALUT_LIBRARY}
    )
    foreach(dll IN LISTS ALUT_DLL)
        get_filename_component(dll_name "${dll}" NAME_WE)
        add_library(${dll_name} SHARED IMPORTED)
        set_target_properties(${dll_name} PROPERTIES
            IMPORTED_LOCATION "${dll}"
            IMPORTED_IMPLIB ${ALUT_LIBRARY}
            INTERFACE_INCLUDE_DIRECTORIES ${ALUT_INCLUDE_DIR}
        )
        target_link_libraries(engine PRIVATE ${dll_name})
    endforeach()
else()
    message(WARNING "not set ALUT_INCLUDE_PATH, or _LIBRARY, or _DLL")
endif()

# rapidjson
if (RAPIDJSON_INCLUDE_DIR)
    # rapidjsonはbuildがコケるので，とりあえずヘッダファイルのみ
    target_include_directories(engine
        PUBLIC
            ${RAPIDJSON_INCLUDE_DIR}
    )
else()
    message(WARNING "not set RAPIDJSON_INCLUDE_DIR")
endif()

# SOIL
if (SOIL_INCLUDE_DIR AND SOIL_LIBRARY)
    # find_packageでパス解決しようとすると，alut側のinclude pathの解決ができないので，手動で指定
    target_include_directories(engine
        PUBLIC
            ${SOIL_INCLUDE_DIR}
    )
    target_link_libraries(engine
        PRIVATE
            ${SOIL_LIBRARY}
            OpenGL::GL
    )
else()
    message(WARNING "not set SOIL_INCLUDE_PATH, or _LIBRARY")
endif()
