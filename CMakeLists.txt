cmake_minimum_required(VERSION 3.16)
project(dds520)
set(CMAKE_CXX_STANDARD 17)
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

if(EMSCRIPTEN)
    set(ASSETS_PATH "/assets")
    add_definitions("-fexceptions -g")
else()
    set(ASSETS_PATH "./assets")
endif()

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_compile_definitions(ASSETS_PATH="${ASSETS_PATH}" PROJECT_NAME="${CMAKE_PROJECT_NAME}")
add_executable(${CMAKE_PROJECT_NAME} WIN32 hello.cpp)
TARGET_COMPILE_OPTIONS(${CMAKE_PROJECT_NAME} PRIVATE ${C_FLAGS})

# Copy assets from source to build
if(EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "-sASSERTIONS --embed-file ../${ASSETS_PATH}")
else()
    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/${ASSETS_PATH}
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${ASSETS_PATH})
endif()

# Link to the actual SDL3 library.
target_link_libraries(dds520 PRIVATE SDL3::SDL3)
