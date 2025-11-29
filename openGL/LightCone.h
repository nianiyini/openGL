#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#pragma execution_character_set("utf-8")
#include "shader.h"
class LightCone {
private:
    unsigned int VAO, VBO;
    std::vector<float> vertices;

public:
    void createCone(float length = 10.0f, float baseRadius = 2.0f, int segments = 16) {
        vertices.clear();

        // 锥尖（起点）
        vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f });

        // 生成底面圆环
        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * 3.14159f * i / segments;
            float x = cos(angle) * baseRadius;
            float y = sin(angle) * baseRadius;

            // 底面顶点 - 在Z轴负方向，距离为length
            vertices.insert(vertices.end(), { x, y, -length });
        }


        // 创建缓冲区
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
            vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection,
        const glm::vec3& position, const glm::vec3& direction) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);

        shader.use();

        // 设置着色器uniform - 这行你漏掉了！
        shader.setVec3("lightPos", position);
        shader.setVec3("lightDirection", direction);
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 0.8f)); // 手电筒颜色

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // 计算锥体的变换矩阵
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);

        // 修正旋转计算
        glm::vec3 forward = glm::normalize(direction);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        // 如果方向与up平行，使用不同的up向量
        if (glm::abs(glm::dot(forward, up)) > 0.99f) {
            up = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        glm::vec3 right = glm::normalize(glm::cross(up, forward));
        up = glm::normalize(glm::cross(forward, right));

        // 构建旋转矩阵
        glm::mat4 rotation = glm::mat4(1.0f);
        rotation[0] = glm::vec4(right, 0.0f);
        rotation[1] = glm::vec4(up, 0.0f);
        rotation[2] = glm::vec4(-forward, 0.0f); // OpenGL是右手坐标系

        model = model * rotation;

        shader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 3);
        glBindVertexArray(0);

        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }
};

