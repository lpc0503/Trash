cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)

project(project02)

add_executable(${PROJECT_NAME}
        main.cpp
        Renderer.cpp
        Renderer.h
        Camera.cpp
        Camera.h
        Log.cpp
        Log.h
        RendererAPI.cpp
        RendererAPI.h
        Core.h
        Utils.cpp
        Utils.h
        Mesh.cpp
        Mesh.h
        Model.cpp
        Model.h
        Vertex.h
        debugbreak.h)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME}
    glad::glad
    imgui
    glfw
    OpenGL::GL
    fmt::fmt
    glm::glm
    tinyobjloader
    common::common
)

if(APPLE)
    target_link_libraries(${PROJECT_NAME}
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
            )
endif(APPLE)


