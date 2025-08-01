#include "shader.hpp"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <iostream>



Shader::Shader(const char* vertex_src, const char* fragment_src) {
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertex_src, NULL);
  glCompileShader(vertexShader);

  {
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                << infoLog << std::endl;
    }
  }

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragment_src, NULL);
  glCompileShader(fragmentShader);

  {
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                << infoLog << std::endl;
    }
  }

  programId = glCreateProgram();
  glAttachShader(programId, vertexShader);
  glAttachShader(programId, fragmentShader);
  glLinkProgram(programId);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  {
    int success;
    char infoLog[512];
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(programId, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }
  }
}

void Shader::use() const {
  glUseProgram(programId);
}

void Shader::setBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(programId, name.data()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(programId, name.data()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(programId, name.data()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const {
  unsigned int loc = glGetUniformLocation(programId, name.data());
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}