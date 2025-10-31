cmake_minimum_required(VERSION 3.16)
project(dds520)
set(CMAKE_CXX_STANDARD 20)
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

if(EMSCRIPTEN)
    set(ASSETS_PATH "/assets")
    add_definitions("-sUSE_LIBPNG -fexceptions")
else()
    set(ASSETS_PATH "./assets")
endif()

find_package(Git QUIET)
if(GIT_FOUND)
    message(STATUS "Git found: initializing/updating submodules...")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE _git_submodule_result
        OUTPUT_QUIET ERROR_QUIET
    )
    if(NOT _git_submodule_result EQUAL 0)
        message(WARNING "git submodule update --init --recursive failed with exit code ${_git_submodule_result}")
    endif()
else()
    message(STATUS "Git not found; skipping automatic git submodule initialization")
endif()

# Emscripten has it's own version of libpng
if(EMSCRIPTEN)
else()
    set(ZLIB_BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_SHARED ON CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_STATIC ON CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_MINIZIP OFF CACHE BOOL "" FORCE)
    set(ZLIB_ROOT "${CMAKE_SOURCE_DIR}/third_party/zlib")
    add_subdirectory(third_party/zlib EXCLUDE_FROM_ALL)

    set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib/")
    set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})
    set(ZLIB_LIBRARY zlib)
    set(PNG_SHARED OFF CACHE BOOL "" FORCE)
    set(PNG_STATIC ON CACHE BOOL "" FORCE)
    set(PNG_TOOLS OFF CACHE BOOL "" FORCE)
    set(PNG_BUILD_ZLIB OFF CACHE BOOL "" FORCE)
    set(PNG_TESTS OFF CACHE BOOL "" FORCE)
    add_subdirectory(third_party/libpng EXCLUDE_FROM_ALL)
endif()

add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)

set(YAML_CPP_BUILD_CONTRIB ON CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "" FORCE)
set(YAML_CPP_FORMAT_SOURCE OFF CACHE BOOL "" FORCE)
set(YAML_CPP_DISABLE_UNINSTALL ON CACHE BOOL "" FORCE)
set(YAML_USE_SYSTEM_GTEST OFF CACHE BOOL "" FORCE)
set(YAML_ENABLE_PIC ON CACHE BOOL "" FORCE)
add_subdirectory(third_party/yaml-cpp EXCLUDE_FROM_ALL)


add_subdirectory(modules/liblandstalker EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_compile_definitions(ASSETS_PATH="${ASSETS_PATH}" PROJECT_NAME="${CMAKE_PROJECT_NAME}")

add_executable(${CMAKE_PROJECT_NAME} WIN32)
add_subdirectory("src")
TARGET_COMPILE_OPTIONS(${CMAKE_PROJECT_NAME} PRIVATE ${C_FLAGS})

# Copy assets from source to build
if(EMSCRIPTEN)
    add_compile_options(-sUSE_LIBPNG)
    set(CMAKE_EXE_LINKER_FLAGS "-sASSERTIONS -sUSE_LIBPNG --embed-file ../${ASSETS_PATH}")
else()
    target_link_libraries(dds520 PRIVATE png_static)
    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/${ASSETS_PATH}
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${ASSETS_PATH})
endif()

# Link to the actual SDL3 library.
target_link_libraries(dds520 PRIVATE SDL3::SDL3 yaml-cpp liblandstalker)
