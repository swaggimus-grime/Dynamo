#include "dynamopch.h"
#include "Model.h"

static std::string directory = "";

Model::Model(Graphics& g, const std::string& path)
    :m_Shader(std::move(std::make_shared<Shader>(g, L"res/shaders/Modelvs.cso", L"res/shaders/Modelps.cso")))
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	    throw MODEL_EXCEP(importer.GetErrorString());

    directory = path.substr(0, path.find_last_of('/'));
    VertexLayout layout;
    layout.AddAttrib("Pos", DXGI_FORMAT_R32G32B32_FLOAT);
    layout.AddAttrib("Norm", DXGI_FORMAT_R32G32B32_FLOAT);
    layout.AddAttrib("Tex", DXGI_FORMAT_R32G32_FLOAT);
    layout.AddAttrib("Tan", DXGI_FORMAT_R32G32B32_FLOAT);
    layout.AddAttrib("Bitan", DXGI_FORMAT_R32G32B32_FLOAT);
    for (UINT meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
        const aiMesh* mesh = scene->mMeshes[meshIndex];
        std::vector<Vertex> vertices;
        std::vector<UINT> indices;

        for (UINT vert = 0; vert < mesh->mNumVertices; vert++) {
            Vertex vertex = {};
            vertex.pos.x = mesh->mVertices[vert].x;
            vertex.pos.y = mesh->mVertices[vert].y;
            vertex.pos.z = mesh->mVertices[vert].z;

            if (mesh->HasNormals()) {
                vertex.norm.x = mesh->mNormals[vert].x;
                vertex.norm.y = mesh->mNormals[vert].y;
                vertex.norm.z = mesh->mNormals[vert].z;
            }

            if (mesh->mTextureCoords[0]) {
                vertex.tex.x = mesh->mTextureCoords[0][vert].x;
                vertex.tex.y = mesh->mTextureCoords[0][vert].y;

                if (mesh->HasTangentsAndBitangents()) {
                    vertex.tan.x = mesh->mTangents[vert].x;
                    vertex.tan.y = mesh->mTangents[vert].y;
                    vertex.tan.z = mesh->mTangents[vert].z;

                    vertex.bitan.x = mesh->mBitangents[vert].x;
                    vertex.bitan.y = mesh->mBitangents[vert].y;
                    vertex.bitan.z = mesh->mBitangents[vert].z;
                }
            }

            vertices.push_back(vertex);
        }

        for (UINT faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
        {
            const aiFace& face = mesh->mFaces[faceIndex];
            for (UINT indIndex = 0; indIndex < face.mNumIndices; indIndex++)
                indices.push_back(face.mIndices[indIndex]);
        }

        std::shared_ptr<VertexBuffer<Vertex>> vbuff = std::make_shared<VertexBuffer<Vertex>>(g, vertices, m_Shader->GetVSCode(), layout);
        std::shared_ptr<IndexBuffer> ibuff = std::make_shared<IndexBuffer>(g, indices);
        Mesh<Vertex> m = { vbuff, ibuff };

        std::vector<std::shared_ptr<Texture2D>> textures;
        if (mesh->mMaterialIndex >= 0) {
            const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            if (std::shared_ptr<Texture2D> diffuse = GetTexture(g, material, aiTextureType_DIFFUSE, 0))
                m.AddTexture(diffuse);
            if (std::shared_ptr<Texture2D> specular = GetTexture(g, material, aiTextureType_SPECULAR, 1))
                m.AddTexture(specular);
            if (std::shared_ptr<Texture2D> normal = GetTexture(g, material, aiTextureType_HEIGHT, 2))
                m.AddTexture(normal);
        }

        m_Meshes.push_back(m);
    }

    m_TransformCBuff = std::make_unique<ModelConstantBuffer>(g, m_Transform, SHADER_TYPE::VS, sizeof(ModelTransform));
}

Model::~Model()
{

}

void Model::Render(Graphics& g) const
{
    m_Shader->Bind(g);
    m_TransformCBuff->Bind(g);
    for (const auto& m : m_Meshes)
        m.Render(g);
}

std::shared_ptr<Texture2D> Model::GetTexture(Graphics& g, const aiMaterial* mat, aiTextureType type, UINT slot)
{
    aiString texName;
    mat->GetTexture(type, 0, &texName);
    if (texName.length == NULL)
        return nullptr;
    std::string texPath = std::string(texName.C_Str());
    texPath = directory + '/' + texPath;
    wchar_t wbuff[1000];
    mbstowcs(wbuff, texPath.c_str(), sizeof(wbuff));
    return std::make_shared<Texture2D>(g, wbuff, slot);
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

Model::ModelConstantBuffer::ModelConstantBuffer(Graphics& g, ModelTransform& transformRef, SHADER_TYPE type, SIZE_T size)
    :ConstantBuffer(g, type, size), m_TransformRef(transformRef)
{
}

void Model::ModelConstantBuffer::Bind(Graphics& g) const
{
    m_TransformRef.ModelView = g.LookAt();
    m_TransformRef.MVP = XMMatrixTranspose(m_TransformRef.ModelView * g.Projection());
    m_TransformRef.ModelView = XMMatrixTranspose(m_TransformRef.ModelView);
    Update(g, sizeof(ModelTransform), &m_TransformRef);
    ConstantBuffer::Bind(g);
}
