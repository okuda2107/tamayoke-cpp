# toolchainで指定しなければならない変数
# - CMAKE_INCLUDE_PATH: 外部ライブラリのinclude path
# - CMAKE_LIBRARY_PATH: 外部ライブラリのlib path
# - CMAKE_LIBRARIES: 外部ライブラリのlibファイル名
# - DLL_FILES: 外部ライブラリの動的ライブラリのpath

cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(prj CXX)

add_subdirectory("engine")
# add_subdirectory("engine_temp")

add_subdirectory("demo")

# engineができてない状態でもtestしたいため，一旦コメントアウト
# todo: ルートからコマンドを叩いて，テストを行いたい．
# add_subdirectory("test")

set(RELEASE_DIR "${CMAKE_SOURCE_DIR}/Release")
add_custom_target(Release ALL
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${RELEASE_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${RELEASE_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:app> ${RELEASE_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:app> ${RELEASE_DIR}
    COMMAND_EXPAND_LISTS
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL_FILES} ${RELEASE_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Assets" "${RELEASE_DIR}/Assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Shaders" "${RELEASE_DIR}/Shaders"
    DEPENDS app
)
