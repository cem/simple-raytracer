cmake_minimum_required (VERSION 2.6)
project (raytracer)

add_subdirectory(src/third-party/zlib)
set(ZLIB_LIBRARY zlibstatic)
set(ZLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/third-party/zlib
                     ${CMAKE_CURRENT_BINARY_DIR}/src/third-party/zlib)

set(PNG_SHARED OFF)
set(PNG_STATIC ON)
add_subdirectory(src/third-party/libpng)
configure_file(src/third-party/libpng/scripts/pnglibconf.h.prebuilt
               ${CMAKE_CURRENT_BINARY_DIR}/pnglibconf.h)
include_directories(src/third-party/libpng
                    ${CMAKE_CURRENT_BINARY_DIR})

add_executable(raytracer src/image.cpp
					     src/input_format.cpp
					     src/main.cpp
					     src/plane.cpp
					     src/scene.cpp
					     src/scene_base.cpp
					     src/sphere.cpp
					     src/triangle.cpp
					     src/v3.cpp)
target_link_libraries(raytracer LINK_PUBLIC png16_static)
