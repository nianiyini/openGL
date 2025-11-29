//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <cmath>
//#include <vector>
//#define STB_IMAGE_IMPLEMENTATION
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#pragma execution_character_set("utf-8")
////#include "stb_image.h"
//// 
//// 行星数据结构
//struct Planet {
//    const char* name;
//    float orbitRadius;
//    float size;
//    float orbitSpeed;
//    float inclination;
//    float rotationAngle;
//    float color[3];
//    bool hasRings;
//};
//
//// 行星数据
//std::vector<Planet> planets = {
//    {"水星", 80.0f, 8.0f, 4.0f, 7.0f, 0.0f, {0.66f, 0.66f, 0.66f}, false},
//    {"金星", 100.0f, 12.0f, 2.0f, 3.4f, 0.0f, {0.85f, 0.65f, 0.13f}, false},
//    {"地球", 130.0f, 13.0f, 1.0f, 0.0f, 0.0f, {0.0f, 0.0f, 1.0f}, false},
//    {"火星", 160.0f, 10.0f, 0.8f, 1.8f, 0.0f, {1.0f, 0.0f, 0.0f}, false},
//    {"木星", 200.0f, 20.0f, 0.4f, 1.3f, 0.0f, {0.82f, 0.71f, 0.55f}, false},
//    {"土星", 250.0f, 18.0f, 0.3f, 2.5f, 0.0f, {1.0f, 0.84f, 0.0f}, true},
//    {"天王星", 290.0f, 15.0f, 0.2f, 0.8f, 0.0f, {0.68f, 0.85f, 0.9f}, false},
//    {"海王星", 320.0f, 14.0f, 0.1f, 1.8f, 0.0f, {0.0f, 0.0f, 0.55f}, false}
//};
//
//// 月球数据
//float moonOrbitRadius = 20.0f;
//float moonSize = 3.0f;
//float moonOrbitSpeed = 12.0f;
//float moonRotation = 0.0f;
//
//// 相机参数
//float cameraDistance = 500.0f;
//float cameraAngleX = 30.0f;
//float cameraAngleY = 0.0f;
//bool firstMouse = true;
//float lastX = 400, lastY = 300;
//
//// 窗口尺寸
//const unsigned int SCR_WIDTH = 1200;
//const unsigned int SCR_HEIGHT = 800;
//
//// 编译着色器
//unsigned int compileShader(GLenum type, const char* source) {
//    unsigned int shader = glCreateShader(type);
//    glShaderSource(shader, 1, &source, nullptr);
//    glCompileShader(shader);
//
//    int success;
//    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
//        std::cout << "着色器编译错误: " << infoLog << std::endl;
//    }
//    return shader;
//}
//
//// 创建着色器程序
//unsigned int createShaderProgram() {
//    const char* vertexShaderSource = R"(
//        #version 330 core
//        layout (location = 0) in vec3 aPos;
//        uniform mat4 model;
//        uniform mat4 view;
//        uniform mat4 projection;
//        void main() {
//            gl_Position = projection * view * model * vec4(aPos, 1.0);
//        }
//    )";
//
//    const char* fragmentShaderSource = R"(
//        #version 330 core
//        out vec4 FragColor;
//        uniform vec3 color;
//        void main() {
//            FragColor = vec4(color, 1.0);
//        }
//    )";
//
//    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
//    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
//
//    unsigned int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    return shaderProgram;
//}
//
//// 生成球体顶点数据
//void createSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices,
//    float radius, int sectors, int stacks) {
//    const float PI = 3.14159265359f;
//
//    // 生成顶点
//    for (int i = 0; i <= stacks; ++i) {
//        float stackAngle = PI / 2 - i * (PI / stacks);
//        float xy = radius * cosf(stackAngle);
//        float z = radius * sinf(stackAngle);
//
//        for (int j = 0; j <= sectors; ++j) {
//            float sectorAngle = j * 2 * PI / sectors;
//            float x = xy * cosf(sectorAngle);
//            float y = xy * sinf(sectorAngle);
//
//            vertices.push_back(x);
//            vertices.push_back(y);
//            vertices.push_back(z);
//        }
//    }
//
//    // 生成索引
//    for (int i = 0; i < stacks; ++i) {
//        for (int j = 0; j < sectors; ++j) {
//            int first = i * (sectors + 1) + j;
//            int second = first + sectors + 1;
//
//            indices.push_back(first);
//            indices.push_back(second);
//            indices.push_back(first + 1);
//
//            indices.push_back(first + 1);
//            indices.push_back(second);
//            indices.push_back(second + 1);
//        }
//    }
//}
//
//// 创建球体VAO
//unsigned int createSphereVAO(float radius) {
//    std::vector<float> vertices;
//    std::vector<unsigned int> indices;
//
//    createSphere(vertices, indices, radius, 36, 18);
//
//    unsigned int VAO, VBO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    return VAO;
//}
//
//// 创建圆环VAO（用于轨道和土星环）
//unsigned int createTorusVAO(float innerRadius, float outerRadius, int sides, int rings) {
//    std::vector<float> vertices;
//    std::vector<unsigned int> indices;
//
//    const float PI = 3.14159265359f;
//
//    for (int i = 0; i <= rings; i++) {
//        float theta = 2.0f * PI * i / rings;
//        float cosTheta = cosf(theta);
//        float sinTheta = sinf(theta);
//
//        for (int j = 0; j <= sides; j++) {
//            float phi = 2.0f * PI * j / sides;
//            float cosPhi = cosf(phi);
//            float sinPhi = sinf(phi);
//
//            float x = cosTheta * (outerRadius + innerRadius * cosPhi);
//            float y = sinTheta * (outerRadius + innerRadius * cosPhi);
//            float z = innerRadius * sinPhi;
//
//            vertices.push_back(x);
//            vertices.push_back(y);
//            vertices.push_back(z);
//        }
//    }
//
//    for (int i = 0; i < rings; i++) {
//        for (int j = 0; j < sides; j++) {
//            int first = i * (sides + 1) + j;
//            int second = first + sides + 1;
//
//            indices.push_back(first);
//            indices.push_back(second);
//            indices.push_back(first + 1);
//
//            indices.push_back(first + 1);
//            indices.push_back(second);
//            indices.push_back(second + 1);
//        }
//    }
//
//    unsigned int VAO, VBO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    return VAO;
//}
//
//// 鼠标回调
//void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
//    if (firstMouse) {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos;
//    lastX = xpos;
//    lastY = ypos;
//
//    float sensitivity = 0.1f;
//    xoffset *= sensitivity;
//    yoffset *= sensitivity;
//
//    cameraAngleY += xoffset;
//    cameraAngleX += yoffset;
//
//    if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
//    if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
//}
//
//// 滚轮回调
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
//    cameraDistance -= (float)yoffset;
//    if (cameraDistance < 100.0f) cameraDistance = 100.0f;
//    if (cameraDistance > 1000.0f) cameraDistance = 1000.0f;
//}
//
//int main() {
//    // 初始化GLFW
//    if (!glfwInit()) {
//        std::cout << "Failed to initialize GLFW" << std::endl;
//        return -1;
//    }
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // 创建窗口
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "太阳系 - GLFW版本", nullptr, nullptr);
//    if (!window) {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    // 初始化GLAD
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // 配置OpenGL
//    glEnable(GL_DEPTH_TEST);
//    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
//
//    // 创建着色器程序
//    unsigned int shaderProgram = createShaderProgram();
//
//    // 创建几何体
//    unsigned int sunVAO = createSphereVAO(25.0f);
//    unsigned int planetVAO = createSphereVAO(1.0f); // 大小在绘制时缩放
//    unsigned int orbitVAO = createTorusVAO(0.5f, 1.0f, 10, 50); // 轨道
//    unsigned int ringVAO = createTorusVAO(2.0f, 25.0f, 20, 30); // 土星环
//
//    // 获取uniform位置
//    int modelLoc = glGetUniformLocation(shaderProgram, "model");
//    int viewLoc = glGetUniformLocation(shaderProgram, "view");
//    int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
//    int colorLoc = glGetUniformLocation(shaderProgram, "color");
//
//    // 渲染循环
//    while (!glfwWindowShouldClose(window)) {
//        // 输入处理
//        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//            glfwSetWindowShouldClose(window, true);
//
//        // 清除缓冲区
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // 使用着色器程序
//        glUseProgram(shaderProgram);
//
//        // 视图矩阵
//        glm::mat4 view = glm::mat4(1.0f);
//        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -cameraDistance));
//        view = glm::rotate(view, glm::radians(cameraAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
//        view = glm::rotate(view, glm::radians(cameraAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
//        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
//
//        // 投影矩阵
//        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
//            (float)SCR_WIDTH / (float)SCR_HEIGHT,
//            0.1f, 1000.0f);
//        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
//
//        // 绘制太阳
//        glm::mat4 sunModel = glm::mat4(1.0f);
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &sunModel[0][0]);
//        glUniform3f(colorLoc, 1.0f, 1.0f, 0.0f); // 黄色
//        glBindVertexArray(sunVAO);
//        glDrawElements(GL_TRIANGLES, 36 * 18 * 6, GL_UNSIGNED_INT, 0);
//
//        // 绘制行星和轨道
//        for (auto& planet : planets) {
//            // 绘制轨道
//            glm::mat4 orbitModel = glm::mat4(1.0f);
//            orbitModel = glm::rotate(orbitModel, glm::radians(planet.inclination), glm::vec3(1.0f, 0.0f, 0.0f));
//            orbitModel = glm::scale(orbitModel, glm::vec3(planet.orbitRadius, planet.orbitRadius, planet.orbitRadius));
//            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &orbitModel[0][0]);
//            glUniform3f(colorLoc, 0.5f, 0.5f, 0.5f); // 灰色轨道
//            glBindVertexArray(orbitVAO);
//            glDrawElements(GL_TRIANGLES, 50 * 10 * 6, GL_UNSIGNED_INT, 0);
//
//            // 绘制行星
//            glm::mat4 planetModel = glm::mat4(1.0f);
//            planetModel = glm::rotate(planetModel, glm::radians(planet.inclination), glm::vec3(1.0f, 0.0f, 0.0f));
//            planetModel = glm::rotate(planetModel, glm::radians(planet.rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
//            planetModel = glm::translate(planetModel, glm::vec3(planet.orbitRadius, 0.0f, 0.0f));
//            planetModel = glm::scale(planetModel, glm::vec3(planet.size));
//            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &planetModel[0][0]);
//            glUniform3f(colorLoc, planet.color[0], planet.color[1], planet.color[2]);
//            glBindVertexArray(planetVAO);
//            glDrawElements(GL_TRIANGLES, 36 * 18 * 6, GL_UNSIGNED_INT, 0);
//
//            // 如果是地球，绘制月球
//            if (planet.name == std::string("地球")) {
//                glm::mat4 moonModel = planetModel; // 从地球位置开始
//                moonModel = glm::scale(moonModel, glm::vec3(1.0f / planet.size)); // 撤销地球缩放
//                moonModel = glm::rotate(moonModel, glm::radians(moonRotation), glm::vec3(0.0f, 1.0f, 0.0f));
//                moonModel = glm::translate(moonModel, glm::vec3(moonOrbitRadius, 0.0f, 0.0f));
//                moonModel = glm::scale(moonModel, glm::vec3(moonSize));
//                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &moonModel[0][0]);
//                glUniform3f(colorLoc, 0.8f, 0.8f, 0.8f); // 月球颜色
//                glBindVertexArray(planetVAO);
//                glDrawElements(GL_TRIANGLES, 36 * 18 * 6, GL_UNSIGNED_INT, 0);
//            }
//
//            // 如果是土星，绘制环
//            if (planet.hasRings) {
//                glm::mat4 ringModel = planetModel;
//                ringModel = glm::scale(ringModel, glm::vec3(1.0f / planet.size)); // 撤销行星缩放
//                ringModel = glm::rotate(ringModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &ringModel[0][0]);
//                glUniform3f(colorLoc, 0.82f, 0.71f, 0.55f); // 土星环颜色
//                glBindVertexArray(ringVAO);
//                glDrawElements(GL_TRIANGLES, 30 * 20 * 6, GL_UNSIGNED_INT, 0);
//            }
//
//            // 更新行星角度
//            planet.rotationAngle += planet.orbitSpeed;
//            if (planet.rotationAngle >= 360.0f) planet.rotationAngle = 0.0f;
//        }
//
//        // 更新月球角度
//        moonRotation += moonOrbitSpeed;
//        if (moonRotation >= 360.0f) moonRotation = 0.0f;
//
//        // 交换缓冲区和检查事件
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // 清理资源
//    glDeleteProgram(shaderProgram);
//    glfwTerminate();
//    return 0;
//}