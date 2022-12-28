#include "FBXLoader.h"
#include "FBXModel.h"
#include "Dx12.h"
#include <list>
#include <algorithm>
#include "Util.h"
using namespace MCB;
using namespace Assimp;
using namespace DirectX;


MCB::FBXModel::~FBXModel()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < nodes[i]->meshes.size(); j++)
		{
			for (int k = 0; k < nodes[i]->meshes[j]->textures.size(); k++)
			{
				nodes[i]->meshes[j]->textures[k]->free = true;
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
	
	if (scene->mNumAnimations > 0)
	{
		for (int i = 0; i < scene->mNumAnimations; i++)
		{
			std::unique_ptr<Animation> tempAnim = std::make_unique<Animation>();
			tempAnim->duration = scene->mAnimations[i]->mDuration;
			tempAnim->name = scene->mAnimations[i]->mName.C_Str();
			tempAnim->ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;
			for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
			{
				NodeAnim tempNodeAnim;
				tempNodeAnim.name = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();
				for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++)
				{
					Vector3D tempPosition;
					double time;
					tempPosition.vec.x = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.x;
					tempPosition.vec.y = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.y;
					tempPosition.vec.z = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.z;
					time = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;

					tempNodeAnim.position.push_back(tempPosition);
					tempNodeAnim.positionTime.push_back(time);
				}

				for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++)
				{
					Vector3D tempScale;
					double time;
					tempScale.vec.x = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.x;
					tempScale.vec.y = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.y;
					tempScale.vec.z = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.z;
					time = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime;
					tempNodeAnim.scale.push_back(tempScale);
					tempNodeAnim.scaleTime.push_back(time);
				}

				for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++)
				{
					Quaternion tempRotation;
					double time;
					tempRotation.x = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x;
					tempRotation.y = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y;
					tempRotation.z = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z;
					tempRotation.w = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w;
					time = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;
					tempNodeAnim.rotation.push_back(tempRotation);
					tempNodeAnim.rotationTime.push_back(time);
				}
				tempAnim->channels.push_back(tempNodeAnim);
			}
			animations.push_back(move(tempAnim));
		}
	}

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
		for (int i = 0; i < ainode->mNumMeshes; i++)//D3D12 ERROR: ID3D12Resource2::ID3D12Resource::Unmap: Resource (0x000001F358E61980:'Unnamed ID3D12Resource Object'), Subresource (0) is not mapped. [ RESOURCE_MANIPULATION ERROR #310: RESOURCE_UNMAP_NOTMAPPED]の原因
		{
			std::unique_ptr<FBXMesh> tempmodel = std::make_unique<FBXMesh>();
			processMesh(scene->mMeshes[ainode->mMeshes[i]], scene, *tempmodel.get()); //D3D12 ERROR 関数抜けた瞬間
			newObject->meshes.push_back(move(tempmodel));
			for (auto& itr :newObject->meshes)//なぜかreturnされるまでfalseだったtextureのfreeがtrueにされているので修正(上記のやつが同じ原因っぽい）
			{
				for (auto& itr2 : itr->textures)
				{
					if (itr2->free)
					{
						itr2->free = false;
					}
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
		newObject->globalInverseTransform = XMMatrixInverse(nullptr,newObject->transform);
		nodes.push_back(std::move(newObject));
		parent = nodes.back().get();
		//transform.SetUnity();
		
	if (targetParent)
	{
		parent->parent = targetParent;
		targetParent->globalTransform *= parent->globalTransform;
		//targetParent->globalInverseTransform *= parent->globalInverseTransform;
	}
	//// continue for all child nodes
	for (int i = 0; i < ainode->mNumChildren; i++) {
		CopyNodesWithMeshes(ainode->mChildren[i], scene, parent);
	}
}


void FBXModel::processMesh(aiMesh* mesh, const aiScene* scene, FBXMesh& tempmodel) {
	// Data to fill

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
	std::vector<std::list<SetWeight>> weightList(tempmodel.vertices.size());
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		Bone temp;
		temp.name = mesh->mBones[i]->mName.C_Str();
		temp.offsetMatrix.r[0].m128_f32[0] = mesh->mBones[i]->mOffsetMatrix.a1;
		temp.offsetMatrix.r[0].m128_f32[1] = mesh->mBones[i]->mOffsetMatrix.a2;
		temp.offsetMatrix.r[0].m128_f32[2] = mesh->mBones[i]->mOffsetMatrix.a3;
		temp.offsetMatrix.r[0].m128_f32[3] = mesh->mBones[i]->mOffsetMatrix.a4;
											 
		temp.offsetMatrix.r[1].m128_f32[0] = mesh->mBones[i]->mOffsetMatrix.b1;
		temp.offsetMatrix.r[1].m128_f32[1] = mesh->mBones[i]->mOffsetMatrix.b2;
		temp.offsetMatrix.r[1].m128_f32[2] = mesh->mBones[i]->mOffsetMatrix.b3;
		temp.offsetMatrix.r[1].m128_f32[3] = mesh->mBones[i]->mOffsetMatrix.b4;
										
		temp.offsetMatrix.r[2].m128_f32[0] = mesh->mBones[i]->mOffsetMatrix.c1;
		temp.offsetMatrix.r[2].m128_f32[1] = mesh->mBones[i]->mOffsetMatrix.c2;
		temp.offsetMatrix.r[2].m128_f32[2] = mesh->mBones[i]->mOffsetMatrix.c3;
		temp.offsetMatrix.r[2].m128_f32[3] = mesh->mBones[i]->mOffsetMatrix.c4;
											 
		temp.offsetMatrix.r[3].m128_f32[0] = mesh->mBones[i]->mOffsetMatrix.d1;
		temp.offsetMatrix.r[3].m128_f32[1] = mesh->mBones[i]->mOffsetMatrix.d2;
		temp.offsetMatrix.r[3].m128_f32[2] = mesh->mBones[i]->mOffsetMatrix.d3;
		temp.offsetMatrix.r[3].m128_f32[3] = mesh->mBones[i]->mOffsetMatrix.d4;

		temp.offsetMatrix = DirectX::XMMatrixTranspose(temp.offsetMatrix);
		
		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{

			SetWeight tempVer;
			tempVer.id = i;
			tempVer.weight = mesh->mBones[i]->mWeights[j].mWeight;
			weightList[ mesh->mBones[i]->mWeights[j].mVertexId].push_back(tempVer);
		}


		bones.push_back(temp);
	}
	for (int i = 0; i < tempmodel.vertices.size(); i++)
	{
		auto& weightL = weightList[i];
		weightL.sort([](auto const& lhs, auto const rhs) {return lhs.weight > rhs.weight; });
		int weightArrayIndex = 0;
		for (auto& weightSet : weightL)
		{
			
			tempmodel.vertices[i].ids[weightArrayIndex] = weightSet.id;
			tempmodel.vertices[i].weights[weightArrayIndex] = weightSet.weight;
			if (++weightArrayIndex >= NUM_BONES_PER_VERTEX)
			{
				float weight = 0.0f;
				for (int j = 1; j < NUM_BONES_PER_VERTEX; j++)
				{
					weight += tempmodel.vertices[i].weights[j];

				}
				tempmodel.vertices[i].weights[0] = 1.0f - weight;
				break;
			}
		}
	}

	tempmodel.Init();
	//return tempmodel;
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
			dx12->commandList->SetGraphicsRootConstantBufferView(2, itr2->material.begin()->constBuffMaterialB1->GetGPUVirtualAddress());
			//SRVヒープの先頭アドレスを取得
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();

			srvGpuHandle.ptr += itr2->textures.front()->texture->incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

			//SRVヒープの先頭にあるSRVをパラメータ1番に設定
			dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

			//頂点データ
			dx12->commandList->IASetVertexBuffers(0, 1, &itr2->vbView);
			//インデックスデータ
			dx12->commandList->IASetIndexBuffer(&itr2->ibView);
			//描画コマンド
			dx12->commandList->DrawIndexedInstanced((unsigned int)itr2->indices.size(), 1, 0, 0, 0);
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
		while (path.find("\\") != std::string::npos)
		{
			path = path.substr(path.find("\\") + 1);
		}
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

  void FBXModel::boneAnimTransform(float timeInSeconds, unsigned int currentAnimation,bool loop)
  {

    
    //if(!nodeAnimMapPtr)
    //{
    //  nodeAnimMapPtr = new NodeAnimMap();
    //}
    
    //NSLog(@"%d", scene->mNumAnimations);
    
    float ticksPerSecond = (float)(animations[currentAnimation]->ticksPerSecond != 0 ? animations[currentAnimation]->ticksPerSecond : 25.0f);
    float timeInTicks = timeInSeconds * ticksPerSecond;
    float animationTime = timeInTicks;
    
    if(loop)
      animationTime = fmod(animationTime, animations[currentAnimation]->duration);
    else
      animationTime = min(animationTime, animations[currentAnimation]->duration -0.0001f);
    
	XMMATRIX matParent = XMMatrixIdentity();
	for (auto& itr : nodes)
	{
		readAnimNodeHeirarchy(animationTime, itr.get(), &matParent, itr->globalInverseTransform);
	}
    
  /*  if(transforms->getCount() == 0)
      transforms->addElements((int)boneMapping().size());
    
    for(uint i = 0; i < boneMapping().size(); i++)
    {
      memcpy(transforms->elementAtIndex(i), &((BoneTransformInfo *)editBoneTransforms->elementAtIndex(i))->finalTransform, sizeof(float16));
    }*/
  }

  void FBXModel::readAnimNodeHeirarchy(float animationTime, Node* pNode, DirectX::XMMATRIX* parentTransform, DirectX::XMMATRIX globalInverseTransform, unsigned int currentAnimation)
  {
	  const string& nodeName = pNode->name;

	  const Animation* pAnimation = animations[currentAnimation].get();

	  XMMATRIX nodeTrans = XMMatrixIdentity();

	  const NodeAnim* pNodeAnim = findNodeAnim(pAnimation, nodeName);

	  if (pNodeAnim)
	  {
		   //Interpolate scaling and generate scaling transformation matrix
		  Vector3D scaling;
		  calcInterpolatedScaling(scaling, animationTime, pNodeAnim);
		  XMMATRIX scalingM = XMMatrixScaling(scaling.vec.x, scaling.vec.y, scaling.vec.z);

		  // Interpolate rotation and generate rotation transformation matrix
		  Quaternion rotationQ;
		  calcInterpolatedRotation(rotationQ, animationTime, pNodeAnim);
		  //quat q = quat(rotationQ.x, rotationQ.y, rotationQ.z, rotationQ.w);
		  XMVECTOR XMrotationQ = { rotationQ.x,rotationQ.y,rotationQ.z,rotationQ.w };
		  XMMATRIX rotationM = XMMatrixRotationQuaternion(XMrotationQ);

		  // Interpolate translation and generate translation transformation matrix
		  Vector3D translation;
		  calcInterpolatedPosition(translation, animationTime, pNodeAnim);
		  XMMATRIX translationM = XMMatrixTranslation( translation.vec.x, translation.vec.y, translation.vec.z );

		   //Combine the above transformations
		  nodeTrans = scalingM * rotationM * translationM;
	  }
	  XMMATRIX mat;
	  *parentTransform = nodeTrans * (*parentTransform);
	  mat = /*pNode->globalTransform **/ *parentTransform;
	  Bone* bonePtr = nullptr;
	  for (auto& itr : bones)
	  {
		  if (itr.name == pNode->name)
		  {
			  bonePtr = &itr;
		  }
	  }

	  if (bonePtr != nullptr)
	  {
		  //unsigned int boneIndex = iter;
		  //BoneTransformInfo* boneInfo = (BoneTransformInfo*)editBoneTransforms->elementAtIndex(boneIndex);

		  XMMATRIX* boneOff = &bonePtr->offsetMatrix;
		  XMMATRIX trans = (*boneOff) * (/*globalInverseTransform **/ mat) ;
		  bonePtr->finalMatrix = trans;
		  //memcpy(&boneInfo->finalTransform, &trans, sizeof(float16));
	  }

	  //for (uint i = 0; i < pNode->mNumChildren; i++)
	  //{
		 // readAnimNodeHeirarchy(animationTime, pNode->mChildren[i], globalTransformation, scene, globalInverseTransform);
	  //}
  }

  const NodeAnim* FBXModel::findNodeAnim(const Animation* pAnimation, const std::string& NodeName)
  {

	  for (unsigned int i = 0; i < pAnimation->channels.size(); i++)
	  {
		  const NodeAnim* pNodeAnim = &pAnimation->channels[i];

		  if (pNodeAnim->name == NodeName)
		  {
			  return pNodeAnim;
		  }
	  }

	  return NULL;
  }



   void FBXModel::calcInterpolatedPosition(Vector3D& Out, float AnimationTime, const NodeAnim* pNodeAnim)
  {
	  if (pNodeAnim->position.size() == 1)
	  {
		  Out = pNodeAnim->position[0];
		  return;
	  }

	  unsigned int PositionIndex = findPosition(AnimationTime, pNodeAnim);
	  unsigned int NextPositionIndex = (PositionIndex + 1);
	  assert(NextPositionIndex < pNodeAnim->position.size());
	  float DeltaTime = (float)(pNodeAnim->positionTime[NextPositionIndex] - pNodeAnim->positionTime[PositionIndex]);
	  float Factor = clamp((AnimationTime - (float)pNodeAnim->positionTime[PositionIndex]) / DeltaTime);
	  Vector3D Start = pNodeAnim->position[PositionIndex];
	  Vector3D End = pNodeAnim->position[NextPositionIndex];
	  Vector3D Delta = End - Start;
	  Out = Start + Factor * Delta;
  }

   void FBXModel::calcInterpolatedRotation(Quaternion& Out, float AnimationTime, const NodeAnim* pNodeAnim)
  {
	  // we need at least two values to interpolate...
	  if (pNodeAnim->rotation.size() == 1) {
		  Out = pNodeAnim->rotation[0];
		  return;
	  }

	  unsigned int RotationIndex = findRotation(AnimationTime, pNodeAnim);
	  unsigned int NextRotationIndex = (RotationIndex + 1);
	  assert(NextRotationIndex < pNodeAnim->rotation.size());
	  float DeltaTime = (float)(pNodeAnim->rotationTime[NextRotationIndex] - pNodeAnim->rotationTime[RotationIndex]);
	  float Factor = clamp((AnimationTime - (float)pNodeAnim->rotationTime[RotationIndex]) / DeltaTime);

	  const Quaternion& StartRotationQ = pNodeAnim->rotation[RotationIndex];
	  const Quaternion& EndRotationQ = pNodeAnim->rotation[NextRotationIndex];
	  Out = Out.Slerp(StartRotationQ, EndRotationQ, Factor);
	  Out.QuaternoinNorm();
  }


   void FBXModel::calcInterpolatedScaling(Vector3D& Out, float AnimationTime, const NodeAnim* pNodeAnim)
  {
	  if (pNodeAnim->scale.size() == 1) {
		  Out = pNodeAnim->scale[0];
		  return;
	  }

	  unsigned int ScalingIndex = findScaling(AnimationTime, pNodeAnim);
	  unsigned int NextScalingIndex = (ScalingIndex + 1);
	  assert(NextScalingIndex < pNodeAnim->scale.size());
	  float DeltaTime = (float)(pNodeAnim->scaleTime[NextScalingIndex] - pNodeAnim->scaleTime[ScalingIndex]);
	  float Factor = clamp((AnimationTime - (float)pNodeAnim->scaleTime[ScalingIndex]) / DeltaTime);
	  //assert(Factor >= 0.0f && Factor <= 1.0f);
	  Vector3D Start = pNodeAnim->scale[ScalingIndex];
	  Vector3D End = pNodeAnim->scale[NextScalingIndex];
	  Vector3D Delta = End - Start;
	  Out = Start + Factor * Delta;
  }

    unsigned int FBXModel::findPosition(float AnimationTime, const NodeAnim* pNodeAnim)
   {
	   for (unsigned int i = 0; i < pNodeAnim->position.size() - 1; i++) {
		   if (AnimationTime < (float)pNodeAnim->positionTime[i + 1]) {
			   return i;
		   }
	   }

	   assert(0);

	   return 0;
   }


    unsigned int FBXModel::findRotation(float AnimationTime, const NodeAnim* pNodeAnim)
   {
	   assert(pNodeAnim->rotation.size() > 0);

	   for (unsigned int i = 0; i < pNodeAnim->rotation.size() - 1; i++) {
		   if (AnimationTime < (float)pNodeAnim->rotationTime[i + 1]) {
			   return i;
		   }
	   }

	   assert(0);

	   return 0;
   }


    unsigned int FBXModel::findScaling(float AnimationTime, const NodeAnim* pNodeAnim)
   {
	   assert(pNodeAnim->scale.size() > 0);

	   for (unsigned int i = 0; i < pNodeAnim->scale.size() - 1; i++) {
		   if (AnimationTime < (float)pNodeAnim->scaleTime[i + 1]) {
			   return i;
		   }
	   }

	   assert(0);

	   return 0;
   }