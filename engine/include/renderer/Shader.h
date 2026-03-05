#pragma once
#include <string>

#include "GL/glew.h"
#include "core/Math.h"

class Shader {
   public:
    Shader();
    ~Shader();
    //頂点シェーダーファイルとフラグメントシェーダーファイルを読み込んで合わせる
    bool Load(const std::string& vertName, const std::string& fragName);
    void Unload();
    void SetActive();
    void SetMatrixUniform(const char* name, const Matrix4& matrix);
    void SetVectorUniform(const char* name, const Vector3& vector);
    void SetFloatUniform(const char* name, float value);

   private:
    //シェーダーファイルをコンパイルする
    bool CompileShader(const std::string& filename, GLenum shaderType,
                       GLuint& outShader);
    //正しくコンパイルできたかどうか
    bool IsCompiled(GLuint shader);
    //正しくシェーダープログラムを作成したかどうか
    bool IsValidProgram();
    //シェーダーファイルのOpenGLIDを格納
    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;
};
