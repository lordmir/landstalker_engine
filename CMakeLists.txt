cmake_minimum_required(VERSION 3.16)
project(dds520)
set(CMAKE_CXX_STANDARD 20)

include(dependencies.cmake)

set(LANDSTALKER_BUILD_SHARED OFF CACHE BOOL "" FORCE)

UpdateSubmodules()
InstallDependencies()

add_subdirectory(modules/liblandstalker EXCLUDE_FROM_ALL)

add_executable(${CMAKE_PROJECT_NAME} WIN32)
# Copy assets from source to build
if(EMSCRIPTEN)
    set(ASSETS_PATH "/assets")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    add_compile_options(-sUSE_LIBPNG -fexceptions)
    set(CMAKE_EXE_LINKER_FLAGS "-sASSERTIONS -sUSE_LIBPNG --embed-file ../${ASSETS_PATH}")
else()
    set(ASSETS_PATH "./assets")
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE png_static zlibstatic)
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
