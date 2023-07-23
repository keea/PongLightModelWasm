#pragma once
#include <Camera.h>
#include <DrawSphere.h>
#include <GUI/DebugGUI.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLFWwindow;
class Shader;

class PongLightModel {
  public:
    PongLightModel(GLFWwindow *window);
    void Render();
    void Release();

    Camera & GetCamera();

  private:
    // 카메라
    Camera camera;

    // 라이트
    glm::vec3 lightPos;

    // 쉐이더
    Shader *lightingShader;

    // 구 오브젝트
    DrawSphere sphereMesh;

    // Gui
    DebugGUI *myGui;

    GLFWwindow *window;
};