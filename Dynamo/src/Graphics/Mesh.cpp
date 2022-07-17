#include "dynamopch.h"
#include "Mesh.h"

#include "Sampler.h"

VertexLayout Mesh::m_Layout = VertexLayout(ATTRIB_POS | ATTRIB_TEX | ATTRIB_NORM | ATTRIB_TAN | ATTRIB_BITAN);

Mesh::Mesh(Graphics& g, const std::string& directory, const aiMesh* mesh, const aiMaterial* mat)
    :m_Directory(std::move(directory))
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
    m_Top = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    if (mesh->mMaterialIndex >= 0) {
        if (Unique<Texture2D> diffuse = GetTexture(g, mat, aiTextureType_DIFFUSE, 0))
            m_Textures.push_back(std::move(diffuse));
        if (Unique<Texture2D> specular = GetTexture(g, mat, aiTextureType_SPECULAR, 1))
            m_Textures.push_back(std::move(specular));
        if (Unique<Texture2D> normal = GetTexture(g, mat, aiTextureType_HEIGHT, 2))
            m_Textures.push_back(std::move(normal));
    }

    {
        Technique lambertian;
        {
            Step only(0);
            only.AddBind(Texture2D::Evaluate(g, "res\\skyboxes\\ocean\\0.jpg", 0));
            only.AddBind(Sampler::Evaluate(g));
            auto& vs = VertexShader::Evaluate(g, "res\\shaders\\Solidvs.cso");
            only.AddBind(InputLayout::Evaluate(g, shape.Vertices, *vs));
            only.AddBind(vs);
            only.AddBind(PixelShader::Evaluate(g, "res\\shaders\\Solidps.cso"));
            only.AddBind(MakeShared<TransformBuffer>(g, *this));
        }
    }
}

Unique<Texture2D> Mesh::GetTexture(Graphics& g, const aiMaterial* mat, aiTextureType type, UINT slot)
{
    aiString texName;
    mat->GetTexture(type, 0, &texName);
    if (texName.length == NULL)
        return nullptr;
    std::string texPath = std::string(texName.C_Str());
    texPath = m_Directory + "\\" + texPath;
    return MakeUnique<Texture2D>(g, texPath, slot);
}

