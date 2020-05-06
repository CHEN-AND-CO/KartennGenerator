cmake_minimum_required(VERSION 3.10)

project(KartennGenerator VERSION 0.0.2 DESCRIPTION "PostGis Township render" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Dependencies import
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
include(cmake/Dependencies.cmake)

# Includes dir
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Executables
add_executable(KartennGenerator ${SOURCES})

# Linking libs
target_link_libraries(KartennGenerator PRIVATE ${Dependencies_LINKER_LIBS})