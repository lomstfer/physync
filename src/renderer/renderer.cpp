// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include "shader.hpp"
#include <iostream>
#include "renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebufferSizeCallback(GLFWwindow *, int width, int height)
{
    glViewport(0, 0, width, height);
}

Renderer::Renderer(GLFWwindow *window)
{
    this->window = window;
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    shader.use();

    camera_pos = glm::vec3(0, 5.0f, 10.0f);
    camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

bool Renderer::consume_state(const WorldState &state)
{
    if (glfwWindowShouldClose(window))
        return false;

    glm::vec3 direction = glm::vec3(0, -0.5f, -1);
    camera_front = glm::normalize(direction);

    glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cube_renderer.render(state.cubes, shader);

    glfwSwapBuffers(window);
    glfwPollEvents();

    return true;
}

Renderer::~Renderer()
{
    glfwTerminate();
}