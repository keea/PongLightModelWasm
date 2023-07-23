#include <PongLightModel.h>
#include <Shader.h>

PongLightModel::PongLightModel(GLFWwindow *window) {
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

    myGui = new DebugGUI(window);
    this->window = window;

    lightingShader =
        new Shader(2, "Shader/basic_lighting.vs", "Shader/basic_lighting.fs");
};

void PongLightModel::Render() {
    myGui->StartFrame();
    myGui->CreateWindow();

    float *lightColor = myGui->GetLightColor();
    float *objectColor = myGui->GetObjectColor();

    // 유니폼 / 그릴 오브젝트를 설정할 때 셰이더를 활성화 해야합니다.
    lightingShader->use();
    lightingShader->setVec3("objectColor", objectColor[0], objectColor[1],
                            objectColor[2]);
    lightingShader->setVec3("lightColor", lightColor[0], lightColor[1],
                            lightColor[2]);
    lightingShader->setVec3("lightPos", lightPos);
    lightingShader->setVec3("viewPos", camera.Position);

    lightingShader->setFloat("ambientStrength", myGui->GetAmbientStrength());
    lightingShader->setFloat("specularStrength", myGui->GetSpecularStrength());
#ifdef __EMSCRIPTEN__
    lightingShader->setFloat("shininess", myGui->GetShininess());
#else
    lightingShader->setInt("shininess", myGui->GetShininess());
#endif
    int width, height;
    glfwGetWindowSize(this->window, &width, &height);

    // 보기/투영 변환
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.Zoom),
                                  ((float)width / (float)height), 0.1f, 100.0f);
    lightingShader->setMat4("projection", projection);
    glm::mat4 view = camera.GetViewMatrix();
    lightingShader->setMat4("view", view);

    // 월드 변환
    glm::mat4 model = glm::mat4(1.0f);
    lightingShader->setMat4("model", model);

    // 구를 렌더링
    sphereMesh.Draw();

    myGui->Rendering();
}

void PongLightModel::Release() {
    sphereMesh.Release();

    delete lightingShader;
    delete myGui;
}

Camera &PongLightModel::GetCamera() { return camera; }