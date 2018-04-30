#include "modelLoader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


bool ModelLoader::loadModels(const String& fileName,
			Array<IndexedModel>& models, Array<uint32>& modelMaterialIndices,
			Array<MaterialSpec>& materials)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName.c_str(), 
											 aiProcess_Triangulate |
											 aiProcess_GenSmoothNormals | 
											 aiProcess_FlipUVs |
											 aiProcess_CalcTangentSpace);
	
	if(!scene) {
		DEBUG_LOG(LOG_TYPE_IO, LOG_ERROR,  "Mesh load failed!: %s",
				fileName.c_str());
		return false;
	}
	
	for(uint32 j = 0; j < scene->mNumMeshes; j++) {
		const aiMesh* model = scene->mMeshes[j];
		modelMaterialIndices.push_back(model->mMaterialIndex);

		IndexedModel newModel;
		newModel.allocateElement(3); // Positions
		newModel.allocateElement(2); // TexCoords
		newModel.allocateElement(3); // Normals
		newModel.allocateElement(3); // Tangents
		newModel.setInstancedElementStartIndex(4); // Begin instanced data
		newModel.allocateElement(16); // Transform matrix

		const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
		for(uint32 i = 0; i < model->mNumVertices; i++) {
			const aiVector3D pos = model->mVertices[i];
			const aiVector3D normal = model->mNormals[i];
			const aiVector3D texCoord = model->HasTextureCoords(0)
				? model->mTextureCoords[0][i] : aiZeroVector;
			const aiVector3D tangent = model->mTangents[i];

			newModel.addElement3f(0, pos.x, pos.y, pos.z);
			newModel.addElement2f(1, texCoord.x, texCoord.y);
			newModel.addElement3f(2, normal.x, normal.y, normal.z);
			newModel.addElement3f(3, tangent.x, tangent.y, tangent.z);
		}
		for(uint32 i = 0; i < model->mNumFaces; i++)
		{
			const aiFace& face = model->mFaces[i];
			assert(face.mNumIndices == 3);
			newModel.addIndices3i(face.mIndices[0], face.mIndices[1],
					face.mIndices[2]);
		}

		models.push_back(newModel);
	}

	for(uint32 i = 0; i < scene->mNumMaterials; i++) {
		const aiMaterial* material = scene->mMaterials[i];
		MaterialSpec spec;
		
		// Currently only handles diffuse textures.
		aiString texturePath;
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
				material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath)
				!= AI_SUCCESS) {
			String str(texturePath.data);
			spec.textureNames["diffuse"] = str;
		}
		materials.push_back(spec);
	}
	
	return true;
}
