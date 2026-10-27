cmake_minimum_required(VERSION 4.2)
project(Double_Pendulum_Balancing LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_Declare(ImGui-SFML
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG v3.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_Declare(ImGui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        # ImGui-SFML v3.0 uses the texture API from ImGui 1.91.
        GIT_TAG v1.91.9b
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)

# This application only uses SFML's graphics, window and system modules.
set(SFML_BUILD_NETWORK OFF CACHE BOOL "Build SFML networking" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "Build SFML audio" FORCE)
FetchContent_MakeAvailable(SFML ImGui)

set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
set(IMGUI_DIR "${imgui_SOURCE_DIR}")

FetchContent_MakeAvailable(ImGui-SFML)

add_executable(Double_Pendulum_Balancing main.cpp
        src/core/Log.cpp
        src/core/Log.h
        src/core/Engine.cpp
        src/core/Engine.h
        src/core/GraphicsEngine.cpp
        src/core/GraphicsEngine.h
        src/core/Common.h
        src/core/Colour.h
        src/physics/Beam.cpp
        src/physics/Beam.h
        src/physics/Constraint.cpp
        src/physics/Constraint.h
        src/physics/PointMass.cpp
        src/physics/PointMass.h
        src/physics/PhysicsSolver.cpp
        src/physics/PhysicsSolver.h
        src/physics/PhysicsCore.h
        src/physics/PhysicsCore.cpp
        src/core/checks.cpp
        src/core/checks.h)

add_custom_command(TARGET Double_Pendulum_Balancing POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Double_Pendulum_Balancing>/assets"
        COMMENT "Copying assets"
        VERBATIM
)

target_include_directories(Double_Pendulum_Balancing PRIVATE "${PROJECT_SOURCE_DIR}/src")

target_link_libraries(Double_Pendulum_Balancing
        PRIVATE
        SFML::Graphics
        ImGui-SFML::ImGui-SFML
)
target_compile_features(Double_Pendulum_Balancing PRIVATE cxx_std_20)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Double_Pendulum_Balancing PRIVATE DEBUG_MODE)
endif ()

set(APP_VERSION "" CACHE STRING "Application version (empty uses Common.h default)")
if(NOT APP_VERSION STREQUAL "")
    string(REPLACE "\\" "\\\\" APP_VERSION_ESCAPED "${APP_VERSION}")
    string(REPLACE "\"" "\\\"" APP_VERSION_ESCAPED "${APP_VERSION_ESCAPED}")
    target_compile_definitions(Double_Pendulum_Balancing PRIVATE "APP_VERSION=\"${APP_VERSION_ESCAPED}\"")
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Double_Pendulum_Balancing)
