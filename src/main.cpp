#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include <PongLightModel.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mainLoop();

// setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

#ifdef __EMSCRIPTEN__
bool fullscreen = false;

void emscGetContainerSize(double* width, double* height) {
    emscripten_get_element_css_size(".emscripten_border", width, height);
}

EM_BOOL emscWindowSizeChanged(int eventType, const EmscriptenUiEvent* uiEvent, void* userData) {
    double width, height;
    emscGetContainerSize(&width, &height);
    printf("Div size changed to: %lf x %lf\n", width, height);

    GLFWwindow* window = (GLFWwindow*)userData;
    //if (glfwGetWindowMonitor(window) != nullptr) // Not working for fullscreen check.
    if(!fullscreen)
    glfwSetWindowSize(window, width-2, height-2);
    return true;
}

EM_BOOL emscFullscreenChanged(int eventType, const EmscriptenFullscreenChangeEvent* fullscreenChangeEvent, void* userData)
{
    fullscreen = fullscreenChangeEvent->isFullscreen;
    if (fullscreen)printf("changed to FULLSCREEN\n"); else printf("changed to windowed\n");
    return true;
}
#endif

PongLightModel *world;

//카메라
// Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 400, lastY = 300;
bool firstMouse = true;

//타이밍
float deltaTime = 0.0f; //현재 프레임과 마지막 프레임 사이의 시간
float lastFrame = 0.0f;

GLFWwindow *window;

// glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main() {

#ifdef __EMSCRIPTEN__
    // // setting up EmscriptenWebGLContextAttributes
	// EmscriptenWebGLContextAttributes attr;
	// emscripten_webgl_init_context_attributes(&attr);
	// attr.alpha = 0;

	// // target the canvas selector
	// EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attr);
	// emscripten_webgl_make_context_current(ctx);
#endif

    glfwInit(); // glfw 초기화

    /* 3.3버전을 사용한다는 것을 알림 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE); // glfw에게 core-profile을 사용한다고 알림.

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    int windowWidth = SCR_WIDTH, windowHeight = SCR_HEIGHT;

#ifdef __EMSCRIPTEN__
    double dw, dh;
    emscGetContainerSize(&dw, &dh);
    windowWidth = dw;
    windowHeight = dh;
#endif

    /* 윈도우 객체 생성 */
    window = glfwCreateWindow(windowWidth, windowHeight, "Phong_Lighting_Model",
                              NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // window의 컨텍스트를 현재 스레드의 주
                                    // 컨텍스트로 지정하겠다고 알림.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

#ifdef __EMSCRIPTEN__
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, window, false, emscWindowSizeChanged);
    emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, false, emscFullscreenChanged);
#endif


    /* GLAD 초기화 */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    world = new PongLightModel(window);

    /* render loop */
#ifdef __EMSCRIPTEN__
    std::cout << "run main Loop!" << std::endl;
    emscripten_set_main_loop(mainLoop, 0, true);
#else
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        mainLoop();
    }
#endif
	world->Release();

    glfwTerminate(); //할당된 모든 자원들을 정리/삭제.

    return 0;
}

void mainLoop() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    /* input */
    processInput(window);

    /* render */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    world->Render();

    glfwSwapBuffers(window); //컬러 버퍼를 교체
    glfwPollEvents(); //이벤트가 발생했는지 확인하고 윈도우 상태
                      //업데이트하며 정해진 함수 호출.
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) ==
        GLFW_PRESS) // ESC 키를 눌렀는지 확인
    {
        std::cout << "ESC 눌려짐" << std::endl;

        glfwSetWindowShouldClose(window,
                                 true); // WindowShouldClose 속성을 true 세팅
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        world->GetCamera().ProcessKeyboard(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        world->GetCamera().ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        world->GetCamera().ProcessKeyboard(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        world->GetCamera().ProcessKeyboard(RIGHT, deltaTime);
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        firstMouse = true;

        return;
    }

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    world->GetCamera().ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    world->GetCamera().ProcessMouseScroll(yoffset);
}