#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <al.h>
#include <alc.h>
#include <vector>
//#include <sndfile.h>
//#include <irrKlang/irrKlang.h>
//#include "stb_image.h"

#pragma execution_character_set("utf-8")

#include "shader.h"
#include "camera.h"
//#include "mineAudio.h"
#include "model_animation.h"


#define CAS 0.001f
#define NUM_LAMPS 8

class SimpleBoneController {
private:
    std::map<std::string, glm::mat4> boneTransforms;
    std::map<std::string, int> boneIDMap;
    int boneCount;

public:
    SimpleBoneController() : boneCount(0) {
        // 初始化默认骨骼
    }

    // 复制构造函数
    SimpleBoneController(const SimpleBoneController& other) {
        this->boneTransforms = other.boneTransforms;
        this->boneIDMap = other.boneIDMap;
        this->boneCount = other.boneCount;
    }
    void SetupFromModel(Model& model) {
        auto& boneInfoMap = model.GetBoneInfoMap();
        boneCount = model.GetBoneCount();

        std::cout << "模型包含 " << boneCount << " 个骨骼" << std::endl;

        // 初始化所有骨骼为单位矩阵
        for (auto& pair : boneInfoMap) {
            std::cout << "骨骼名称: '" << pair.first << "'" << std::endl;
            std::cout << "  骨骼ID: " << pair.second.id << std::endl;
            std::cout << "  名称长度: " << pair.first.length() << std::endl;

            // 检查名称中是否有特殊字符
            for (char c : pair.first) {
                std::cout << "  字符: '" << c << "' ASCII: " << (int)c << std::endl;
            }

            boneTransforms[pair.first] = glm::mat4(1.0f);
            boneIDMap[pair.first] = pair.second.id;
        }
    }

    void RotateBone(const std::string& boneName, float angleX, float angleY, glm::vec3 front = glm::vec3(0.0f, 1.0f, 0.0f)) {
        if (boneTransforms.find(boneName) != boneTransforms.end()) {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(1.0f, 0.0f, 0.0f));
            rotation = glm::rotate(rotation, glm::radians(angleX), glm::vec3(0.0f, 1.0f, 0.0f));
            std::cout << "旋转骨骼 " << boneName << " - X: " << angleX << " Y: " << angleY << std::endl;
            boneTransforms[boneName] = rotation;
        }
        else {
            std::cout << "未找到骨骼: " << boneName << std::endl;
            // 打印所有可用的骨骼名称
            std::cout << "可用的骨骼: ";
            for (auto& pair : boneTransforms) {
                std::cout << pair.first << " ";
            }
            std::cout << std::endl;
        }
    }

    std::vector<glm::mat4> GetBoneMatrices() {
        std::vector<glm::mat4> matrices(100, glm::mat4(1.0f));

        for (auto& pair : boneTransforms) {
            std::string boneName = pair.first;
            if (boneIDMap.find(boneName) != boneIDMap.end()) {
                int boneID = boneIDMap[boneName];
                if (boneID >= 0 && boneID < 100) {
                    matrices[boneID] = pair.second;
                }
            }
        }

        return matrices;
    }
};
//
SimpleBoneController boneController;

// 基础设置（窗口尺寸等
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool bloom = true;
bool bloomKeyPressed = false;
float exposure = 1.0f;

// 鼠标控制
bool resetMouseNextFrame = false;

