//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <cmath>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#pragma execution_character_set("utf-8")
//
//// 窗口尺寸
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//
//// 顶点着色器源码 - 替换原来的vertexShaderSource
//const char* vertexShaderSource = R"(
//#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
//
//out vec3 ourColor;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//uniform float time;  // 添加时间uniform
//
//void main()
//{
//    gl_Position = projection * view * model * vec4(aPos, 1.0);
//    
//    // 基于顶点位置和时间创建动态颜色
//    vec3 dynamicColor = aColor * (0.7 + 0.3 * sin(time * 2.0 + aPos.x * 4.0 + aPos.y * 4.0 + aPos.z * 4.0));
//    ourColor = dynamicColor;
//}
//)";
//
//// 增强版片段着色器源码 - 更丰富的霓虹效果
//const char* fragmentShaderSource = R"(
//#version 330 core
//out vec4 FragColor;
//in vec3 ourColor;
//
//uniform float time;
//
//void main()
//{
//    // 多重频率的脉动效果
//    float pulse1 = 0.5 + 0.5 * sin(time * 2.0);
//    float pulse2 = 0.5 + 0.5 * sin(time * 3.14159);
//    float pulse3 = 0.5 + 0.5 * sin(time * 1.5);
//    
//    // 组合脉动效果
//    float combinedPulse = (pulse1 + pulse2 + pulse3) / 3.0;
//    
//    // 霓虹颜色增强
//    vec3 baseNeon = ourColor * (0.8 + 0.4 * combinedPulse);
//    
//    // 添加颜色偏移效果
//    vec3 colorShift = vec3(
//        baseNeon.r * (0.9 + 0.1 * sin(time * 1.2)),
//        baseNeon.g * (0.9 + 0.1 * sin(time * 1.5)),
//        baseNeon.b * (0.9 + 0.1 * sin(time * 1.8))
//    );
//    
//    // 发光效果
//    float brightness = length(colorShift);
//    vec3 glow = colorShift * brightness * 0.3;
//    
//    vec3 finalColor = colorShift + glow;
//    
//    // 输出最终颜色
//    FragColor = vec4(clamp(finalColor, 0.0, 1.0), 1.0);
//}
//)";
//
//// 处理输入
//void processInput(GLFWwindow* window) {
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}
//
//// 窗口大小调整回调
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//}
//
//// 编译着色器
//unsigned int compileShader(unsigned int type, const char* source) {
//    unsigned int shader = glCreateShader(type);
//    glShaderSource(shader, 1, &source, NULL);
//    glCompileShader(shader);
//
//    // 检查编译错误
//    int success;
//    char infoLog[512];
//    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        glGetShaderInfoLog(shader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//    return shader;
//}
//
//// 创建着色器程序
//unsigned int createShaderProgram() {
//    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
//    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
//
//    unsigned int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    // 检查链接错误
//    int success;
//    char infoLog[512];
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    return shaderProgram;
//}
//
//int main2() {
//    // 初始化GLFW
//    if (!glfwInit()) {
//        std::cout << "Failed to initialize GLFW" << std::endl;
//        return -1;
//    }
//
//    // 配置GLFW
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // 创建窗口
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "霓虹立方体 - GLFW + GLAD", NULL, NULL);    if (window == NULL) {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    // 初始化GLAD
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // 创建着色器程序
//    unsigned int shaderProgram = createShaderProgram();
//
//    // 立方体顶点数据 - 8个顶点，每个顶点有唯一的颜色
//    float vertices[] = {
//        // 位置              // 颜色 (每个角点唯一的颜色)
//        // 前左下角 - 顶点0
//        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  // 红色
//
//        // 前右下角 - 顶点1
//         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // 绿色
//
//         // 前右上角 - 顶点2
//          0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // 蓝色
//
//          // 前左上角 - 顶点3
//          -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // 黄色
//
//          // 后左下角 - 顶点4
//          -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  // 紫色
//
//          // 后右下角 - 顶点5
//           0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // 青色
//
//           // 后右上角 - 顶点6
//            0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 0.0f,  // 橙色
//
//            // 后左上角 - 顶点7
//            -0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 1.0f   // 紫蓝色
//    };
//
//    // 索引数据 - 使用8个顶点构建6个面
//    unsigned int indices[] = {
//        // 前面
//        0, 1, 2, 2, 3, 0,
//        // 后面
//        4, 5, 6, 6, 7, 4,
//        // 左面
//        7, 4, 0, 0, 3, 7,
//        // 右面
//        1, 5, 6, 6, 2, 1,
//        // 下面
//        4, 5, 1, 1, 0, 4,
//        // 上面
//        3, 2, 6, 6, 7, 3
//    };
//
//    // 创建VAO, VBO, EBO
//    unsigned int VAO, VBO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    // 绑定VAO
//    glBindVertexArray(VAO);
//
//    // 绑定VBO并复制顶点数据
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // 绑定EBO并复制索引数据
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    // 设置顶点属性指针
//    // 位置属性
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // 颜色属性
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    // 解绑
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    // 启用深度测试
//    glEnable(GL_DEPTH_TEST);
//
//    // 渲染循环
//        // 渲染循环
//    while (!glfwWindowShouldClose(window)) {
//        // 输入处理
//        processInput(window);
//
//        // 渲染
//        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);  // 更深的背景以突出霓虹效果
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // 使用着色器程序
//        glUseProgram(shaderProgram);
//
//        // 创建变换矩阵
//        float time = glfwGetTime();
//
//        // 模型矩阵（旋转）
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::rotate(model, time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
//
//        // 视图矩阵（相机位置）
//        glm::mat4 view = glm::mat4(1.0f);
//        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//
//        // 投影矩阵
//        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//
//        // 传递矩阵到着色器
//        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
//        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
//        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
//
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
//        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
//        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
//
//        // 传递时间到着色器 - 新增这部分
//        unsigned int timeLoc = glGetUniformLocation(shaderProgram, "time");
//        glUniform1f(timeLoc, time);
//
//        // 渲染立方体
//        glBindVertexArray(VAO);
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//        // 交换缓冲区和轮询事件
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//    // 清理资源
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//    glDeleteProgram(shaderProgram);
//
//    glfwTerminate();
//    return 0;
//}