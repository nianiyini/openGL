#include <sndfile.h>
#include <vector>
#include <al.h>
#include <alc.h>
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>

class Audio3DSystem {
private:
    ALCdevice* device;
    ALCcontext* context;

public:
    // 初始化OpenAL
    bool initialize() {
        device = alcOpenDevice(nullptr);
        if (!device) {
            std::cout << "Failed to open audio device" << std::endl;
            return false;
        }

        context = alcCreateContext(device, nullptr);
        if (!context) {
            std::cout << "Failed to create audio context" << std::endl;
            alcCloseDevice(device);
            return false;
        }

        alcMakeContextCurrent(context);
        std::cout << "OpenAL 3D Audio System Initialized" << std::endl;
        return true;
    }

    // 加载音频文件
    ALuint loadAudio(const std::string& filename) {
        SF_INFO sfInfo;
        SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &sfInfo);
        if (!file) {
            std::cout << "Failed to open audio file: " << filename << std::endl;
            return 0;
        }

        std::vector<short> pcmData(sfInfo.frames * sfInfo.channels);
        sf_read_short(file, pcmData.data(), pcmData.size());

        ALenum format;
        if (sfInfo.channels == 1) format = AL_FORMAT_MONO16;  // 3D音效推荐单声道
        else if (sfInfo.channels == 2) format = AL_FORMAT_STEREO16;
        else {
            sf_close(file);
            return 0;
        }

        ALuint buffer;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, pcmData.data(),
            pcmData.size() * sizeof(short), sfInfo.samplerate);

        sf_close(file);
        std::cout << "Loaded: " << filename << " (" << sfInfo.channels << " channels)" << std::endl;
        return buffer;
    }

    // 创建3D音频源
    ALuint create3DSource(ALuint buffer, const glm::vec3& position, bool looping = false) {
        ALuint source;
        alGenSources(1, &source);

        // 关联缓冲区
        alSourcei(source, AL_BUFFER, buffer);

        // 设置3D属性
        alSource3f(source, AL_POSITION, position.x, position.y, position.z);
        alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
        alSourcef(source, AL_PITCH, 1.0f);
        alSourcef(source, AL_GAIN, 1.0f);

        // 3D音效关键设置
        alSourcef(source, AL_REFERENCE_DISTANCE, 5.0f);   // 参考距离
        alSourcef(source, AL_MAX_DISTANCE, 50.0f);        // 最大可听距离
        alSourcef(source, AL_ROLLOFF_FACTOR, 1.0f);       // 衰减因子
        alSourcei(source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);

        // 启用3D空间化
        alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);  // 绝对坐标

        return source;
    }

    // 设置听者位置和朝向
    void setListener(const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up) {
        ALfloat listenerOri[6] = {
            forward.x, forward.y, forward.z,  // 前向量
            up.x, up.y, up.z                  // 上向量
        };

        alListener3f(AL_POSITION, position.x, position.y, position.z);
        alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
        alListenerfv(AL_ORIENTATION, listenerOri);
    }

    // 移动音频源
    void moveSource(ALuint source, const glm::vec3& newPosition) {
        alSource3f(source, AL_POSITION, newPosition.x, newPosition.y, newPosition.z);
    }

    // 播放音频
    void playSource(ALuint source) {
        alSourcePlay(source);
    }

    // 停止音频
    void stopSource(ALuint source) {
        alSourceStop(source);
    }

    // 设置多普勒效应（移动声源）
    void setDopplerEffect(float factor = 1.0f, float velocity = 1.0f) {
        alDopplerFactor(factor);      // 多普勒强度
        alDopplerVelocity(velocity);  // 声音速度
    }

    // 清理资源
    void cleanup() {
        alcDestroyContext(context);
        alcCloseDevice(device);
    }
};