// camera
Camera camera(glm::vec3(0.0f, 20.0f, 40.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

////光源位置所有光源信息
//std::vector<glm::vec3> lightPositions;
//std::vector<glm::vec3> lightColors;

// 动画控制
bool isAnimating = false;
bool useBones = true;
float currentRotationX = 0.0f;
float currentRotationY = 0.0f;

// 在全局变量部分添加
bool lampControlMode = false;  // 灯具控制模式
bool lampKeyPressed = false; // 切换灯具控制模式的按键状态
int currentLampIndex = 0;  // 当前控制的灯具索引 (0-7)

// 每个灯具独立的旋转角度
std::vector<float> lampRotationX;  // 每个灯具的X旋转角度
std::vector<float> lampRotationY;  // 每个灯具的Y旋转角度
// 灯具实例数组
std::vector<SimpleBoneController> lampBoneControllers;
std::vector<glm::vec3> lampPositions;  // 每个灯具的位置
glm::vec3 dir1 = glm::vec3(0.0f, -1.0f, 1.0f);
glm::vec3 dir2 = glm::vec3(0.0f, -1.0f, -1.0f);

// 在全局变量部分添加
struct LampLight {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float strength;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
    float softEdge;
};

// 所有光源信息
std::vector<LampLight> lampLights;

// 以下四行实现相机在灯座后上方
bool cameraFollowLight = false;
glm::vec3 cameraOffset = glm::vec3(0.0f, 5.0f, -10.0f); // 相机相对于灯具的偏移
// 保存原始相机状态，用于退出灯具控制模式时恢复
Camera originalCamera(glm::vec3(0.0f, 20.0f, 40.0f));
bool originalCameraSaved = false;

// 骨骼动画相关
Model* stageLightModel = nullptr;



//视口调整函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    const float cameraSpeed = 10.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !lampKeyPressed)
    {
        lampControlMode = !lampControlMode;
        isAnimating = lampControlMode; // 同步动画状态
        cout << (lampControlMode ? "进入灯具控制模式" : "退出灯具控制模式") << endl;
        lampKeyPressed = true;// 标记下一帧需要重置鼠标

        // 重置鼠标位置
        if (lampControlMode) {
            glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);
            resetMouseNextFrame = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
    {
        lampKeyPressed = false;
    }
    // 数字键选择灯具
    for (int i = 0; i < NUM_LAMPS; i++) {
        if (glfwGetKey(window, GLFW_KEY_1 + i) == GLFW_PRESS) {
            currentLampIndex = i;
            cout << "切换到灯具: " << (currentLampIndex + 1)
                << " 角度(X:" << lampRotationX[currentLampIndex]
                << " Y:" << lampRotationY[currentLampIndex] << ")" << endl;
        }
    }
    if (!lampControlMode)
    {
        glm::vec3 originalPos = camera.Position;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, cameraSpeed);

        float groundLevel = -10.0f;  // 地面在Y=-10
        float minCameraHeight = groundLevel + 1.0f;  // 相机最低在Y=-9（离地面1单位）

        if (camera.Position.y < minCameraHeight) {
            camera.Position.y = minCameraHeight;
        }

        // 可选：添加下降限制（比如按Shift下降）
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            camera.Position.y -= cameraSpeed * 2.0f;  // 快速下降
            // 同样应用限制
            if (camera.Position.y < minCameraHeight) {
                camera.Position.y = minCameraHeight;
            }
        }
        // 可选：上升（比如按空格）
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.Position.y += cameraSpeed * 2.0f;  // 快速上升
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !bloomKeyPressed)
        {
            bloom = !bloom;
            bloomKeyPressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
        {
            bloomKeyPressed = false;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            if (exposure > 0.0f)
                exposure -= 0.001f;
            else
                exposure = 0.0f;
        }
        else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            exposure += 0.001f;
        }
    }

}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    static bool firstMouse = true;
    static float lastX = SCR_WIDTH / 2.0f;
    static float lastY = SCR_HEIGHT / 2.0f;

    // 如果需要重置鼠标，忽略这一帧的移动
    if (resetMouseNextFrame) {
        lastX = SCR_WIDTH / 2.0f;
        lastY = SCR_HEIGHT / 2.0f;
        resetMouseNextFrame = false;
        return;
    }

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;  // 第一次调用时不处理
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    // 只在有实际移动时才处理
    if (fabs(xoffset) < 0.1f && fabs(yoffset) < 0.1f) {
        return;  // 忽略微小移动
    }

    // 根据控制模式选择不同的行为
    if (lampControlMode)
    {
        // 灯具控制模式：旋转灯具骨骼
        float sensitivity = 0.5f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        // 更新当前灯具的旋转角度（累积）
        lampRotationX[currentLampIndex] -= xoffset;
        lampRotationY[currentLampIndex] += yoffset;

        // 限制角度范围
        lampRotationY[currentLampIndex] = glm::clamp(lampRotationY[currentLampIndex], -80.0f, 80.0f);
        lampRotationX[currentLampIndex] = glm::clamp(lampRotationX[currentLampIndex], -80.0f, 80.0f);

        // 更新当前选中灯具的骨骼
        lampBoneControllers[currentLampIndex].RotateBone("LampBone",
            lampRotationX[currentLampIndex],
            lampRotationY[currentLampIndex]);

        // 计算新的聚光灯方向
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(lampRotationY[currentLampIndex]), glm::vec3(1.0f, 0.0f, 0.0f));
        rotation = glm::rotate(rotation, glm::radians(lampRotationX[currentLampIndex]), glm::vec3(0.0f, 1.0f, 0.0f)); // 水平旋转
        rotation = glm::rotate(rotation, glm::radians(lampRotationY[currentLampIndex]), glm::vec3(1.0f, 0.0f, 0.0f)); // 垂直旋转

        // 应用旋转到标准前向向量
        glm::vec3 newDirection = glm::normalize(glm::vec3(rotation * glm::vec4(dir2, 0.0f)));

        if (currentLampIndex < NUM_LAMPS / 2) {
            newDirection.z = -newDirection.z; // 反转Z方向
            newDirection.x = -newDirection.x; // 反转Z方向
        }

        // 更新灯具的光照方向
        lampLights[currentLampIndex].direction = newDirection;

        std::cout << "旋转灯具 " << (currentLampIndex + 1)
            << " - X: " << lampRotationX[currentLampIndex]
            << " Y: " << lampRotationY[currentLampIndex] << std::endl;
    }
    else
    {
        // 相机控制模式：旋转相机
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void light_para(const Shader& modelShader)
{
    for (unsigned int i = 0; i < lampLights.size(); i++)
    {
        modelShader.setVec3("light[" + std::to_string(i) + "].position", lampLights[i].position);
        modelShader.setVec3("light[" + std::to_string(i) + "].direction", lampLights[i].direction);
        modelShader.setVec3("light[" + std::to_string(i) + "].color", lampLights[i].color);
        modelShader.setFloat("light[" + std::to_string(i) + "].strength", lampLights[i].strength);
        modelShader.setFloat("light[" + std::to_string(i) + "].constant", lampLights[i].constant);
        modelShader.setFloat("light[" + std::to_string(i) + "].linear", lampLights[i].linear);
        modelShader.setFloat("light[" + std::to_string(i) + "].quadratic", lampLights[i].quadratic);
        modelShader.setFloat("light[" + std::to_string(i) + "].cutOff", lampLights[i].cutOff);
        modelShader.setFloat("light[" + std::to_string(i) + "].outerCutOff", lampLights[i].outerCutOff);
        modelShader.setFloat("light[" + std::to_string(i) + "].softEdge", lampLights[i].softEdge);
    }
}

//加载纹理
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

//加载天空盒纹理
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Model milkbar("C:/Users/Hibb/OneDrive/Desktop/mimia/milkbar.fbx");
    //Model spotlight("C:/Users/Hibb/OneDrive/Desktop/mimia/shotlight/shotlight.fbx");
    //Model gan("C:/Users/Hibb/OneDrive/Desktop/mimia/gan.obj");
    Model milkbar("../assets/milkbar.fbx");
    Model spotlight("../assets/shotlight.fbx");
    Model gan("../assets/gan.obj");
    Model musicbox4("C:/Users/Hibb/OneDrive/Desktop/mimia/musicbox4.fbx");

    boneController.SetupFromModel(spotlight);
    // 初始化8个灯具
    lampBoneControllers.resize(NUM_LAMPS);
    lampPositions.resize(NUM_LAMPS);
    lampRotationX.resize(NUM_LAMPS, 0.0f);  // 初始化为0度
    lampRotationY.resize(NUM_LAMPS, 0.0f);  // 初始化为0度
    // 设置每个灯具的位置
    for (float i = 0; i < 4; i++)
        lampPositions[i] = glm::vec3(-18.0f + 11 * i, 20.0f, 30.0f);
    for (float i = 4; i < 8; i++)
        lampPositions[i] = glm::vec3(-18.0f + 11 * (i - 4), 20.0f, 55.5f);
    // 初始化每个灯具的骨骼控制器
    for (int i = 0; i < NUM_LAMPS; i++) {
        lampBoneControllers[i].SetupFromModel(spotlight);
        std::cout << "Initialized bone controller for lamp " << (i + 1) << std::endl;
    }

    // 替换原来的灯具信息归入部分
    lampLights.resize(NUM_LAMPS);

    // 初始化每个灯具的位置和光照属性
    for (int i = 0; i < NUM_LAMPS; i++) {
        lampLights[i].position = lampPositions[i];

        // 初始方向
// 初始方向 - 使用归一化的向量
        if (i < NUM_LAMPS / 2)
            lampLights[i].direction = glm::normalize(dir1);  // 归一化！
        else
            lampLights[i].direction = glm::normalize(dir2); // 归一化！
        // 设置不同的颜色（可以根据需要调整）
        if (i < 2) lampLights[i].color = glm::vec3(1.0f, 0.2f, 0.2f); // 红色
        else if (i < 4) lampLights[i].color = glm::vec3(0.2f, 1.0f, 0.2f); // 绿色
        else if (i < 6) lampLights[i].color = glm::vec3(0.2f, 0.2f, 1.0f); // 蓝色
        else lampLights[i].color = glm::vec3(1.0f, 1.0f, 1.0f); // 白色

        lampLights[i].strength = 3.0f; // 光强
        lampLights[i].constant = 1.0f;
        lampLights[i].linear = 0.007f;
        lampLights[i].quadratic = 0.00014f;
        lampLights[i].cutOff = glm::cos(glm::radians(8.0f));
        lampLights[i].outerCutOff = glm::cos(glm::radians(12.0f));
        lampLights[i].softEdge = 0.2f;
    }

    Shader modelShader("model.vert", "model.frag");
    Shader skyboxShader("skybox.vert", "skybox.frag");
    Shader quadShader("quad.vert", "quad.frag");

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    {
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    //天空盒纹理
    vector<std::string> faces
    {
        "../LearnOpenGL/resources/textures/skybox/right.jpg",
        "../LearnOpenGL/resources/textures/skybox/left.jpg",
        "../LearnOpenGL/resources/textures/skybox/top.jpg",
        "../LearnOpenGL/resources/textures/skybox/bottom.jpg",
        "../LearnOpenGL/resources/textures/skybox/front.jpg",
        "../LearnOpenGL/resources/textures/skybox/back.jpg",
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    //普通全屏贴图VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    {
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }
    //创建帧缓冲区
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // 颜色附件纹理
    unsigned int textureColorbuffer;
    {
        glGenTextures(1, &textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    }
    // 深度和模板附件
    unsigned int rbo;
    {
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    }
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //启动深度测试
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        //light model
        if (1)
        {
            modelShader.use();
            modelShader.setBool("useBones", useBones);

            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            //modelShader.setMat4("model", model);
            modelShader.setMat4("projection", projection);
            modelShader.setMat4("view", view);

            // 设置光照参数
            light_para(modelShader);

            modelShader.setVec3("viewPos", camera.Position);
            modelShader.setFloat("material.shininess", 32.0f);

            //// 获取骨骼矩阵并传递给着色器
            //auto boneTransforms = boneController.GetBoneMatrices();
            //for (int i = 0; i < boneTransforms.size(); ++i) {
            //    std::string name = "boneMatrices[" + std::to_string(i) + "]";
            //    modelShader.setMat4(name, boneTransforms[i]);
            //}

            // 绘制所有灯具
            for (int i = 0; i < NUM_LAMPS; i++) {
                // 设置每个灯具的模型矩阵（位置）
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, lampPositions[i]);
                model = glm::scale(model, glm::vec3(4.0f));  // 适当缩放
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                if (i >= NUM_LAMPS / 2)
                    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

                modelShader.setMat4("model", model);

                // 修复：使用正确的骨骼控制器
                if (isAnimating && i == currentLampIndex) {
                    lampBoneControllers[i].RotateBone("LampBone",
                        lampRotationX[currentLampIndex],
                        lampRotationY[currentLampIndex]);
                }

                // 设置当前灯具的骨骼矩阵
                auto boneTransforms = lampBoneControllers[i].GetBoneMatrices();
                for (int j = 0; j < boneTransforms.size(); ++j) {
                    std::string name = "boneMatrices[" + std::to_string(j) + "]";
                    modelShader.setMat4(name, boneTransforms[j]);
                }
                // 绘制模型
                spotlight.Draw(modelShader);
            }
        }
        //gan model
        if (1)
        {
            modelShader.use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(8.0f));
            model = glm::translate(model, glm::vec3(0.0f, 2.5f, 3.53f));

            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            modelShader.setMat4("model", model);
            modelShader.setMat4("projection", projection);
            modelShader.setMat4("view", view);

            // 设置光照参数
            light_para(modelShader);
            modelShader.setVec3("viewPos", camera.Position);
            modelShader.setFloat("material.shininess", 32.0f);

            // 绘制模型
            gan.Draw(modelShader);
        }
        //bar model
        if (1)
        {
            modelShader.use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.01f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            modelShader.setMat4("model", model);
            modelShader.setMat4("projection", projection);
            modelShader.setMat4("view", view);

            // 设置光照参数
            light_para(modelShader);
            modelShader.setVec3("viewPos", camera.Position);
            modelShader.setFloat("material.shininess", 32.0f);

            // 绘制模型
            milkbar.Draw(modelShader);
        }

        //天空盒
        if (1)
        {
            // draw skybox as last
            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
            skyboxShader.use();
            skyboxShader.setInt("skybox", 1);

            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
            skyboxShader.setMat4("view", view);
            skyboxShader.setMat4("projection", projection);
            // skybox cube
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS); // set depth function back to default
        }


        //// second pass
        //glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        //quadShader.use();
        //glBindVertexArray(quadVAO);
        //glDisable(GL_DEPTH_TEST);
        //glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}
