cmake_minimum_required(VERSION 3.22)
project(WendysBasement CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory setup
include_directories(include)

# Automatically collect all C++ source files in src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Find SFML 3 components
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)

# Windows-specific handling
if(WIN32)
    # Prevents opening a console window on Windows release builds
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

if(WIN32)
    add_custom_command(
        TARGET WendysBasement POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:WendysBasement>
            $<TARGET_FILE_DIR:WendysBasement>
        COMMAND_EXPAND_LISTS
    )
endif()

if(MSVC)
    set_target_properties(WendysBasement PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()