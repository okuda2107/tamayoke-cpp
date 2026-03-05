#pragma once
#include <AL/al.h>

#include <string>

#include "core/Math.h"

inline const char* alGetErrorString(ALenum error) {
    switch (error) {
        case AL_NO_ERROR:
            return "AL_NO_ERROR: No error";
        case AL_INVALID_NAME:
            return "AL_INVALID_NAME: Invalid name parameter";
        case AL_INVALID_ENUM:
            return "AL_INVALID_ENUM: Invalid enum parameter value";
        case AL_INVALID_VALUE:
            return "AL_INVALID_VALUE: Invalid parameter value";
        case AL_INVALID_OPERATION:
            return "AL_INVALID_OPERATION: Invalid operation";
        case AL_OUT_OF_MEMORY:
            return "AL_OUT_OF_MEMORY: Unable to allocate memory";
        default:
            return "Unknown AL error";
    }
}

/*
エンジンの座標系からOpenALの座標系に変換
+x: 前方, +y: 右, +z: 上
-> +x: 右, +y: 上, +z: 後方
*/
static constexpr float rate = 0.01;
inline Vector3 VecToOpenAL(const Vector3& in) {
    Vector3 out;
    out.x = in.y * rate;
    out.y = in.z * rate;
    out.z = -in.x * rate;
    return out;
}
