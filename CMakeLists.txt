
#### Config ####

cmake_minimum_required(VERSION 3.22)
project(WendysBasement CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#### Source Code ####


# Include directory setup
include_directories(include)

# Automatically collect all C++ source files in src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Default SFML path for Windows installations in C:\
if(WIN32 AND NOT CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "C:/SFML")
endif()


#### Mode ####


# Default build type to Release not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


# Enable DEBUG_LOG_ENABLED in Debug builds, disable (omit) in Release/non-Debug
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG_LOG_ENABLED>
)

# Compiler flags per build configuration for MSVC and GCC/Clang
target_compile_options(${PROJECT_NAME} PRIVATE
    # MSVC - Debug
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Zi /Od /W4 /RTC1>
    # MSVC - Release (and other non-Debug configs)
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<NOT:$<CONFIG:Debug>>>:/O2 /W4 /DNDEBUG>

    # GCC / Clang - Debug
    $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-g -O0 -Wall -Wextra -fsanitize=address,undefined>
    # GCC / Clang - Release (and other non-Debug configs)
    $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<NOT:$<CONFIG:Debug>>>:-O3 -Wall -Wextra -DNDEBUG>
)


#### Linking ####


# Linker flags per build configuration
target_link_options(${PROJECT_NAME} PRIVATE
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/DEBUG>
    $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-fsanitize=address,undefined>
)

# Find SFML 3 components
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)

# Windows-specific post-build DLL copying
if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()


### Extra ###

# Copy assets directory to the build output directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)