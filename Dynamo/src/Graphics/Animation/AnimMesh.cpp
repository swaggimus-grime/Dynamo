#include "dynamopch.h"
#include "AnimMesh.h"

#include "AnimModel.h"
#include "Entities/Light.h"
#include "Entities/ObjectCBuffs.h"
#include "Bindable/Sampler.h"
#include "Bindable/Shader.h"
#include "Bindable/InputLayout.h"
#include "Bindable/TransformBuffer.h"
#include "AssimpXMHelpers.h"
#include "Bone.h"

AnimMesh::AnimMesh(Graphics& g, AnimModel* parent, const std::string& directory, const aiMesh* mesh, const aiMaterial* mat)
    :m_Parent(parent), m_Directory(std::move(directory))
{
    m_Layout = MakeUnique<VertexLayout>(ATTRIB_POS | ATTRIB_TEX | ATTRIB_NORM | ATTRIB_TAN | ATTRIB_BITAN | ATTRIB_BONES | ATTRIB_WEIGHTS);
    VertexData vertices(*m_Layout, mesh->mNumVertices);
    for (UINT vert = 0; vert < mesh->mNumVertices; vert++) {
        auto& pos = *reinterpret_cast<XMFLOAT3*>(&mesh->mVertices[vert].x);
        vertices.Pos(vert) = pos;
        vertices.Norm(vert) = *reinterpret_cast<XMFLOAT3*>(&mesh->mNormals[vert].x);
        vertices.Tex(vert) = *reinterpret_cast<XMFLOAT2*>(&mesh->mTextureCoords[0][vert].x);
        vertices.Tan(vert) = *reinterpret_cast<XMFLOAT3*>(&mesh->mTangents[vert].x);
        vertices.Bitan(vert) = *reinterpret_cast<XMFLOAT3*>(&mesh->mBitangents[vert].x);
        vertices.Bones(vert) = std::move(XMINT4(-1, -1, -1, -1));
        vertices.Weights(vert) = std::move(XMFLOAT4());
    }

    std::vector<UINT> indices;
    for (UINT faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
    {
        const aiFace& face = mesh->mFaces[faceIndex];
        for (UINT indIndex = 0; indIndex < face.mNumIndices; indIndex++)
            indices.push_back(face.mIndices[indIndex]);
    }

    auto& bones = parent->Bones();
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (bones.find(boneName) == bones.end())
        {
            BoneInfo bone;
            bone.id = parent->BoneCounter();
            bone.offset = XMMatrixTranspose(aiToXMMat4(mesh->mBones[boneIndex]->mOffsetMatrix));
            bones[boneName] = bone;
            boneID = parent->BoneCounter();
            parent->BoneCounter()++;
        }
        else
            boneID = bones[boneName].id;

        DYNAMO_ASSERT(boneID != -1, "BONER ALERT");
        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            DYNAMO_ASSERT(vertexId <= vertices.Size(), "BONER ALERT");

            auto& bones = vertices.Bones(vertexId);
            auto& weights = vertices.Weights(vertexId);
            for (SIZE_T i = 0; i < sizeof(XMINT4) / sizeof(INT); i++)
            {
                if (*(&bones.x + i) < 0)
                {
                    *(&weights.x + i) = weight;
                    *(&bones.x + i) = boneID;
                    break;
                }
            }
        }
    }

    m_VBuff = MakeUnique<VertexBuffer>(g, vertices);
    m_IBuff = MakeUnique<IndexBuffer>(g, indices);
    m_Top = Topology::Evaluate(g);
    auto transform = MakeShared<TransformBuffer>(g);

    if (mesh->mMaterialIndex >= 0) {
        if (Shared<Texture2D>& diffuse = GetTexture(g, mat, aiTextureType_DIFFUSE, 0))
            m_Textures.push_back(std::move(diffuse));
        if (Shared<Texture2D>& specular = GetTexture(g, mat, aiTextureType_METALNESS, 1))
            m_Textures.push_back(std::move(specular));
        if (Shared<Texture2D>& normal = GetTexture(g, mat, aiTextureType_NORMALS, 2))
            m_Textures.push_back(std::move(normal));
    }

    auto boneBuff = MakeShared<BoneBuffer>(g);
    boneBuff->SetModel(m_Parent);
    
    {
        Technique lambertian("Shade");
        {
            Step only("lambertian");
            auto& vs = VertexShader::Evaluate(g, "res\\shaders\\AnimModelvs.hlsl");
            only.AddBind(InputLayout::Evaluate(g, *m_Layout, *vs));
            only.AddBind(vs);
            only.AddBind(PixelShader::Evaluate(g, "res\\shaders\\Modelps.hlsl"));
            auto camBuff = PixelConstantBuffer<CamStuff>::Evaluate(g, 2);
            only.AddBind(std::move(camBuff));
            auto specular = PixelConstantBuffer<SpecularMaterial>::Evaluate(g);
            SpecularMaterial spec;
            spec.SpecPower = 4.f;
            spec.SpecColor = { 0.1f, 0.1f, 0.1f, 1.f };
            spec.hasSpecMap = true;
            specular->Update(g, &spec);
            only.AddBind(std::move(specular));
            only.AddBind(transform);
            only.AddBind(boneBuff);
            for (auto& t : m_Textures)
                only.AddBind(t);
            lambertian.AddStep(std::move(only));
        }

        AddTechnique(std::move(lambertian));
    }
    {
        Technique shadowMap("shadowMap");
        {
            Step only("shadowMap");
            auto& vs = VertexShader::Evaluate(g, "res\\shaders\\AnimSolidvs.hlsl");
            only.AddBind(vs);
            only.AddBind(InputLayout::Evaluate(g, *m_Layout, *vs));
            only.AddBind(transform);
            only.AddBind(boneBuff);
            shadowMap.AddStep(std::move(only));
        }

        AddTechnique(std::move(shadowMap));
    }
}

XMMATRIX AnimMesh::ModelMat() const
{
    return m_Parent->ModelMat();
}

void AnimMesh::Bind(Graphics& g) const
{
    Renderable::Bind(g);
}

Shared<Texture2D> AnimMesh::GetTexture(Graphics& g, const aiMaterial* mat, aiTextureType type, UINT slot)
{
    aiString texName;
    mat->GetTexture(type, 0, &texName);
    if (texName.length == NULL)
        return nullptr;
    std::string texPath = std::string(texName.C_Str());
    texPath = m_Directory + "/" + texPath;
    return Texture2D::Evaluate(g, texPath, slot);
}
