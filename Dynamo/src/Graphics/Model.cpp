#include "dynamopch.h"
#include "Model.h"
#include <imgui.h>

static std::string directory = "";

Model::Model(Graphics& g, const std::string& path, const Transform& t)
    :Transformable(t),
    m_Shader(std::move(std::make_shared<Shader>(g, L"res\\shaders\\Modelvs.cso", L"res\\shaders\\Modelps.cso")))
{
    m_TransformCBuff = std::make_unique<ModelTransformBuffer>(g);
    Material mat = { .6f, 30.f };
    m_MatCBuff = std::make_unique<MaterialBuffer>(g, mat);
    m_Samp = std::make_unique<Sampler>(g, SAMPLER_MODE::ANISO_WRAP);
    m_Blend = std::make_unique<BlendState>(g);
    Reload(g, path);
}

Model::~Model()
{

}

void Model::Reload(Graphics& g, const std::string& path)
{
    m_Meshes.clear();
    m_TotalVertices = 0;
    m_TotalIndices = 0;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw MODEL_EXCEP(importer.GetErrorString());

    directory = path.substr(0, path.find_last_of('\\'));
    VertexLayout layout;
    layout.AddAttrib("Pos", DXGI_FORMAT_R32G32B32_FLOAT);
    layout.AddAttrib("Tex", DXGI_FORMAT_R32G32_FLOAT);
    layout.AddAttrib("Norm", DXGI_FORMAT_R32G32B32_FLOAT);
    layout.AddAttrib("Tan", DXGI_FORMAT_R32G32B32_FLOAT);
    layout.AddAttrib("Bitan", DXGI_FORMAT_R32G32B32_FLOAT);
    for (UINT meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
        const aiMesh* mesh = scene->mMeshes[meshIndex];
        std::vector<Vertex> vertices;
        std::vector<UINT> indices;

        m_TotalVertices += mesh->mNumVertices;
        for (UINT vert = 0; vert < mesh->mNumVertices; vert++) {
            Vertex vertex = {};
            vertex.pos = *reinterpret_cast<XMFLOAT3*>(&mesh->mVertices[vert].x);
            vertex.norm = *reinterpret_cast<XMFLOAT3*>(&mesh->mNormals[vert].x);
            vertex.tex = *reinterpret_cast<XMFLOAT2*>(&mesh->mTextureCoords[0][vert].x);
            vertex.tan = *reinterpret_cast<XMFLOAT3*>(&mesh->mTangents[vert].x);
            vertex.bitan = *reinterpret_cast<XMFLOAT3*>(&mesh->mBitangents[vert].x);

            vertices.push_back(vertex);
        }

        for (UINT faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
        {
            const aiFace& face = mesh->mFaces[faceIndex];
            m_TotalIndices += face.mNumIndices;
            for (UINT indIndex = 0; indIndex < face.mNumIndices; indIndex++)
                indices.push_back(face.mIndices[indIndex]);
        }

        Mesh<Vertex> m = { g, vertices, m_Shader->GetVSCode(), layout, indices };
        if (mesh->mMaterialIndex >= 0) {
            const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            if (std::shared_ptr<Texture2D> diffuse = GetTexture(g, material, aiTextureType_DIFFUSE, 0))
                m.AddTexture(std::move(diffuse));
            if (std::shared_ptr<Texture2D> specular = GetTexture(g, material, aiTextureType_SPECULAR, 1))
                m.AddTexture(std::move(specular));
            if (std::shared_ptr<Texture2D> normal = GetTexture(g, material, aiTextureType_HEIGHT, 2))
                m.AddTexture(std::move(normal));
        }

        m_Meshes.push_back(std::move(m));
    }
}

void Model::Render(Graphics& g)
{
    m_Shader->Bind(g);
    m_TransformCBuff->SetModel(GetModelMat());
    m_TransformCBuff->Bind(g);
    m_MatCBuff->Bind(g);
    m_Samp->Bind(g);
    m_Blend->Bind(g);
    for (auto& m : m_Meshes)
        m.Render(g);
}

void Model::ShowGUI(Graphics& g)
{
    ImGui::Text("Vertices: %d", m_TotalVertices);
    ImGui::Text("Indices: %d", m_TotalIndices);
    Transformable::ShowGUI(g);
}

std::shared_ptr<Texture2D> Model::GetTexture(Graphics& g, const aiMaterial* mat, aiTextureType type, UINT slot)
{
    aiString texName;
    mat->GetTexture(type, 0, &texName);
    if (texName.length == NULL)
        return nullptr;
    std::string texPath = std::string(texName.C_Str());
    texPath = directory + "\\" + texPath;
    wchar_t wbuff[1000];
    mbstowcs(wbuff, texPath.c_str(), sizeof(wbuff));
    return std::move(std::make_shared<Texture2D>(g, wbuff, slot));
}

Model::ModelException::ModelException(const char* file, unsigned int line, const char* msg)
	:DynamoException(file, line)
{
	std::stringstream s;
	s << __super::what() << std::endl << msg;
	m_What = s.str();
}

const char* Model::ModelException::GetType() const
{
	return "Model Exception";
}

const char* Model::ModelException::what() const
{
	return m_What.c_str();
}

Model::ModelTransformBuffer::ModelTransformBuffer(Graphics& g)
    :ConstantBuffer(g, SHADER_TYPE::VS, sizeof(m_Transform))
{
}

void Model::ModelTransformBuffer::Bind(Graphics& g)
{
    m_Transform.Model = XMMatrixTranspose(m_Model);
    m_Transform.ModelView = m_Model * g.LookAt();
    m_Transform.MVP = XMMatrixTranspose(m_Transform.ModelView * g.Projection());
    m_Transform.ModelView = XMMatrixTranspose(m_Transform.ModelView);
    Update(g, sizeof(m_Transform), &m_Transform);
    ConstantBuffer::Bind(g);
}

Model::MaterialBuffer::MaterialBuffer(Graphics& g, const Material& mat)
    :ConstantBuffer(g, SHADER_TYPE::PS, sizeof(mat), 0, &mat)
{
}
