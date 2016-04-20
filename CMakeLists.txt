CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

project(glob)

if (MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static-libgcc")
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()

set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_IMPORT_LIBRARY_PREFIX "")

set(LIBSASS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." CACHE FILEPATH "Path to libsass directory")
set(LIBSASS_LIBRARY_DIR "${LIBSASS_DIR}/lib" CACHE FILEPATH "Path to libsass libraries")
set(LIBSASS_INCLUDE_DIR "${LIBSASS_DIR}/include" CACHE FILEPATH "Path to libsass includes")

link_directories("${LIBSASS_LIBRARY_DIR}" "${LIBSASS_DIR}")
include_directories("${LIBSASS_INCLUDE_DIR}" "${LIBSASS_DIR}" "vendor")

add_library(glob SHARED vendor/FS.cpp src/glob.cpp)

find_library(SASS_LIBRARY NAMES sass
    HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../lib"
)

target_include_directories(glob PUBLIC ../include ../boost)
target_link_libraries(glob LINK_PUBLIC sass)
