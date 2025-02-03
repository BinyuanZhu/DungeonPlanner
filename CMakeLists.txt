cmake_minimum_required(VERSION 3.12)
project(DungeonPlanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# local include directory for GLAD
set(CMAKE_REQUIRED_INCLUDES "/usr/local/include")
include_directories("/usr/local/include")

# Add the source files
set(SOURCES src/main.cpp)

# Find and link against GLFW and GLAD
find_package(glfw3 REQUIRED)

add_subdirectory(dependencies)
add_executable(DungeonPlanner ${SOURCES})

target_link_libraries(DungeonPlanner
    PRIVATE
        glfw
        glad
        "-framework OpenGL"  # Add this line to link against the OpenGL library
)

target_include_directories(DungeonPlanner
    PRIVATE
        "main/"
)

# Set the include directory for GLFW, assuming brew
target_include_directories(DungeonPlanner PRIVATE "/opt/homebrew/include")
