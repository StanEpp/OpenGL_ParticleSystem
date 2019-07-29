cmake_minimum_required(VERSION 3.5)

project(OpenGL_Particlesystem)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(deps)

include_directories(${DEPENDENCY_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_FILES
    src/main.cpp
    src/gl3w.c
    src/ShaderManager.cpp
    src/tga.c
    src/ParticleBuffer.cpp
    src/ParticleSystem.cpp
    src/ParticleTexture.cpp
    src/ShaderManager.cpp
)

set(HEADER_FILES
    src/Attractor.hpp
    src/GLFWWindow.hpp
    src/GLFWInput.hpp
    src/Camera.hpp
    src/ParticleBuffer.hpp
    src/ShaderManager.hpp
    src/tga.h
    src/ParticleSystem.hpp
    src/GLFWTimer.hpp
)

set(SHADER_FILES
    shader/cs.glsl
    shader/fs.glsl
    shader/gs.glsl
    shader/vs.glsl
)

foreach(SHADER ${SHADER_FILES})
configure_file(${SHADER} ${SHADER} COPYONLY)
endforeach()

configure_file(settings.txt settings.txt COPYONLY)
configure_file(texture/Particle.tga texture/Particle.tga COPYONLY)
add_executable(OpenGL_Particlesystem ${SOURCE_FILES} ${HEADER_FILES})
add_dependencies(OpenGL_Particlesystem glfw_ext glm_ext)
target_link_libraries(OpenGL_Particlesystem PUBLIC glfw dl)
target_compile_options(OpenGL_Particlesystem PUBLIC -Wall -g)