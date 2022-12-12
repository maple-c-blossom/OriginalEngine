#include "FBXLoader.h"
#include "FBXModel.h"
#include "Dx12.h"
using namespace MCB;
using namespace Assimp;



MCB::FBXModel::~FBXModel()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < nodes[i]->meshes.size(); j++)
		{
			for (int k = 0; k < nodes[i]->meshes[j].textures.size(); k++)
			{
				nodes[i]->meshes[j].textures[k]->free = true;
			}
		}
	}
}


bool MCB::FBXModel::Load(std::string fileName) {
	// Create an instance of the Importer class
	Assimp::Importer importer;
	//importer.SetIOHandler(new MyIOSystem());
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	this->fileName = fileName;
	std::string baseDirectory = "Resources\\";
	std::string extend = ".fbx";
	fileName = baseDirectory + fileName + "\\" + fileName + extend;
	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_CalcTangentSpace | //インポートしたメッシュのタンジェントとかを計算
		aiProcess_Triangulate | //三角面化
		aiProcess_JoinIdenticalVertices | //インポートされたすべてのメッシュの中で、同一の頂点データセットを識別し、結合
		aiProcess_SortByPType |//2つ以上のプリミティブタイプを持つメッシュを均質なサブメッシュに分割
		aiProcess_GenNormals |//法線がない場合法線を計算
		aiProcess_FixInfacingNormals |//法線が内側を向いている場合、法線を反転してくれる
		aiProcess_ConvertToLeftHanded);//左手座標に必要な形に変換

	// If the import failed, report it
	if (nullptr == scene) {
		//DoTheErrorLogging(importer.GetErrorString());
		return false;
	}


	// Now we can access the file's contents.
	//DoTheSceneProcessing(scene);
	CopyNodesWithMeshes(scene->mRootNode, scene);
	// We're done. Everything will be cleaned up by the importer destructor
	//scene->~aiScene();
	return true;
}
void MCB::FBXModel::CopyNodesWithMeshes( aiNode* ainode,const aiScene* scene, Node* targetParent)
{
	Node* parent;
	//Matrix4x4 transform;

	std::unique_ptr<Node> newObject = std::make_unique<Node>();


	// if node has meshes, create a new scene object for it
		/*targetParent.addChild(newObject);*/
		//targetParent->parent = newObject.get();
		newObject->name = ainode->mName.C_Str();
		// copy the meshes
		//CopyMeshes(node, newObject);
		for (int i = 0; i < ainode->mNumMeshes; i++)
		{

			newObject->meshes.push_back(processMesh(scene->mMeshes[ainode->mMeshes[i]], scene));
			for (auto& itr :newObject->meshes)//なぜかreturnされるまでfalseだったtextureのfreeがtrueにされているので修正
			{
				for (auto& itr2 : itr.textures)
				{
					itr2->free = false;
				}
			}
		}
		// the new object is the parent for all child nodes

		// the new object is the parent for all child nodes
		newObject->transform.r[0].m128_f32[0] = ainode->mTransformation.a1;
		newObject->transform.r[0].m128_f32[1] = ainode->mTransformation.a2;
		newObject->transform.r[0].m128_f32[2] = ainode->mTransformation.a3;
		newObject->transform.r[0].m128_f32[3] = ainode->mTransformation.a4;

		newObject->transform.r[1].m128_f32[0] = ainode->mTransformation.b1;
		newObject->transform.r[1].m128_f32[1] = ainode->mTransformation.b2;
		newObject->transform.r[1].m128_f32[2] = ainode->mTransformation.b3;
		newObject->transform.r[1].m128_f32[3] = ainode->mTransformation.b4;

		newObject->transform.r[2].m128_f32[0] = ainode->mTransformation.c1;
		newObject->transform.r[2].m128_f32[1] = ainode->mTransformation.c2;
		newObject->transform.r[2].m128_f32[2] = ainode->mTransformation.c3;
		newObject->transform.r[2].m128_f32[3] = ainode->mTransformation.c4;

		newObject->transform.r[3].m128_f32[0] = ainode->mTransformation.d1;
		newObject->transform.r[3].m128_f32[1] = ainode->mTransformation.d2;
		newObject->transform.r[3].m128_f32[2] = ainode->mTransformation.d3;
		newObject->transform.r[3].m128_f32[3] = ainode->mTransformation.d4;

		newObject->transform = DirectX::XMMatrixTranspose(newObject->transform);
		newObject->globalTransform = newObject->transform;
		nodes.push_back(std::move(newObject));
		parent = newObject.get();
		//transform.SetUnity();

	if (targetParent)
	{
		targetParent->parent = parent;
		targetParent->globalTransform *= parent->globalTransform;
	}
	//// continue for all child nodes
	for (int i = 0; i < ainode->mNumChildren; i++) {
		CopyNodesWithMeshes(ainode->mChildren[i], scene, parent);
	}
}


FBXMesh FBXModel::processMesh(aiMesh* mesh, const aiScene* scene) {
	// Data to fill
	FBXMesh tempmodel;

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
		mat.Init();
		tempmodel.material.push_back(mat);
		tempmodel.textures = this->loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "texture_diffuse", scene);
		//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}
	tempmodel.Init();
	return tempmodel;
	//return Mesh(dev_, vertices, indices, textures);
}
void MCB::FBXModel::Draw()
{
	Dx12* dx12 = Dx12::GetInstance();
	ShaderResource* descriptor = ShaderResource::GetInstance();
	for (auto& itr : nodes)
	{
		for (auto& itr2 : itr->meshes)
		{
			//定数バッファビュー(CBV)の設定コマンド
			dx12->commandList->SetGraphicsRootConstantBufferView(2, itr2.material.begin()->constBuffMaterialB1->GetGPUVirtualAddress());
			//SRVヒープの先頭アドレスを取得
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();

			srvGpuHandle.ptr += itr2.textures.front()->texture->incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

			//SRVヒープの先頭にあるSRVをパラメータ1番に設定
			dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

			//頂点データ
			dx12->commandList->IASetVertexBuffers(0, 1, &itr2.vbView);
			//インデックスデータ
			dx12->commandList->IASetIndexBuffer(&itr2.ibView);
			//描画コマンド
			dx12->commandList->DrawIndexedInstanced((unsigned int)itr2.indices.size(), 1, 0, 0, 0);
		}
	}


}
//

std::vector<TextureCell*> FBXModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene) {
	std::vector<TextureCell*> textures;
	for (UINT i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		TextureCell* tempTex;
		std::string path;
		mat->GetTexture(type, i, &str);
		path = str.C_Str();
		path = "Resources\\" + fileName + "\\" + path;
		wchar_t wfilepath[128];
		int iBufferSize = MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, wfilepath, _countof(wfilepath));
		tempTex = textureManager->LoadTexture(wfilepath);
		textures.push_back(tempTex);

	}

	if (textures.empty())
	{
		TextureCell* tempTex;
		tempTex = textureManager->CreateNoTextureFileIsTexture();
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
