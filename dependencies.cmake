include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

function(UpdateSubmodules)
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
endfunction()

function(InstallZlib)
    message("Fetching ZLIB sources...")
    FetchContent_Declare(
        zlib
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG "51b7f2abdade71cd9bb0e7a373ef2610ec6f9daf" # "v1.3.1"
        EXCLUDE_FROM_ALL
    )
    set(ZLIB_BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_STATIC ON CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_MINIZIP OFF CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    message("Configuring ZLIB...")
    FetchContent_MakeAvailable(zlib)
    add_library(ZLIB::ZLIB ALIAS zlibstatic)
    set(ZLIB_ROOT "${zlib_SOURCE_DIR}" CACHE INTERNAL "Root for the zlib library, used by libpng")
    set(ZLIB_LIBRARY zlibstatic CACHE INTERNAL "" FORCE)
    set(ZLIB_INCLUDE_DIR "${zlib_SOURCE_DIR};${zlib_BINARY_DIR}" CACHE INTERNAL "" FORCE)
endfunction()

function(InstallLibpng)
    message("Fetching LIBPNG sources...")
    FetchContent_Declare(
        png
        GIT_REPOSITORY https://github.com/pnggroup/libpng.git
        GIT_TAG "7c67f70da1db5f1d7a75cf518d8e5bd704ba89ce" # "libpng18"
        EXCLUDE_FROM_ALL
    )
    set(PNG_SHARED OFF CACHE BOOL "" FORCE)
    set(PNG_STATIC ON CACHE BOOL "" FORCE)
    set(PNG_TOOLS OFF CACHE BOOL "" FORCE)
    set(PNG_BUILD_ZLIB OFF CACHE BOOL "" FORCE)
    set(PNG_TESTS OFF CACHE BOOL "" FORCE)
    set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)
    message("Configuring LIBPNG...")
    FetchContent_MakeAvailable(png)
endfunction()

function(InstallYamlcpp)
    message("Fetching YAML-CPP sources...")
    FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG "0579ae3d976091d7d664aa9d2527e0d0cff25763" # "yaml-cpp-0.7.0"
        EXCLUDE_FROM_ALL
    )
    set(YAML_CPP_BUILD_CONTRIB ON CACHE BOOL "" FORCE)
    set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
    set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(YAML_CPP_INSTALL OFF CACHE BOOL "" FORCE)
    set(YAML_CPP_FORMAT_SOURCE OFF CACHE BOOL "" FORCE)
    set(YAML_CPP_DISABLE_UNINSTALL ON CACHE BOOL "" FORCE)
    set(YAML_USE_SYSTEM_GTEST OFF CACHE BOOL "" FORCE)
    set(YAML_ENABLE_PIC ON CACHE BOOL "" FORCE)
    message("Configuring YAML-CPP...")
    FetchContent_MakeAvailable(yaml-cpp)
endfunction()

function(InstallSdl3)
    message("Fetching SDL3 sources...")
    FetchContent_Declare(
        sdl3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG b31c4b70b3031dc4eb873612d65595a31b439a30 # "preview-3.3.2"
        EXCLUDE_FROM_ALL
    )
    message("Configuring SDL3...")
    FetchContent_MakeAvailable(sdl3)
endfunction()

function(InstallDependencies)
    # Emscripten has it's own version of libpng
    if(NOT(EMSCRIPTEN))
        InstallZlib()
        InstallLibpng()
    endif()
    InstallYamlcpp()
    InstallSdl3()
endfunction()
