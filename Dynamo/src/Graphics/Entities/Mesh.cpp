#include "dynamopch.h"
#include "Mesh.h"

#include "Model.h"
#include "Bindable/Sampler.h"
#include "Bindable/Shader.h"
#include "Bindable/InputLayout.h"
#include "Bindable/TransformBuffer.h"
#include "Light.h"

VertexLayout Mesh::m_Layout = VertexLayout(ATTRIB_POS | ATTRIB_TEX | ATTRIB_NORM | ATTRIB_TAN | ATTRIB_BITAN);

struct Specular {
    alignas(8) float Intensity;
    alignas(8) float Power;
} spec;

Mesh::Mesh(Graphics& g, Model* parent, const std::string& directory, const aiMesh* mesh, const aiMaterial* mat)
    :m_Parent(parent), m_Directory(std::move(directory))
{
    VertexData vertices(m_Layout, mesh->mNumVertices);
    for (UINT vert = 0; vert < mesh->mNumVertices; vert++) {
        vertices.Pos(vert) = *reinterpret_cast<XMFLOAT3*>(&mesh->mVertices[vert].x);
        vertices.Norm(vert) = *reinterpret_cast<XMFLOAT3*>(&mesh->mNormals[vert].x);
        vertices.Tex(vert) = *reinterpret_cast<XMFLOAT2*>(&mesh->mTextureCoords[0][vert].x);
        vertices.Tan(vert) = *reinterpret_cast<XMFLOAT3*>(&mesh->mTangents[vert].x);
        vertices.Bitan(vert) = *reinterpret_cast<XMFLOAT3*>(&mesh->mBitangents[vert].x);
    }

    std::vector<UINT> indices;
    for (UINT faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
    {
        const aiFace& face = mesh->mFaces[faceIndex];
        for (UINT indIndex = 0; indIndex < face.mNumIndices; indIndex++)
            indices.push_back(face.mIndices[indIndex]);
    }

    m_VBuff = MakeUnique<VertexBuffer>(g, vertices);
    m_IBuff = MakeUnique<IndexBuffer>(g, indices);
    m_Top = Topology::Evaluate(g);

    if (mesh->mMaterialIndex >= 0) {
        if (Shared<Texture2D>& diffuse = GetTexture(g, mat, aiTextureType_DIFFUSE, 0))
            m_Textures.push_back(std::move(diffuse));
        if (Shared<Texture2D>& specular = GetTexture(g, mat, aiTextureType_SPECULAR, 1))
            m_Textures.push_back(std::move(specular));
        if (Shared<Texture2D>& normal = GetTexture(g, mat, aiTextureType_HEIGHT, 2))
            m_Textures.push_back(std::move(normal));
    }

    {
        Technique lambertian("Shade");
        {
            Step only("lambertian");
            only.AddBind(Sampler::Evaluate(g));
            auto& vs = VertexShader::Evaluate(g, "res\\shaders\\Modelvs.cso");
            only.AddBind(InputLayout::Evaluate(g, m_Layout, *vs));
            only.AddBind(vs);
            only.AddBind(PixelShader::Evaluate(g, "res\\shaders\\Modelps.cso"));
            auto specular = PixelConstantBuffer<Specular>::Evaluate(g);
            spec.Intensity = 0.1;
            spec.Power = 16;
            specular->Update(g, &spec);
            only.AddBind(std::move(specular));
            only.AddBind(MakeShared<TransformBuffer>(g));
            for (auto& t : m_Textures)
               only.AddBind(t);
            lambertian.AddStep(only);
        }

        AddTechnique(lambertian);
    }
}

XMMATRIX Mesh::ModelMat() const
{
    return m_Parent->ModelMat();
}

Shared<Texture2D> Mesh::GetTexture(Graphics& g, const aiMaterial* mat, aiTextureType type, UINT slot)
{
    aiString texName;
    mat->GetTexture(type, 0, &texName);
    if (texName.length == NULL)
        return nullptr;
    std::string texPath = std::string(texName.C_Str());
    texPath = m_Directory + "/" + texPath;
    return Texture2D::Evaluate(g, texPath, slot);
}

