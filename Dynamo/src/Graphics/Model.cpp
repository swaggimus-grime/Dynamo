#include "dynamopch.h"
#include "Model.h"

static std::string directory = "";

Model::Model(Graphics& g, const std::string& path, std::shared_ptr<Shader> shader)
    :m_Shader(shader)
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

        std::vector<std::shared_ptr<Texture2D>> textures;
        if (mesh->mMaterialIndex >= 0) {
            const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            if (std::shared_ptr<Texture2D> diffuse = GetTexture(g, material, aiTextureType_DIFFUSE, 0))
                textures.push_back(diffuse);
            if (std::shared_ptr<Texture2D> specular = GetTexture(g, material, aiTextureType_SPECULAR, 1))
                textures.push_back(specular);
            if (std::shared_ptr<Texture2D> normal = GetTexture(g, material, aiTextureType_HEIGHT, 2))
                textures.push_back(normal);
        }

        std::shared_ptr<VertexBuffer<Vertex>> vbuff = std::make_shared<VertexBuffer<Vertex>>(g, vertices, shader->GetVSCode(), layout);
        std::shared_ptr<IndexBuffer> ibuff = std::make_shared<IndexBuffer>(g, indices);
        Mesh<Vertex> m = { vbuff, ibuff, textures };
        m_Meshes.push_back(m);
    }
}

Model::~Model()
{

}

void Model::Render(Graphics& g) const
{
    m_Shader->Bind(g);
    for (const auto& m : m_Meshes) {
        m.VBuff->Bind(g);
        m.IBuff->Bind(g);
        for (const auto& t : m.Textures)
            t->Bind(g);

        g.DC().DrawIndexed(m.IBuff->Size(), 0, 0);
    }
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

