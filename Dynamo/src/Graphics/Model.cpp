#include "dynamopch.h"
#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    XMFLOAT3 pos;
    XMFLOAT3 norm;
    XMFLOAT2 tex;
    XMFLOAT3 tan;
    XMFLOAT3 bitan;
};

Model::Model(const std::string& path)
{
	//Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);
	//if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	//	throw MODEL_EXCEP(importer.GetErrorString());

	//std::string directory = path.substr(0, path.find_last_of('/'));

 //   for (UINT meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
 //       const aiMesh* mesh = scene->mMeshes[meshIndex];
 //       std::vector<Vertex> vertices;
 //       std::vector<UINT> indices;

 //       for (UINT vert = 0; vert < mesh->mNumVertices; vert++) {
 //           Vertex vertex = {};
 //           vertex.pos.x = mesh->mVertices[vert].x;
 //           vertex.pos.y = mesh->mVertices[vert].y;
 //           vertex.pos.z = mesh->mVertices[vert].z;

 //           if (mesh->HasNormals()) {
 //               vertex.norm.x = mesh->mNormals[vert].x;
 //               vertex.norm.y = mesh->mNormals[vert].y;
 //               vertex.norm.z = mesh->mNormals[vert].z;
 //           }

 //           if (mesh->mTextureCoords[0]) {
 //               vertex.tex.y = mesh->mTextureCoords[0][vert].x;
 //               vertex.tex.y = mesh->mTextureCoords[0][vert].y;

 //               if (mesh->HasTangentsAndBitangents()) {
 //                   vertex.tan.x = mesh->mTangents[vert].x;
 //                   vertex.tan.y = mesh->mTangents[vert].y;
 //                   vertex.tan.z = mesh->mTangents[vert].z;
 //                   
 //                   vertex.bitan.x = mesh->mBitangents[vert].x;
 //                   vertex.bitan.y = mesh->mBitangents[vert].y;
 //                   vertex.bitan.z = mesh->mBitangents[vert].z;
 //               }
 //           }

 //           vertices.push_back(vertex);
 //       }

 //       for (UINT faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
 //       {
 //           const aiFace& face = mesh->mFaces[faceIndex];
 //           for (UINT indIndex = 0; indIndex < face.mNumIndices; indIndex++)
 //               indices.push_back(face.mIndices[indIndex]);
 //       }

 //       /*std::vector<std::shared_ptr<Texture>> textures;
 //       if (mesh->mMaterialIndex >= 0) {
 //           const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

 //           if (std::shared_ptr<Texture> diffuse = GetTexture(material, aiTextureType_DIFFUSE, 0))
 //               textures.push_back(diffuse);
 //           if (std::shared_ptr<Texture> specular = GetTexture(material, aiTextureType_SPECULAR, 1))
 //               textures.push_back(specular);
 //           if (std::shared_ptr<Texture> normal = GetTexture(material, aiTextureType_HEIGHT, 2))
 //               textures.push_back(normal);
 //       }

 //       Meshes.push_back(std::make_shared<Mesh>(vertices, indices, textures, ShaderObject));*/
}

Model::~Model()
{

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
