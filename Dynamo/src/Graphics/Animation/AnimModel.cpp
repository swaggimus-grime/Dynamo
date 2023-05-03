#include "dynamopch.h"
#include "AnimModel.h"

#include "Graphics.h"
#include "RDG/RDG.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

AnimModel::AnimModel(Graphics& g, const std::string& path, bool leftHanded)
{
    Assimp::Importer importer;

    auto steps = aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_FlipUVs |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace |
        (leftHanded ? aiProcess_ConvertToLeftHanded : 0);

    const aiScene* scene = importer.ReadFile(path, steps);
    DYNAMO_ASSERT(scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode,
        importer.GetErrorString());

    for (UINT meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
        const aiMesh* mesh = scene->mMeshes[meshIndex];
        const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        m_Meshes.push_back(MakeUnique<AnimMesh>(g, this, path.substr(0, path.find_last_of('/')), mesh, mat));
    }

    const aiScene* animScene = importer.ReadFile(path, aiProcess_Triangulate);
    m_Animation = std::make_unique<Animation>(*animScene, this);
    m_Animator = std::make_unique<Animator>(m_Animation.get());
}

void AnimModel::Submit()
{
    for (auto& m : m_Meshes)
        m->Submit();
}

void AnimModel::LinkToRDG(RDG& graph)
{
    for (auto& m : m_Meshes)
        m->LinkToRDG(graph);
}

void AnimModel::Update(float dt)
{
    m_Animator->UpdateAnimation(dt);
}

void AnimModel::ShowGUI(Graphics& g)
{
    Transformable::ShowGUI(g);
}

XMMATRIX AnimModel::ModelMat() const
{
    return m_Scale * m_Rot * m_Trans;
}
