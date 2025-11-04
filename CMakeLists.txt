cmake_minimum_required(VERSION 3.16)
project(dds520)
set(CMAKE_CXX_STANDARD 20)

option(LANDSTALKER_BUILD_SHARED "Build liblandstalker as a shared library" OFF)
option(INSTALL_DEPS "Automatically download and build dependencies" ON)

include(dependencies.cmake)

UpdateSubmodules()

# Download dependencies if this is a standalone build
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    InstallDependencies()
endif()

if(WIN32)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

if(${LANDSTALKER_BUILD_SHARED})
    if(EMSCRIPTEN)
        message(FATAL_ERROR "LANDSTALKER_BUILD_SHARED can not be used when building for Emscripten")
    endif()

    set(${CMAKE_PROJECT_NAME}_RUNTIME_DEPENDENCIES png_shared yaml-cpp zlib pugixml-shared)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ZLIB::ZLIB png_shared yaml-cpp::yaml-cpp pugixml::shared)
else()
    add_library(${CMAKE_PROJECT_NAME} STATIC)


    if(EMSCRIPTEN)
        add_compile_options(-sUSE_LIBPNG)
        add_definitions("-sUSE_LIBPNG -fexceptions")
        set(CMAKE_EXE_LINKER_FLAGS "-sASSERTIONS -sUSE_LIBPNG")
        target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE yaml-cpp::yaml-cpp pugixml::static)
    else()
        target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ZLIB::ZLIB png_static yaml-cpp::yaml-cpp pugixml::static)
    endif()
endif()

if (MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
endif()

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(modules/liblandstalker EXCLUDE_FROM_ALL)

add_executable(${CMAKE_PROJECT_NAME} WIN32)

# Copy assets from source to build
if(EMSCRIPTEN)
    set(ASSETS_PATH "/assets")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
else()
    set(ASSETS_PATH "./assets")
    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/${ASSETS_PATH}
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${ASSETS_PATH})
endif()
add_compile_definitions(ASSETS_PATH="${ASSETS_PATH}" PROJECT_NAME="${CMAKE_PROJECT_NAME}")

add_subdirectory("src")

if (WIN32)
    add_custom_command (
        TARGET "${CMAKE_PROJECT_NAME}" POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>"
                "$<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}>" USES_TERMINAL COMMAND_EXPAND_LISTS
    )
endif ()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3 yaml-cpp::yaml-cpp pugixml::static landstalker)

install(
    TARGETS ${CMAKE_PROJECT_NAME}
    EXPORT ${CMAKE_PROJECT_NAME}Targets
    CONFIGURATIONS Release Debug
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(
    DIRECTORY "${CMAKE_SOURCE_DIR}/${ASSETS_PATH}" DESTINATION bin
)

install(FILES $<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}> TYPE BIN)
