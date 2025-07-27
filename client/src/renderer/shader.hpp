#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
 public:
  unsigned int programId;
  Shader(const char* vertex_src, const char* fragment_src);
  void use() const;
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setMat4(const std::string& name, glm::mat4 value) const;

  static constexpr const char* cube_vertex_src = R"(
 #version 330 core
 layout (location = 0) in vec3 aPos;
 out vec3 col;
 uniform mat4 model;
 uniform mat4 view;
 uniform mat4 projection;
 void main()
 {
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   col = aPos * 0.5 + 0.5;
 }
 )";

  static constexpr const char* cube_fragment_src = R"(
 #version 330 core
 out vec4 FragColor;
 in vec3 col;
 void main()
 {
   FragColor = vec4(col.xyz, 1.0f);
 }
 )";



 static constexpr const char* plane_vertex_src = R"(
 #version 330 core
 layout (location = 0) in vec3 aPos;
 out vec3 col;
 uniform mat4 model;
 uniform mat4 view;
 uniform mat4 projection;
 void main()
 {
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   col = aPos * 0.5 + 0.5;
 }
 )";

  static constexpr const char* plane_fragment_src = R"(
 #version 330 core
 out vec4 FragColor;
 in vec3 col;
 void main()
 {
   FragColor = vec4(col.xyz, 1.0f);
 }
 )";
};