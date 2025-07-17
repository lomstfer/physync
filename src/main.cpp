#include <glad/glad.h>
#include "renderer/renderer.hpp"
#include <iostream>

GLFWwindow *init_glfw_window()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "physync", monitor, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
}

int main()
{
    WorldState world_state = WorldState();
    world_state.cubes.push_back(Cube{0, 0, 0});
    world_state.cubes.push_back(Cube{2, 0, 0});
    world_state.cubes.push_back(Cube{3, 0, 0});
    world_state.cubes.push_back(Cube{4, 0, 3});
    world_state.cubes.push_back(Cube{4, 1, 3});
    world_state.cubes.push_back(Cube{4, 3, 3});

    Renderer renderer = Renderer(init_glfw_window());

    while (renderer.consume_state(world_state))
    {
    }
}