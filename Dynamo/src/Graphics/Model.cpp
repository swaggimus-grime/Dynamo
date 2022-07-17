#include "dynamopch.h"
#include "Model.h"

#include "Graphics.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

Model::Model(Graphics& g, const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace);
    DYNAMO_ASSERT(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode,
        importer.GetErrorString());
    
    for (UINT meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
        const aiMesh* mesh = scene->mMeshes[meshIndex];
        const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        Mesh m(g, path.substr(0, path.find_last_of('\\')), mesh, mat);
        m_Meshes.push_back(std::move(m));
    }
}