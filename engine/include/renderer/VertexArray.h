#pragma once
#include "GL/glew.h"

class VertexArray {
   public:
    VertexArray(const float* verts, unsigned int numVerts,
                const unsigned int* indices, unsigned int numIndices);
    ~VertexArray();

    //このクラスに格納されている頂点配列オブジェクトをアクティブにする
    void SetActive();

    unsigned int GetNumIndices() const { return mNumIndices; }
    unsigned int GetNumVerts() const { return mNumVerts; }

   private:
    //頂点の数
    const unsigned int mNumVerts;
    //インデックスの数
    const unsigned int mNumIndices;
    //頂点バッファのOpenGL ID
    unsigned int mVertexBuffer;
    //インデックスバッファのOpenGL ID
    unsigned int mIndexBuffer;
    //頂点配列オブジェクトのOpenGL ID
    unsigned int mVertexArray;
};
