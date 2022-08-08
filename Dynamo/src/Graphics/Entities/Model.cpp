#include "dynamopch.h"
#include "Model.h"

#include "Graphics.h"
#include "RDG/RDG.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(Graphics& g, const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ConvertToLeftHanded |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace);
    DYNAMO_ASSERT(scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode,
        importer.GetErrorString());
    
    for (UINT meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
        const aiMesh* mesh = scene->mMeshes[meshIndex];
        const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        m_Meshes.push_back(MakeUnique<Mesh>(g, path.substr(0, path.find_last_of('/')), mesh, mat));
    }

}

void Model::Submit()
{
    for (auto& m : m_Meshes)
        m->Submit();
}

void Model::LinkToRDG(RDG& graph)
{
    for (auto& m : m_Meshes)
        m->LinkToRDG(graph);
}

void Model::SetPos(const XMFLOAT3& p)
{
    for (auto& m : m_Meshes)
        m->SetPos(p);
}

void Model::SetRot(const XMFLOAT3& r)
{
    for (auto& m : m_Meshes)
        m->SetRot(r);
}

void Model::SetScale(const XMFLOAT3& s)
{
    for (auto& m : m_Meshes)
        m->SetScale(s);
}
