#include "FBXLoader.h"
#include "FBXModel.h"

using namespace MCB;
using namespace Assimp;
const std::string AssimpLoader::baseDirectory = "Resources\\";

void AssimpLoader::DeleteInstance()
{
	
	GetInstance()->Finalize();
	delete GetInstance();
}

AssimpLoader* AssimpLoader::GetInstance()
{
	static AssimpLoader* instance = new AssimpLoader;
	return instance;
}

AssimpLoader* AssimpLoader::GetInitInstance()
{
	static AssimpLoader* instance = GetInstance();
	instance->Initialize();
	return instance;
}

void AssimpLoader::Initialize()
{
;


}

void MCB::AssimpLoader::Finalize()
{

}


bool MCB::AssimpLoader::DoTheImportThing(ImportSetting importSetting) {
	// Create an instance of the Importer class
	Assimp::Importer importer;
	auto meshes = importSetting.meshies;
	//importer.SetIOHandler(new MyIOSystem());
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(importSetting.fileName,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_MakeLeftHanded |
		aiProcess_FlipUVs);

	// If the import failed, report it
	if (nullptr != scene) {
		//DoTheErrorLogging(importer.GetErrorString());
		return false;
	}



	// Now we can access the file's contents.
	//DoTheSceneProcessing(scene);

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}
void MCB::AssimpLoader::CopyNodesWithMeshes( aiNode ainode, Node* targetParent,aiScene* scene)
{
	Node* parent;
	//Matrix4x4 transform;



	// if node has meshes, create a new scene object for it
	if (ainode.mNumMeshes > 0) {
		std::unique_ptr<Node> newObject = std::make_unique<Node>();

		/*targetParent.addChild(newObject);*/
		targetParent->parent = newObject.get();
		newObject->name = ainode.mName.C_Str();
		// copy the meshes
		//CopyMeshes(node, newObject);
		for (int i = 0; i < ainode.mNumMeshes; i++)
		{

			newObject->meshes.push_back(processMesh(scene->mMeshes[ainode.mMeshes[i]], scene));
			
		}
		// the new object is the parent for all child nodes
		nodes.push_back(std::move(newObject));
		parent = nodes.end()->get();
		//transform.SetUnity();
	}
	else {
		// if no meshes, skip the node, but keep its transformation
		parent = targetParent;
		//transform = node.mTransformation * accTransform;
	}

	//// continue for all child nodes
	for (int i = 0; i < ainode.mNumChildren; i++) {
		CopyNodesWithMeshes(*ainode.mChildren[i], parent,scene);
	}
}


FBXModel AssimpLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
	// Data to fill
	FBXModel tempmodel;

	// Walk through each of the mesh's vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		FBXVertex vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;
		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;
		if (mesh->mTextureCoords[0]) {
			vertex.uv.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.uv.y = (float)mesh->mTextureCoords[0][i].y;
		}

		tempmodel.vertices.push_back(vertex);
	}


	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			tempmodel.indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) {
		aiColor3D color;
		ObjectMaterial mat;
		aiString name;

		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE,color);
		mat.material.diffuse.x = color.r;
		mat.material.diffuse.y = color.g;
		mat.material.diffuse.z = color.b;

		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_SPECULAR, color);
		mat.material.specular.x = color.r;
		mat.material.specular.y = color.g;
		mat.material.specular.z = color.b;

		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_AMBIENT, color);
		mat.material.ambient.x = color.r;
		mat.material.ambient.y = color.g;
		mat.material.ambient.z = color.b;

		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_NAME, name);
		mat.material.name = name.C_Str();

		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_OPACITY, mat.material.alpha);
		tempmodel.material.push_back(mat);
		tempmodel.textures = this->loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "texture_diffuse", scene);
		//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}
	tempmodel.Init();
	return tempmodel;
	//return Mesh(dev_, vertices, indices, textures);
}
//

std::vector<Texture> AssimpLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene) {
	std::vector<Texture> textures;
	for (UINT i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		Texture tempTex;
		std::string path;
		mat->GetTexture(type, i, &str);
		path = str.C_Str();
		path = "Resources\\" + path;
		wchar_t wfilepath[128];
		int iBufferSize = MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, wfilepath, _countof(wfilepath));
		tempTex.CreateTexture(wfilepath);
		textures.push_back(tempTex);

	}
	return textures;
}

//void ModelLoader::Close() {
//	for (auto& t : textures_loaded_)
//		t.Release();
//
//	for (size_t i = 0; i < meshes_.size(); i++) {
//		meshes_[i].Close();
//	}
//}
//
//void AssimpLoader::processNode(aiNode* node, const aiScene* scene) {
//	for (UINT i = 0; i < node->mNumMeshes; i++) {
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//		nodes.push_back(this->processMesh(mesh, scene));
//	}
//
//	for (UINT i = 0; i < node->mNumChildren; i++) {
//		this->processNode(node->mChildren[i], scene);
//	}
//}
//
//ID3D11ShaderResourceView* ModelLoader::loadEmbeddedTexture(const aiTexture* embeddedTexture) {
//	HRESULT hr;
//	ID3D11ShaderResourceView* texture = nullptr;
//
//	if (embeddedTexture->mHeight != 0) {
//		// Load an uncompressed ARGB8888 embedded texture
//		D3D11_TEXTURE2D_DESC desc;
//		desc.Width = embeddedTexture->mWidth;
//		desc.Height = embeddedTexture->mHeight;
//		desc.MipLevels = 1;
//		desc.ArraySize = 1;
//		desc.SampleDesc.Count = 1;
//		desc.SampleDesc.Quality = 0;
//		desc.Usage = D3D11_USAGE_DEFAULT;
//		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//		desc.CPUAccessFlags = 0;
//		desc.MiscFlags = 0;
//
//		D3D11_SUBRESOURCE_DATA subresourceData;
//		subresourceData.pSysMem = embeddedTexture->pcData;
//		subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
//		subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;
//
//		ID3D11Texture2D* texture2D = nullptr;
//		hr = dev_->CreateTexture2D(&desc, &subresourceData, &texture2D);
//		if (FAILED(hr))
//			MessageBox(hwnd_, "CreateTexture2D failed!", "Error!", MB_ICONERROR | MB_OK);
//
//		hr = dev_->CreateShaderResourceView(texture2D, nullptr, &texture);
//		if (FAILED(hr))
//			MessageBox(hwnd_, "CreateShaderResourceView failed!", "Error!", MB_ICONERROR | MB_OK);
//
//		return texture;
//	}
//
//	// mHeight is 0, so try to load a compressed texture of mWidth bytes
//	const size_t size = embeddedTexture->mWidth;
//
//	hr = CreateWICTextureFromMemory(dev_, devcon_, reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, nullptr, &texture);
//	if (FAILED(hr))
//		MessageBox(hwnd_, "Texture couldn't be created from memory!", "Error!", MB_ICONERROR | MB_OK);
//
//	return texture;
//}
