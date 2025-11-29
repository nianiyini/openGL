#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// 在include部分之后，main函数之前添加LightingSystem类
class LightingSystem {
private:
    unsigned int lightingUBO;

public:
    // 使用标准布局确保C++和GLSL内存布局一致
    struct PointLight {
        glm::vec3 position;    // 偏移 0
        float padding0;        // 偏移 12 (对齐到16字节)
        glm::vec3 diffuse;     // 偏移 16
        float padding1;        // 偏移 28
        glm::vec3 specular;    // 偏移 32
        float padding2;        // 偏移 44
        float constant;        // 偏移 48
        float linear;          // 偏移 52
        float quadratic;       // 偏移 56
        // 总共64字节
    };

    struct DirLight {
        glm::vec3 direction;   // 偏移 0
        float padding0;        // 偏移 12
        glm::vec3 diffuse;     // 偏移 16
        float padding1;        // 偏移 28
        glm::vec3 specular;    // 偏移 32
        float padding2;        // 偏移 44
        // 总共48字节
    };

    struct SpotLight {
        glm::vec3 position;    // 偏移 0
        float padding0;        // 偏移 12
        glm::vec3 direction;   // 偏移 16
        float padding1;        // 偏移 28
        glm::vec3 diffuse;     // 偏移 32
        float padding2;        // 偏移 44
        glm::vec3 specular;    // 偏移 48
        float padding3;        // 偏移 60
        float cutOff;          // 偏移 64
        float outerCutOff;     // 偏移 68
        float constant;        // 偏移 72
        float linear;          // 偏移 76
        float quadratic;       // 偏移 80
        // 总共84字节，对齐到96字节
    };

    struct LightingData {
        PointLight pointLights[4];        // 4 * 64 = 256字节
        DirLight dirLight;                // 48字节 (偏移256)
        SpotLight spotLights[2];          // 2 * 96 = 192字节 (偏移304)
        glm::vec3 ambientColor;           // 偏移496
        float padding0;                   // 偏移508
        int numPointLights;               // 偏移512
        int numSpotLights;                // 偏移516
        // 总共520字节
    };

    LightingData data;

    void init() {
        glGenBuffers(1, &lightingUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, lightingUBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(LightingData), NULL, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightingUBO); // 绑定到位置1
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // 初始化默认值
        resetToDefault();
    }

    void update() {
        glBindBuffer(GL_UNIFORM_BUFFER, lightingUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightingData), &data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void resetToDefault() {
        // 重置所有光源为默认值
        data.ambientColor = glm::vec3(0.1f);
        data.numPointLights = 0;
        data.numSpotLights = 0;

        // 初始化点光源
        for (int i = 0; i < 4; ++i) {
            data.pointLights[i].position = glm::vec3(0.0f);
            data.pointLights[i].diffuse = glm::vec3(1.0f);
            data.pointLights[i].specular = glm::vec3(1.0f);
            data.pointLights[i].constant = 1.0f;
            data.pointLights[i].linear = 0.09f;
            data.pointLights[i].quadratic = 0.032f;
        }

        // 初始化方向光
        data.dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
        data.dirLight.diffuse = glm::vec3(0.5f);
        data.dirLight.specular = glm::vec3(1.0f);

        // 初始化聚光灯
        for (int i = 0; i < 2; ++i) {
            data.spotLights[i].position = glm::vec3(0.0f);
            data.spotLights[i].direction = glm::vec3(0.0f, 0.0f, -1.0f);
            data.spotLights[i].diffuse = glm::vec3(1.0f);
            data.spotLights[i].specular = glm::vec3(1.0f);
            data.spotLights[i].cutOff = glm::cos(glm::radians(12.5f));
            data.spotLights[i].outerCutOff = glm::cos(glm::radians(17.5f));
            data.spotLights[i].constant = 1.0f;
            data.spotLights[i].linear = 0.09f;
            data.spotLights[i].quadratic = 0.032f;
        }

        update();
    }

    // 添加点光源的便捷方法
    void addPointLight(const glm::vec3& position, const glm::vec3& diffuse = glm::vec3(1.0f),
        const glm::vec3& specular = glm::vec3(1.0f)) {
        if (data.numPointLights < 4) {
            int index = data.numPointLights++;
            data.pointLights[index].position = position;
            data.pointLights[index].diffuse = diffuse;
            data.pointLights[index].specular = specular;
            update();
        }
    }

    // 设置方向光
    void setDirectionalLight(const glm::vec3& direction, const glm::vec3& diffuse = glm::vec3(0.5f),
        const glm::vec3& specular = glm::vec3(1.0f)) {
        data.dirLight.direction = glm::normalize(direction);
        data.dirLight.diffuse = diffuse;
        data.dirLight.specular = specular;
        update();
    }

    // 设置环境光
    void setAmbientLight(const glm::vec3& ambient) {
        data.ambientColor = ambient;
        update();
    }

    ~LightingSystem() {
        if (lightingUBO != 0) {
            glDeleteBuffers(1, &lightingUBO);
        }
    }
};