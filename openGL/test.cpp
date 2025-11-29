//#define STB_IMAGE_IMPLEMENTATION
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <cmath>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <al.h>
//#include <alc.h>
//#include <vector>
//#include <sndfile.h>
////#include <irrKlang/irrKlang.h>
////#include "stb_image.h"
//
//#pragma execution_character_set("utf-8")
//
//#include "shader.h"
//#include "camera.h"
////#include "model.h"
////#include "mesh.h"
////#include "LightingSystem.h"
//#include "mineAudio.h"
//#include "model_animation.h"
//#include "animator.h"
//
//#define CAS 0.001f
//#define NUM_LAMPS 8
////using namespace irrklang;
////ISoundEngine* SoundEngine = createIrrKlangDevice();
//
//class SimpleBoneController {
//private:
//    std::map<std::string, glm::mat4> boneTransforms;
//    std::map<std::string, int> boneIDMap;
//    int boneCount;
//
//public:
//    SimpleBoneController() : boneCount(0) {
//        // 初始化默认骨骼
//    }
//
//    // 复制构造函数
//    SimpleBoneController(const SimpleBoneController& other) {
//        this->boneTransforms = other.boneTransforms;
//        this->boneIDMap = other.boneIDMap;
//        this->boneCount = other.boneCount;
//    }
//    void SetupFromModel(Model& model) {
//        auto& boneInfoMap = model.GetBoneInfoMap();
//        boneCount = model.GetBoneCount();
//
//        std::cout << "模型包含 " << boneCount << " 个骨骼" << std::endl;
//
//        // 初始化所有骨骼为单位矩阵
//        for (auto& pair : boneInfoMap) {
//            std::cout << "骨骼名称: '" << pair.first << "'" << std::endl;
//            std::cout << "  骨骼ID: " << pair.second.id << std::endl;
//            std::cout << "  名称长度: " << pair.first.length() << std::endl;
//
//            // 检查名称中是否有特殊字符
//            for (char c : pair.first) {
//                std::cout << "  字符: '" << c << "' ASCII: " << (int)c << std::endl;
//            }
//
//            boneTransforms[pair.first] = glm::mat4(1.0f);
//            boneIDMap[pair.first] = pair.second.id;
//        }
//    }
//
//    void RotateBone(const std::string& boneName, float angleX, float angleY, glm::vec3 front = glm::vec3(0.0f, 1.0f, 0.0f)) {
//        if (boneTransforms.find(boneName) != boneTransforms.end()) {
//            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(1.0f, 0.0f, 0.0f));
//            rotation = glm::rotate(rotation, glm::radians(angleX), glm::vec3(0.0f, 1.0f, 0.0f));
//            std::cout << "旋转骨骼 " << boneName << " - X: " << angleX << " Y: " << angleY << std::endl;
//            boneTransforms[boneName] = rotation;
//        }
//        else {
//            std::cout << "未找到骨骼: " << boneName << std::endl;
//            // 打印所有可用的骨骼名称
//            std::cout << "可用的骨骼: ";
//            for (auto& pair : boneTransforms) {
//                std::cout << pair.first << " ";
//            }
//            std::cout << std::endl;
//        }
//    }
//
//    std::vector<glm::mat4> GetBoneMatrices() {
//        std::vector<glm::mat4> matrices(100, glm::mat4(1.0f));
//
//        for (auto& pair : boneTransforms) {
//            std::string boneName = pair.first;
//            if (boneIDMap.find(boneName) != boneIDMap.end()) {
//                int boneID = boneIDMap[boneName];
//                if (boneID >= 0 && boneID < 100) {
//                    matrices[boneID] = pair.second;
//                }
//            }
//        }
//
//        return matrices;
//    }
//};
////
//SimpleBoneController boneController;
//
//// 基础设置（窗口尺寸等
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//bool bloom = true;
//bool bloomKeyPressed = false;
//float exposure = 1.0f;
//
//// 鼠标控制
//bool resetMouseNextFrame = false;
//
//// camera
//Camera camera(glm::vec3(0.0f, 20.0f, 40.0f));
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
////光源位置
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//
//// 动画控制
//bool isAnimating = false;
//bool useBones = true;
//float currentRotationX = 0.0f;
//float currentRotationY = 0.0f;
//
//// OpenAL 变量
////ALCdevice* audioDevice = nullptr;
////ALCcontext* audioContext = nullptr;
////ALuint audioSource, audioBuffer;
//Audio3DSystem audioSystem;
//ALuint explosionBuffer, backgroundBuffer, movingSourceBuffer;
//ALuint explosionSource, backgroundSource, movingSource;
//
//GLFWwindow* globalWindow = nullptr;
//
//// 在全局变量部分添加
//bool lampControlMode = false;  // 灯具控制模式
//bool lampKeyPressed = false; // 切换灯具控制模式的按键状态
//int currentLampIndex = 0;  // 当前控制的灯具索引 (0-7)
//
//// 每个灯具独立的旋转角度
//std::vector<float> lampRotationX;  // 每个灯具的X旋转角度
//std::vector<float> lampRotationY;  // 每个灯具的Y旋转角度
//// 灯具实例数组
//std::vector<SimpleBoneController> lampBoneControllers;
//std::vector<glm::vec3> lampPositions;  // 每个灯具的位置
//
//// 骨骼动画相关
//Model* stageLightModel = nullptr;
//Animator* animator = nullptr;
//Animation* lampAnimation = nullptr;
//
//
//// 音频加载函数
////bool loadOGGAudio(const std::string& filename) {
////    SF_INFO sfInfo;
////    SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &sfInfo);
////    if (!file) {
////        std::cout << "Failed to open OGG file: " << filename << std::endl;
////        return false;
////    }
////
////    // 读取PCM数据
////    std::vector<short> pcmData(sfInfo.frames * sfInfo.channels);
////    sf_read_short(file, pcmData.data(), pcmData.size());
////
////    // 确定OpenAL格式
////    ALenum format;
////    if (sfInfo.channels == 1) format = AL_FORMAT_MONO16;
////    else if (sfInfo.channels == 2) format = AL_FORMAT_STEREO16;
////    else {
////        sf_close(file);
////        return false;
////    }
////
////    // 上传到OpenAL
////    alBufferData(audioBuffer, format, pcmData.data(),
////        pcmData.size() * sizeof(short), sfInfo.samplerate);
////
////    sf_close(file);
////    std::cout << "OGG loaded: " << filename << std::endl;
////    return true;
////}
//// 简化骨骼控制器（用于没有动画文件的情况）
//
//// 音频系统更新函数
//void updateAudioSystem() {
//    static float timer = 0.0f;
//    static bool explosionPlayed = false;
//    static float movingAngle = 0.0f;
//    static bool spacePressed = false;
//    timer += deltaTime;
//
//    // 更新听者位置（跟随相机）
//    audioSystem.setListener(camera.Position, camera.Front, camera.Up);
//
//    // 3秒后播放爆炸声（右侧）
//    if (timer > 3.0f && !explosionPlayed) {
//        audioSystem.playSource(explosionSource);
//        explosionPlayed = true;
//        std::cout << "爆炸声从右侧传来！" << std::endl;
//    }
//
//    // 创建环绕音效 - 移动声源在圆周上运动
//    movingAngle += deltaTime * 1.0f; // 每秒1弧度
//    float radius = 8.0f;
//    glm::vec3 movingPosition(
//        cos(movingAngle) * radius,
//        0.0f,
//        sin(movingAngle) * radius
//    );
//
//    audioSystem.moveSource(movingSource, movingPosition);
//
//    // 自动播放移动声源
//    ALint state;
//    alGetSourcei(movingSource, AL_SOURCE_STATE, &state);
//    if (state != AL_PLAYING) {
//        audioSystem.playSource(movingSource);
//    }
//
//    // 测试：按空格键在玩家位置播放音效
//    if (glfwGetKey(globalWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
//        if (!spacePressed) {
//            ALuint testSource = audioSystem.create3DSource(movingSourceBuffer, camera.Position);
//            audioSystem.playSource(testSource);
//            spacePressed = true;
//        }
//    }
//    else {
//        spacePressed = false;
//    }
//}
//
////void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
////{
////    if (key == GLFW_KEY_K && action == GLFW_PRESS)
////    {
////        isAnimating = !isAnimating;
////        std::cout << "Animation " << (isAnimating ? "started" : "stopped") << std::endl;
////    }
////    if (key == GLFW_KEY_B && action == GLFW_PRESS)
////    {
////        useBones = !useBones;
////        std::cout << "Bones " << (useBones ? "enabled" : "disabled") << std::endl;
////    }
////}
//
////视口调整函数
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//        float currentFrame = (float)glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//        const float cameraSpeed = 30.0f * deltaTime;
//
//        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !lampKeyPressed)
//        {
//            lampControlMode = !lampControlMode;
//            isAnimating = lampControlMode; // 同步动画状态
//            cout << (lampControlMode ? "进入灯具控制模式" : "退出灯具控制模式") << endl;
//            lampKeyPressed = true;// 标记下一帧需要重置鼠标
//
//            // 重置鼠标位置
//            if (lampControlMode) {
//                glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);
//                resetMouseNextFrame = true;
//            }
//        }
//
//        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
//        {
//            lampKeyPressed = false;
//        }
//        // 数字键选择灯具
//        for (int i = 0; i < NUM_LAMPS; i++) {
//            if (glfwGetKey(window, GLFW_KEY_1 + i) == GLFW_PRESS) {
//                currentLampIndex = i;
//                cout << "切换到灯具: " << (currentLampIndex + 1)
//                    << " 角度(X:" << lampRotationX[currentLampIndex]
//                    << " Y:" << lampRotationY[currentLampIndex] << ")" << endl;
//            }
//        }
//        if (!lampControlMode)
//        {
//            glm::vec3 originalPos = camera.Position;
//
//            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//                camera.ProcessKeyboard(FORWARD, deltaTime);
//            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//                camera.ProcessKeyboard(BACKWARD, deltaTime);
//            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//                camera.ProcessKeyboard(LEFT, deltaTime);
//            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//                camera.ProcessKeyboard(RIGHT, deltaTime);
//
//            float groundLevel = -10.0f;  // 地面在Y=-10
//            float minCameraHeight = groundLevel + 1.0f;  // 相机最低在Y=-9（离地面1单位）
//
//            if (camera.Position.y < minCameraHeight) {
//                camera.Position.y = minCameraHeight;
//            }
//
//            // 可选：添加下降限制（比如按Shift下降）
//            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
//                camera.Position.y -= cameraSpeed * 2.0f;  // 快速下降
//                // 同样应用限制
//                if (camera.Position.y < minCameraHeight) {
//                    camera.Position.y = minCameraHeight;
//                }
//            }
//            // 可选：上升（比如按空格）
//            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
//                camera.Position.y += cameraSpeed * 2.0f;  // 快速上升
//            }
//
//            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !bloomKeyPressed)
//            {
//                bloom = !bloom;
//                bloomKeyPressed = true;
//            }
//            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
//            {
//                bloomKeyPressed = false;
//            }
//
//            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//            {
//                if (exposure > 0.0f)
//                    exposure -= 0.001f;
//                else
//                    exposure = 0.0f;
//            }
//            else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//            {
//                exposure += 0.001f;
//            }
//        }
//    
//}
//
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//    float xpos = static_cast<float>(xposIn);
//    float ypos = static_cast<float>(yposIn);
//
//    static bool firstMouse = true;
//    static float lastX = SCR_WIDTH / 2.0f;
//    static float lastY = SCR_HEIGHT / 2.0f;
//
//    // 如果需要重置鼠标，忽略这一帧的移动
//    if (resetMouseNextFrame) {
//        lastX = SCR_WIDTH / 2.0f;
//        lastY = SCR_HEIGHT / 2.0f;
//        resetMouseNextFrame = false;
//        return;
//    }
//
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//        return;  // 第一次调用时不处理
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//    lastX = xpos;
//    lastY = ypos;
//
//    // 只在有实际移动时才处理
//    if (fabs(xoffset) < 0.1f && fabs(yoffset) < 0.1f) {
//        return;  // 忽略微小移动
//    }
//
//    // 根据控制模式选择不同的行为
//    if (lampControlMode)
//    {
//        // 灯具控制模式：旋转灯具骨骼
//        float sensitivity = 0.5f;
//        xoffset *= sensitivity;
//        yoffset *= sensitivity;
//
//        // 更新当前灯具的旋转角度（累积）
//        lampRotationX[currentLampIndex] += xoffset;
//        lampRotationY[currentLampIndex] += yoffset;
//
//        // 限制角度范围
//        //lampRotationY[currentLampIndex] = glm::clamp(lampRotationY[currentLampIndex], -80.0f, 80.0f);
//        //lampRotationX[currentLampIndex] = glm::clamp(lampRotationX[currentLampIndex], -80.0f, 80.0f);
//        if (lampRotationY[currentLampIndex] > 80.0f) lampRotationY[currentLampIndex] = 80.0f;
//        if (lampRotationY[currentLampIndex] < -80.0f) lampRotationY[currentLampIndex] = -80.0f;
//        if (lampRotationX[currentLampIndex] > 80.0f) lampRotationX[currentLampIndex] = 80.0f;
//        if (lampRotationX[currentLampIndex] < -80.0f) lampRotationX[currentLampIndex] = -80.0f;
//
//        // 更新当前选中灯具的骨骼
//        lampBoneControllers[currentLampIndex].RotateBone("LampBone",
//            lampRotationX[currentLampIndex],
//            lampRotationY[currentLampIndex]);
//
//        std::cout << "旋转灯具 " << (currentLampIndex + 1)
//            << " - X: " << lampRotationX[currentLampIndex]
//            << " Y: " << lampRotationY[currentLampIndex] << std::endl;
//    }
//    else
//    {
//        // 相机控制模式：旋转相机
//        camera.ProcessMouseMovement(xoffset, yoffset);
//    }
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(static_cast<float>(yoffset));
//}
//
////
//void lampScene(const Shader& shader,int bonenumber = 0);
//void lampCube();
//
////加载纹理
//unsigned int loadTexture(char const* path)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}
//
////加载天空盒纹理
//unsigned int loadCubemap(vector<std::string> faces)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//
//    int width, height, nrComponents;
//    for (unsigned int i = 0; i < faces.size(); i++)
//    {
//        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
//        if (data)
//        {
//            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//            stbi_image_free(data);
//        }
//        else
//        {
//            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
//            stbi_image_free(data);
//        }
//    }
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//
//    return textureID;
//}
//
//void renderQuad();
//void renderCube();
//
//int main()
//{
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//
//    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//    glViewport(0, 0, 800, 600);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//
//    // tell GLFW to capture our mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    // 定义顶点数据和索引数据
//    float vertices[] = {
//        // positions          // colors           // texture coords
//         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
//         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
//        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
//    };
//    unsigned int indices[] = {
//        0, 1, 3,   // 第一个三角形
//        1, 2, 3    // 第二个三角形
//    };
//    float vertices2[] = {
//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
// 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
// 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
// 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
// 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
// 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//    };
//    float vertices3[] = {
//        // positions          // normals           // texture coords
//        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
//         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
//         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
//         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
//
//        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
//         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
//         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
//         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
//
//        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
//        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
//        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//
//         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
//         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//
//        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
//         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
//         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
//
//        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
//         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
//         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
//         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
//    };     
//    glm::vec3 cubePositions[] = {
//    glm::vec3(0.0f,  0.0f,  0.0f),
//    glm::vec3(2.0f,  5.0f, -15.0f),
//    glm::vec3(-1.5f, -2.2f, -2.5f),
//    glm::vec3(-3.8f, -2.0f, -12.3f),
//    glm::vec3(2.4f, -0.4f, -3.5f),
//    glm::vec3(-1.7f,  3.0f, -7.5f),
//    glm::vec3(1.3f, -2.0f, -2.5f),
//    glm::vec3(1.5f,  2.0f, -2.5f),
//    glm::vec3(1.5f,  0.2f, -1.5f),
//    glm::vec3(-1.3f,  1.0f, -1.5f)
//    };
//    float skyboxVertices[] = {
//        // positions          
//        -1.0f,  1.0f, -1.0f,
//        -1.0f, -1.0f, -1.0f,
//         1.0f, -1.0f, -1.0f,
//         1.0f, -1.0f, -1.0f,
//         1.0f,  1.0f, -1.0f,
//        -1.0f,  1.0f, -1.0f,
//
//        -1.0f, -1.0f,  1.0f,
//        -1.0f, -1.0f, -1.0f,
//        -1.0f,  1.0f, -1.0f,
//        -1.0f,  1.0f, -1.0f,
//        -1.0f,  1.0f,  1.0f,
//        -1.0f, -1.0f,  1.0f,
//
//         1.0f, -1.0f, -1.0f,
//         1.0f, -1.0f,  1.0f,
//         1.0f,  1.0f,  1.0f,
//         1.0f,  1.0f,  1.0f,
//         1.0f,  1.0f, -1.0f,
//         1.0f, -1.0f, -1.0f,
//
//        -1.0f, -1.0f,  1.0f,
//        -1.0f,  1.0f,  1.0f,
//         1.0f,  1.0f,  1.0f,
//         1.0f,  1.0f,  1.0f,
//         1.0f, -1.0f,  1.0f,
//        -1.0f, -1.0f,  1.0f,
//
//        -1.0f,  1.0f, -1.0f,
//         1.0f,  1.0f, -1.0f,
//         1.0f,  1.0f,  1.0f,
//         1.0f,  1.0f,  1.0f,
//        -1.0f,  1.0f,  1.0f,
//        -1.0f,  1.0f, -1.0f,
//
//        -1.0f, -1.0f, -1.0f,
//        -1.0f, -1.0f,  1.0f,
//         1.0f, -1.0f, -1.0f,
//         1.0f, -1.0f, -1.0f,
//        -1.0f, -1.0f,  1.0f,
//         1.0f, -1.0f,  1.0f
//    };
//    float groundVertices[] = {
//        // 位置           // 纹理坐标
//        -1.0f, 0.0f, -1.0f,  0.0f, 0.0f,  // 左下
//         1.0f, 0.0f, -1.0f,  1.0f, 0.0f,  // 右下
//         1.0f, 0.0f,  1.0f,  1.0f, 1.0f,  // 右上
//        -1.0f, 0.0f,  1.0f,  0.0f, 1.0f   // 左上
//    };
//    unsigned int groundIndices[] = {
//    0, 1, 2,  // 第一个三角形
//    2, 3, 0   // 第二个三角形
//    };
//    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//        // positions   // texCoords
//        -1.0f,  1.0f,  0.0f, 1.0f,
//        -1.0f, -1.0f,  0.0f, 0.0f,
//         1.0f, -1.0f,  1.0f, 0.0f,
//
//        -1.0f,  1.0f,  0.0f, 1.0f,
//         1.0f, -1.0f,  1.0f, 0.0f,
//         1.0f,  1.0f,  1.0f, 1.0f
//    };
//    float instanceVertices[] = {
//        // positions     // colors
//        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
//         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
//        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,
//
//        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
//         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
//         0.05f,  0.05f,  0.0f, 1.0f, 1.0f
//    };
//    //Model ourModel("../LearnOpenGL/resources/objects/rock/rock.obj");
//    //Model lightModel("../LearnOpenGL/resources/objects/lightphone/1.obj");
//    Model rock("../LearnOpenGL/resources/objects/rock/rock.obj");
//    Model planet("../LearnOpenGL/resources/objects/planet/planet.obj");
//    //Model bar("C:/Users/Hibb/OneDrive/Desktop/mimia/source/bar_50M/model.dae");
//    Model milkbar("C:/Users/Hibb/OneDrive/Desktop/mimia/milkbar.fbx");
//    Model spotlight("C:/Users/Hibb/OneDrive/Desktop/mimia/shotlight/shotlight.fbx");
//    Model gan("C:/Users/Hibb/OneDrive/Desktop/mimia/shotlight/gan.fbx");
//    Model musicbox4("C:/Users/Hibb/OneDrive/Desktop/mimia/shotlight/musicbox4.fbx");
//
//    boneController.SetupFromModel(spotlight);
//    // 初始化8个灯具
//    lampBoneControllers.resize(NUM_LAMPS);
//    lampPositions.resize(NUM_LAMPS);
//    lampRotationX.resize(NUM_LAMPS, 0.0f);  // 初始化为0度
//    lampRotationY.resize(NUM_LAMPS, 0.0f);  // 初始化为0度
//    // 设置每个灯具的位置
//    for (float i = 0; i < 4; i++)
//        lampPositions[i] = glm::vec3(-18.0f + 11 * i, 20.0f, 30.0f);
//    for (float i = 4; i < 8; i++)
//        lampPositions[i] = glm::vec3(-18.0f + 11 * (i-4), 20.0f, 55.5f);
//    // !!! 重要：初始化每个灯具的骨骼控制器 !!!
//    for (int i = 0; i < NUM_LAMPS; i++) {
//        lampBoneControllers[i].SetupFromModel(spotlight);
//        std::cout << "Initialized bone controller for lamp " << (i + 1) << std::endl;
//    }
//
//    // 创建1
//    unsigned int VAO, VBO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//    {
//        // 绑定VAO
//        glBindVertexArray(VAO);
//
//        // 绑定VBO并复制顶点数据
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//        // 绑定EBO并复制索引数据
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//        // 位置属性
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//        // 颜色属性
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(1);
//        // 纹理属性
//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//        glEnableVertexAttribArray(2);
//        // 解绑
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindVertexArray(0);
//    }
//
//    // 创建2
//    unsigned int VAO2, VBO2, EBO2;
//    glGenVertexArrays(1, &VAO2);
//    glGenBuffers(1, &VBO2);
//    glGenBuffers(1, &EBO2);
//    {
//        // 绑定VAO
//        glBindVertexArray(VAO2);
//
//        // 绑定VBO并复制顶点数据
//        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
//
//        // 位置属性
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//        // 纹理属性
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(1);
//        // 解绑
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindVertexArray(0);
//    }
//
//    // 创建3（标准箱子VAO）
//    unsigned int VAO3,VBO3;
//    glGenVertexArrays(1, &VAO3);
//    glGenBuffers(1, &VBO3);
//    {
//        // 绑定VAO
//        glBindVertexArray(VAO3);
//
//        // 绑定VBO并复制顶点数据
//        glBindBuffer(GL_ARRAY_BUFFER, VBO3);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
//
//        // 位置属性
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//        // 法线属性
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(1);
//        // 纹理属性
//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//        glEnableVertexAttribArray(2);
//        // 解绑
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindVertexArray(0);
//    }
//
//    // 创建4
//    unsigned int VAO4, VBO4;
//    glGenVertexArrays(1, &VAO4);
//    glGenBuffers(1, &VBO4);
//    {
//        // 绑定VAO
//        glBindVertexArray(VAO4);
//
//        // 绑定VBO并复制顶点数据
//        glBindBuffer(GL_ARRAY_BUFFER, VBO4);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
//
//        // 位置属性
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//        // 解绑
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindVertexArray(0);
//    }
//
//    // 创建地面VAO
//    unsigned int groundVAO, groundVBO, groundEBO;
//    glGenVertexArrays(1, &groundVAO);
//    glGenBuffers(1, &groundVBO);
//    glGenBuffers(1, &groundEBO);
//    {
//
//        glBindVertexArray(groundVAO);
//
//        glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
//
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);
//
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(1);
//
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindVertexArray(0);
//    }
//    
//    // skybox VAO
//    unsigned int skyboxVAO, skyboxVBO;
//    glGenVertexArrays(1, &skyboxVAO);
//    glGenBuffers(1, &skyboxVBO);
//    {
//        glBindVertexArray(skyboxVAO);
//        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    }
//
//    // 点精灵
//    float points[] = {
//        // 位置 + 颜色
//        -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // 左上，红色
//         0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // 右上，绿色
//         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // 右下，蓝色
//        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f   // 左下，黄色
//    };
//    unsigned int pointVAO, pointVBO;
//    {
//        glGenVertexArrays(1, &pointVAO);
//        glGenBuffers(1, &pointVBO);
//        glBindVertexArray(pointVAO);
//        glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(1);
//    }
//
//    //实例化
//    glm::vec2 translations[100];
//    int index = 0;
//    float offset = 0.1f;
//    for (int y = -10; y < 10; y += 2)
//    {
//        for (int x = -10; x < 10; x += 2)
//        {
//            glm::vec2 translation;
//            translation.x = (float)x / 10.0f + offset;
//            translation.y = (float)y / 10.0f + offset;
//            translations[index++] = translation;
//        }
//    }
//    unsigned int instanceVBO;
//    {
//        glGenBuffers(1, &instanceVBO);
//        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//    }
//    unsigned int instanceVAO, instanceVBO1;
//    {
//        glGenVertexArrays(1, &instanceVAO);
//        glGenBuffers(1, &instanceVBO1);
//        glBindVertexArray(instanceVAO);
//        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO1);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(instanceVertices), instanceVertices, GL_STATIC_DRAW);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
//        glEnableVertexAttribArray(2);
//        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute
//    }
//
//    //小行星
//    unsigned int amount = 1000;
//    glm::mat4* modelMatrices;
//    modelMatrices = new glm::mat4[amount];
//    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
//    float radius = 150.0;
//    float offset1 = 25.0f;
//    for (unsigned int i = 0; i < amount; i++)
//    {
//        glm::mat4 model = glm::mat4(1.0f);
//        // 1. translation: displace along circle with 'radius' in range [-offset1, offset1]
//        float angle = (float)i / (float)amount * 360.0f;
//        float displacement = (rand() % (int)(2 * offset1 * 100)) / 100.0f - offset1;
//        float x = sin(angle) * radius + displacement;
//        displacement = (rand() % (int)(2 * offset1 * 100)) / 100.0f - offset1;
//        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
//        displacement = (rand() % (int)(2 * offset1 * 100)) / 100.0f - offset1;
//        float z = cos(angle) * radius + displacement;
//        model = glm::translate(model, glm::vec3(x, y, z));
//
//        // 2. scale: Scale between 0.05 and 0.25f
//        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
//        model = glm::scale(model, glm::vec3(scale));
//
//        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
//        float rotAngle = static_cast<float>((rand() % 360));
//        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
//
//        // 4. now add to list of matrices
//        modelMatrices[i] = model;
//    }
//    //行星VBO
//    unsigned int buffer;
//    glGenBuffers(1, &buffer);
//    glBindBuffer(GL_ARRAY_BUFFER, buffer);
//    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
//    //行星VAO
//    for (unsigned int i = 0; i < rock.meshes.size(); i++)
//    {
//        unsigned int VAO = rock.meshes[i].VAO;
//        glBindVertexArray(VAO);
//        // set attribute pointers for matrix (4 times vec4)
//        glEnableVertexAttribArray(3);
//        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
//        glEnableVertexAttribArray(4);
//        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
//        glEnableVertexAttribArray(5);
//        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
//        glEnableVertexAttribArray(6);
//        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
//
//        glVertexAttribDivisor(3, 1);
//        glVertexAttribDivisor(4, 1);
//        glVertexAttribDivisor(5, 1);
//        glVertexAttribDivisor(6, 1);
//
//        glBindVertexArray(0);
//    }
//
//    //普通全屏贴图VAO
//    unsigned int quadVAO, quadVBO;
//    glGenVertexArrays(1, &quadVAO);
//    glGenBuffers(1, &quadVBO);
//    {
//        glBindVertexArray(quadVAO);
//        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
//    }
//
//    //创建帧缓冲区
//    unsigned int framebuffer;
//    glGenFramebuffers(1, &framebuffer);
//    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//    // 颜色附件纹理
//    unsigned int textureColorbuffer;
//    {
//        glGenTextures(1, &textureColorbuffer);
//        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
//    }
//    // 深度和模板附件
//    unsigned int rbo;
//    {
//        glGenRenderbuffers(1, &rbo);
//        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
//        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
//    }
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//
//
//    // 启用点大小控制
//    glEnable(GL_PROGRAM_POINT_SIZE);
//    
//    // 设置纹理
//    unsigned int texture = loadTexture("../LearnOpenGL/resources/textures/bricks2.jpg");
//    unsigned int diffuseMap = loadTexture("../LearnOpenGL/resources/textures/container2.png");
//    unsigned int specularMap = loadTexture("../LearnOpenGL/resources/textures/container2_specular.png");
//    unsigned int wood = loadTexture("../LearnOpenGL/resources/textures/wood.png");
//
//    Shader shaderProgram;
//    Shader lightingShader("color.vert", "color.frag");
//    Shader lightingUBOShader("color.vert", "color2.frag");
//    Shader lightCubeShader("light.vert", "light.frag");
//    Shader modelShader("model.vert", "model.frag");
//    Shader skyboxShader("skybox.vert", "skybox.frag");
//    Shader groundShader("ground.vert", "ground.frag");
//    Shader pointShader("point.vert", "point.frag");
//    Shader quadShader("quad.vert", "quad.frag");
//    Shader instanceShader("instance.vert", "instance.frag");
//    Shader asteroidShader("asteroids.vert", "asteroids.frag");
//    Shader planetShader("planet.vert", "planet.frag");
//
//    //天空盒纹理
//    vector<std::string> faces
//    {
//        "../LearnOpenGL/resources/textures/skybox/right.jpg",
//        "../LearnOpenGL/resources/textures/skybox/left.jpg",
//        "../LearnOpenGL/resources/textures/skybox/top.jpg",
//        "../LearnOpenGL/resources/textures/skybox/bottom.jpg",
//        "../LearnOpenGL/resources/textures/skybox/front.jpg",
//        "../LearnOpenGL/resources/textures/skybox/back.jpg",
//    };
//    unsigned int cubemapTexture = loadCubemap(faces);
//
//    //启动深度测试
//    glEnable(GL_DEPTH_TEST);
//
//    //SoundEngine->play2D(".. / LearnOpenGL / resources / audio/breakout.mp3", true);
//    // ============ 初始化3D音频系统 ============
//    //if (!audioSystem.initialize()) {
//    //    std::cout << "Failed to initialize audio system" << std::endl;
//    //    return -1;
//    //}
//    //// 加载各种音效
//    //{
//    //    explosionBuffer = audioSystem.loadAudio("../LearnOpenGL/resources/audio/solid.wav");
//    //    backgroundBuffer = audioSystem.loadAudio("../LearnOpenGL/resources/audio/");
//    //    movingSourceBuffer = audioSystem.loadAudio("../LearnOpenGL/resources/audio/powerup.wav");
//    //
//    //    // 创建3D音频源
//    //    explosionSource = audioSystem.create3DSource(explosionBuffer, glm::vec3(10.0f, 0.0f, 5.0f));
//    //    backgroundSource = audioSystem.create3DSource(backgroundBuffer, glm::vec3(0.0f, 0.0f, 0.0f), true); // 循环播放
//    //    movingSource = audioSystem.create3DSource(movingSourceBuffer, glm::vec3(-5.0f, 0.0f, 0.0f));
//    //
//    //    // 设置多普勒效应
//    //    audioSystem.setDopplerEffect(2.0f, 343.0f); // 增强多普勒效果
//    //
//    //    // 播放背景音乐
//    //    audioSystem.playSource(backgroundSource);
//    //
//    //    // 设置初始听者位置（跟随相机）
//    //    audioSystem.setListener(
//    //        camera.Position,
//    //        camera.Front,
//    //        camera.Up
//    //    );
//    //}
//
//    while (!glfwWindowShouldClose(window))
//    {
//        processInput(window);
//        // ============ 更新音频系统 ============
//        //updateAudioSystem();
//
//        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        if (0) {
//            shaderProgram.use();
//            shaderProgram.setInt("ourTexture", 0);
//
//            glm::mat4 model = glm::mat4(1.0f);
//            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
//
//            glm::mat4 view = camera.GetViewMatrix();
//
//            glm::mat4 projection;
//            projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//
//            shaderProgram.setMat4("view", view);
//            shaderProgram.setMat4("projection", projection);
//            shaderProgram.setMat4("model", model);
//
//            glBindTexture(GL_TEXTURE_2D, texture);
//            glBindVertexArray(VAO2);
//            for (unsigned int i = 0; i < 10; i++)
//            {
//                glm::mat4 model = glm::mat4(1.0f);
//                model = glm::translate(model, cubePositions[i]);
//                float angle = 20.0f * i;
//                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//                shaderProgram.setMat4("model", model);
//
//                glDrawArrays(GL_TRIANGLES, 0, 36);
//            }
//            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//        }
//        if (1==2)
//        {
//            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//
//            lightingShader.use();
//            lightingShader.setInt("material.diffuse", 0);
//            lightingShader.setInt("material.specular", 1);
//
//            lightingShader.setVec3("viewPos", camera.Position);
//            lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
//            lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
//            lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
//            lightingShader.setFloat("material.shininess", 32.0f);
//
//            lightingShader.setVec3("p_light.position", lightPos);
//            lightingShader.setVec3("p_light.ambient", 0.2f, 0.2f, 0.2f);
//            lightingShader.setVec3("p_light.diffuse", 0.5f, 0.5f, 0.5f);
//            lightingShader.setVec3("p_light.specular", 1.0f, 1.0f, 1.0f);
//
//            lightingShader.setFloat("p_light.constant", 1.0f);
//            lightingShader.setFloat("p_light.linear", 0.09f);
//            lightingShader.setFloat("p_light.quadratic", 0.032f);
//
//            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//            glm::mat4 view = camera.GetViewMatrix();
//            lightingShader.setMat4("projection", projection);
//            lightingShader.setMat4("view", view);
//
//            glm::mat4 model = glm::mat4(1.0f);
//            lightingShader.setMat4("model", model);
//
//            glBindVertexArray(VAO3);
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, diffuseMap);
//            glActiveTexture(GL_TEXTURE1);
//            glBindTexture(GL_TEXTURE_2D, specularMap);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//
//            //lightingUBOShader.use();
//            //
//            //// 设置材质uniform（不是UBO的一部分）
//            //lightingUBOShader.setInt("material.diffuse", 0);
//            //lightingUBOShader.setInt("material.specular", 1);
//            //lightingUBOShader.setFloat("material.shininess", 32.0f);
//            //lightingUBOShader.setVec3("viewPos", camera.Position);
//            //
//            //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//            //glm::mat4 view = camera.GetViewMatrix();
//            //lightingUBOShader.setMat4("projection", projection);
//            //lightingUBOShader.setMat4("view", view);
//            //
//            //glm::mat4 model = glm::mat4(1.0f);
//            //lightingUBOShader.setMat4("model", model);
//            //
//            //glBindVertexArray(VAO3);
//            //glActiveTexture(GL_TEXTURE0);
//            //glBindTexture(GL_TEXTURE_2D, diffuseMap);
//            //glActiveTexture(GL_TEXTURE1);
//            //glBindTexture(GL_TEXTURE_2D, specularMap);
//            //glDrawArrays(GL_TRIANGLES, 0, 36);
//
//            lightCubeShader.use();
//            lightCubeShader.setMat4("projection", projection);
//            lightCubeShader.setMat4("view", view);
//            model = glm::mat4(1.0f);
//            model = glm::translate(model, lightPos);
//            model = glm::scale(model, glm::vec3(0.2f));
//            lightCubeShader.setMat4("model", model);
//
//            glBindVertexArray(VAO4);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//
//
//        }
//        if (0)
//        {
//            modelShader.use();
//            glm::mat4 model = glm::mat4(1.0f);
//            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//            glm::mat4 view = camera.GetViewMatrix();
//            modelShader.setMat4("model", model);
//            modelShader.setMat4("projection", projection);
//            modelShader.setMat4("view", view);
//
//            // 设置光照参数
//            modelShader.setVec3("light.position", lightPos);
//            modelShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
//            modelShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
//            modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//            modelShader.setVec3("viewPos", camera.Position);
//            modelShader.setFloat("material.shininess", 32.0f);
//
//            // 绘制模型
//            rock.Draw(modelShader);
//        }
//        if (0)
//        {
//            modelShader.use();
//            glm::mat4 model = glm::mat4(1.0f);
//            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//            glm::mat4 view = camera.GetViewMatrix();
//            modelShader.setMat4("model", model);
//            modelShader.setMat4("projection", projection);
//            modelShader.setMat4("view", view);
//
//            // 设置光照参数
//            modelShader.setVec3("light.position", lightPos);
//            modelShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
//            modelShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
//            modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//            modelShader.setVec3("viewPos", camera.Position);
//            modelShader.setFloat("material.shininess", 32.0f);
//
//            // 绘制模型
//           rock.Draw(modelShader);
//
//            // draw skybox as last
//            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
//            skyboxShader.use();
//            skyboxShader.setInt("skybox", 1);
//
//            view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
//            skyboxShader.setMat4("view", view);
//            skyboxShader.setMat4("projection", projection);
//            // skybox cube
//            glBindVertexArray(skyboxVAO);
//            glActiveTexture(GL_TEXTURE1);
//            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//            glBindVertexArray(0);
//            glDepthFunc(GL_LESS); // set depth function back to default
//
//        }
//        if (0)
//        {
//            pointShader.use();
//            glm::mat4 model = glm::mat4(1.0f);
//            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//            glm::mat4 view = camera.GetViewMatrix();
//            float time = (float)glfwGetTime();
//            pointShader.setMat4("model", model);
//            pointShader.setMat4("projection", projection);
//            pointShader.setMat4("view", view);
//            pointShader.setFloat("time", time);
//
//            glBindVertexArray(pointVAO);
//            glDrawArrays(GL_POINTS, 0, 4);
//            glBindVertexArray(0);
//
//        }
//        if (1==2)
//        {
//
//            //modelShader.use();
//            //glm::mat4 model = glm::mat4(1.0f);
//            //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));  // 移动到(0,0,3)
//            //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//            //model = glm::scale(model, glm::vec3(0.01f));
//            //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//            //glm::mat4 view = camera.GetViewMatrix();
//            //modelShader.setMat4("model", model);
//            //modelShader.setMat4("projection", projection);
//            //modelShader.setMat4("view", view);
//            //
//            //// 设置光照参数
//            //modelShader.setVec3("light.position", lightPos);
//            //modelShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
//            //modelShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
//            //modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//            //modelShader.setVec3("viewPos", camera.Position);
//            //modelShader.setFloat("material.shininess", 32.0f);
//            //
//            //// 绘制模型
//            //lightModel.Draw(modelShader);
//
//            glm::mat4 model = glm::mat4(1.0f);
//            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//            glm::mat4 view = camera.GetViewMatrix();
//
//
//            groundShader.use();
//
//            groundShader.setMat4("view", view);
//            groundShader.setMat4("projection", projection);
//            groundShader.setMat4("model", glm::mat4(1.0f));
//            groundShader.setVec3("cameraPos", camera.Position);
//
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, wood);
//            groundShader.setInt("groundTexture", 0);
//
//            glBindVertexArray(groundVAO);
//            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//            glBindVertexArray(0);
//
//            // second pass
//            glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
//            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//            glClear(GL_COLOR_BUFFER_BIT);
//
//            quadShader.use();
//            glBindVertexArray(quadVAO);
//            glDisable(GL_DEPTH_TEST);
//            glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
//            glDrawArrays(GL_TRIANGLES, 0, 6);
//
//
//        }
//        //实例化
//        if (0)
//        {
//            instanceShader.use();
//            glBindVertexArray(instanceVAO);
//            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
//            glBindVertexArray(0);
//        }
//        //小行星
//        if (0)
//        {
//            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
//            glm::mat4 view = camera.GetViewMatrix();
//            asteroidShader.use();
//            asteroidShader.setMat4("projection", projection);
//            asteroidShader.setMat4("view", view);
//            planetShader.use();
//            planetShader.setMat4("projection", projection);
//            planetShader.setMat4("view", view);
//
//            // draw planet
//            glm::mat4 model = glm::mat4(1.0f);
//            model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
//            model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
//            planetShader.setMat4("model", model);
//            planet.Draw(planetShader);
//
//            // draw meteorites
//            asteroidShader.use();
//            asteroidShader.setInt("texture_diffuse1", 0);
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
//            for (unsigned int i = 0; i < rock.meshes.size(); i++)
//            {
//                glBindVertexArray(rock.meshes[i].VAO);
//                glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(rock.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
//                glBindVertexArray(0);
//            }
//        }
//        //阴影
//        if (0)
//        {
//            instanceShader.use();
//            glBindVertexArray(instanceVAO);
//            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
//            glBindVertexArray(0);
//        }
//        //bar model
//        if (1)
//        {
//            modelShader.use();
//            glm::mat4 model = glm::mat4(1.0f);
//            model = glm::scale(model, glm::vec3(0.01f));
//            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//            glm::mat4 view = camera.GetViewMatrix();
//            modelShader.setMat4("model", model);
//            modelShader.setMat4("projection", projection);
//            modelShader.setMat4("view", view);
//
//            // 设置光照参数
//            modelShader.setVec3("light.position", lightPos);
//            modelShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
//            modelShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
//            modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//            modelShader.setVec3("viewPos", camera.Position);
//            modelShader.setFloat("material.shininess", 32.0f);
//
//            // 绘制模型
//            milkbar.Draw(modelShader);
//        }
//        //light model
//        if (1)
//        {
//            modelShader.use();
//            modelShader.setBool("useBones", useBones);
//
//            //glm::mat4 model = glm::mat4(1.0f);
//            //model = glm::translate(model, glm::vec3(0.0f, 20.0f, 30.0f));
//            //model = glm::scale(model, glm::vec3(4.0f));
//            //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//            //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//            //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//            glm::mat4 view = camera.GetViewMatrix();
//            //modelShader.setMat4("model", model);
//            modelShader.setMat4("projection", projection);
//            modelShader.setMat4("view", view);
//
//            // 设置光照参数
//            modelShader.setVec3("light.position", lightPos);
//            modelShader.setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
//            modelShader.setVec3("light.diffuse", 0.6f, 0.6f, 0.6f);
//            modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//            modelShader.setVec3("viewPos", camera.Position);
//            modelShader.setFloat("material.shininess", 32.0f);
//
//            //// 获取骨骼矩阵并传递给着色器
//            //auto boneTransforms = boneController.GetBoneMatrices();
//            //for (int i = 0; i < boneTransforms.size(); ++i) {
//            //    std::string name = "boneMatrices[" + std::to_string(i) + "]";
//            //    modelShader.setMat4(name, boneTransforms[i]);
//            //}
//
//            // 绘制所有灯具
//            for (int i = 0; i < NUM_LAMPS; i++) {
//                // 设置每个灯具的模型矩阵（位置）
//                glm::mat4 model = glm::mat4(1.0f);
//                model = glm::translate(model, lampPositions[i]);
//                model = glm::scale(model, glm::vec3(4.0f));  // 适当缩放
//                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//                if(i>= NUM_LAMPS/2)
//                    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//
//                modelShader.setMat4("model", model);
//
//                // 更新骨骼变换
//                if (isAnimating) {
//                    boneController.RotateBone("LampBone", lampRotationX[currentLampIndex], lampRotationY[currentLampIndex]);
//                }
//                // 设置当前灯具的骨骼矩阵（每个灯具都有自己的骨骼状态）
//                auto boneTransforms = lampBoneControllers[i].GetBoneMatrices();
//                for (int j = 0; j < boneTransforms.size(); ++j) {
//                    std::string name = "boneMatrices[" + std::to_string(j) + "]";
//                    modelShader.setMat4(name, boneTransforms[j]);
//                }
//                // 绘制模型
//                spotlight.Draw(modelShader);
//            }
//        }
//        //天空盒
//        if (1)
//        {
//            // draw skybox as last
//            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
//            skyboxShader.use();
//            skyboxShader.setInt("skybox", 1);
//
//            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//            glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
//            skyboxShader.setMat4("view", view);
//            skyboxShader.setMat4("projection", projection);
//            // skybox cube
//            glBindVertexArray(skyboxVAO);
//            glActiveTexture(GL_TEXTURE1);
//            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//            glBindVertexArray(0);
//            glDepthFunc(GL_LESS); // set depth function back to default
//        }
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//
//    glDeleteVertexArrays(1, &VAO2);
//    glDeleteBuffers(1, &VBO2);
//    glDeleteBuffers(1, &EBO2);
//
//    glDeleteVertexArrays(1, &VAO3);
//    glDeleteBuffers(1, &VBO3);
//
//    glDeleteVertexArrays(1, &groundVAO);
//    glDeleteBuffers(1, &groundVBO);
//
//    audioSystem.cleanup();
//
//    glfwTerminate();
//    return 0;
//}
//
//
//unsigned int cubeVAO = 0;
//unsigned int cubeVBO = 0;
//void renderCube()
//{
//    // initialize (if necessary)
//    if (cubeVAO == 0)
//    {
//        float vertices[] = {
//            // back face
//            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
//             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
//             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
//             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
//            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
//            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
//            // front face
//            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
//             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
//             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
//             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
//            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
//            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
//            // left face
//            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
//            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
//            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
//            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
//            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
//            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
//            // right face
//             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
//             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
//             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
//             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
//             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
//             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
//             // bottom face
//             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
//              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
//              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
//              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
//             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
//             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
//             // top face
//             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
//              1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
//              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
//              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
//             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
//             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
//        };
//        glGenVertexArrays(1, &cubeVAO);
//        glGenBuffers(1, &cubeVBO);
//        // fill buffer
//        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//        // link vertex attributes
//        glBindVertexArray(cubeVAO);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(2);
//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindVertexArray(0);
//    }
//    // render Cube
//    glBindVertexArray(cubeVAO);
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//    glBindVertexArray(0);
//}
//
//unsigned int quadVAO = 0;
//unsigned int quadVBO;
//void renderQuad()
//{
//    if (quadVAO == 0)
//    {
//        float quadVertices[] = {
//            // positions        // texture Coords
//            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
//            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
//             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//        };
//        // setup plane VAO
//        glGenVertexArrays(1, &quadVAO);
//        glGenBuffers(1, &quadVBO);
//        glBindVertexArray(quadVAO);
//        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    }
//    glBindVertexArray(quadVAO);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    glBindVertexArray(0);
//}
//
////void lampScene(const Shader& shader,int bonenumber)
////{
////    modelShader.use();
////    modelShader.setBool("useBones", useBones);
////
////    glm::mat4 model = glm::mat4(1.0f);
////    model = glm::translate(model, glm::vec3(0.0f, 20.0f, 30.0f));
////    model = glm::scale(model, glm::vec3(4.0f));
////    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
////    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
////    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
////    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
////    glm::mat4 view = camera.GetViewMatrix();
////    modelShader.setMat4("model", model);
////    modelShader.setMat4("projection", projection);
////    modelShader.setMat4("view", view);
////
////    // 设置光照参数
////    modelShader.setVec3("light.position", lightPos);
////    modelShader.setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
////    modelShader.setVec3("light.diffuse", 0.6f, 0.6f, 0.6f);
////    modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
////    modelShader.setVec3("viewPos", camera.Position);
////    modelShader.setFloat("material.shininess", 32.0f);
////
////    // 更新骨骼变换
////    if (isAnimating) {
////        boneController.RotateBone("LampBone", currentRotationX, currentRotationY);
////    }
////
////    // 获取骨骼矩阵并传递给着色器
////    auto boneTransforms = boneController.GetBoneMatrices();
////    for (int i = 0; i < boneTransforms.size(); ++i) {
////        std::string name = "boneMatrices[" + std::to_string(i) + "]";
////        modelShader.setMat4(name, boneTransforms[i]);
////    }
////    // 绘制模型
////    spotlight.Draw(modelShader);
////
////    // room cube
////    glm::mat4 model = glm::mat4(1.0f);
////    model = glm::scale(model, glm::vec3(5.0f));
////    shader.setMat4("model", model);
////    glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
////    shader.setInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
////    renderCube();
////    shader.setInt("reverse_normals", 0); // and of course disable it
////    glEnable(GL_CULL_FACE);
////    // cubes
////    model = glm::mat4(1.0f);
////    model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
////    model = glm::scale(model, glm::vec3(0.5f));
////    shader.setMat4("model", model);
////    renderCube();
////    model = glm::mat4(1.0f);
////    model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
////    model = glm::scale(model, glm::vec3(0.75f));
////    shader.setMat4("model", model);
////    renderCube();
////    model = glm::mat4(1.0f);
////    model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
////    model = glm::scale(model, glm::vec3(0.5f));
////    shader.setMat4("model", model);
////    renderCube();
////    model = glm::mat4(1.0f);
////    model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
////    model = glm::scale(model, glm::vec3(0.5f));
////    shader.setMat4("model", model);
////    renderCube();
////    model = glm::mat4(1.0f);
////    model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
////    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
////    model = glm::scale(model, glm::vec3(0.75f));
////    shader.setMat4("model", model);
////    renderCube();
////}
////unsigned int cubeVAO = 0;
////unsigned int cubeVBO = 0;
////void lampCube()
////{
////    // initialize (if necessary)
////    modelShader.use();
////    modelShader.setBool("useBones", useBones);
////
////    glm::mat4 model = glm::mat4(1.0f);
////    model = glm::translate(model, glm::vec3(0.0f, 20.0f, 30.0f));
////    model = glm::scale(model, glm::vec3(4.0f));
////    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
////    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
////    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
////    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
////    glm::mat4 view = camera.GetViewMatrix();
////    modelShader.setMat4("model", model);
////    modelShader.setMat4("projection", projection);
////    modelShader.setMat4("view", view);
////
////    // 设置光照参数
////    modelShader.setVec3("light.position", lightPos);
////    modelShader.setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
////    modelShader.setVec3("light.diffuse", 0.6f, 0.6f, 0.6f);
////    modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
////    modelShader.setVec3("viewPos", camera.Position);
////    modelShader.setFloat("material.shininess", 32.0f);
////
////    // 更新骨骼变换
////    if (isAnimating) {
////        boneController.RotateBone("LampBone", currentRotationX, currentRotationY);
////    }
////
////    // 获取骨骼矩阵并传递给着色器
////    auto boneTransforms = boneController.GetBoneMatrices();
////    for (int i = 0; i < boneTransforms.size(); ++i) {
////        std::string name = "boneMatrices[" + std::to_string(i) + "]";
////        modelShader.setMat4(name, boneTransforms[i]);
////    }
////    // 绘制模型
////    spotlight.Draw(modelShader);
////}