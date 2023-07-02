#include "FBXLoader.h"
#include "FBXModel.h"
#include "Dx12.h"
#include <list>
#include <algorithm>
#include "Util.h"
using namespace MCB;
using namespace Assimp;
using namespace DirectX;


MCB::AnimationModel::~AnimationModel()
{
	for (uint32_t i = 0; i < nodes_.size(); i++)
	{
		for (uint32_t j = 0; j < nodes_[i]->meshes.size(); j++)
		{
			for (uint32_t k = 0; k < nodes_[i]->meshes[j]->textures_.size(); k++)
			{
				nodes_[i]->meshes[j]->textures_[k]->free = true;
			}
		}
	}
}


bool MCB::AnimationModel::Load(std::string fileName,const std::string& fileType) {
	// Create an instance of the Importer class
	Assimp::Importer importer;
	//importer.SetIOHandler(new MyIOSystem());
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	fileName_ = fileName;
	std::string baseDirectory = "Resources\\";
	std::string extend = "." + fileType;
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
		return false;
	}


	// Now we can access the file's contents.
	//DoTheSceneProcessing(scene);
	CopyNodesWithMeshes(scene->mRootNode, scene);
	
	if (scene->mNumAnimations > 0)
	{
		for (uint32_t i = 0; i < scene->mNumAnimations; i++)
		{
			std::unique_ptr<Animation> tempAnim = std::make_unique<Animation>();
			tempAnim->duration = scene->mAnimations[i]->mDuration;
			tempAnim->name = scene->mAnimations[i]->mName.C_Str();
			tempAnim->ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;
			for (uint32_t j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
			{
				NodeAnim tempNodeAnim;
				tempNodeAnim.name = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();
				for (uint32_t k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++)
				{
					Vector3D tempPosition;
					double time;
					tempPosition.vec_.x_ = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.x;
					tempPosition.vec_.y_ = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.y;
					tempPosition.vec_.z_ = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.z;
					time = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;

					tempNodeAnim.position.push_back(tempPosition);
					tempNodeAnim.positionTime.push_back(time);
				}

				for (uint32_t k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++)
				{
					Vector3D tempScale;
					double time;
					tempScale.vec_.x_ = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.x;
					tempScale.vec_.y_ = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.y;
					tempScale.vec_.z_ = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.z;
					time = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime;
					tempNodeAnim.scale.push_back(tempScale);
					tempNodeAnim.scaleTime.push_back(time);
				}

				for (uint32_t k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++)
				{
					Quaternion tempRotation;
					double time;
					tempRotation.x_ = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x;
					tempRotation.y_ = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y;
					tempRotation.z_ = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z;
					tempRotation.w_ = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w;
					time = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;
					tempNodeAnim.rotation.push_back(tempRotation);
					tempNodeAnim.rotationTime.push_back(time);
				}
				tempAnim->channels.push_back(tempNodeAnim);
			}
			animations_[tempAnim->name] = move(tempAnim);
		}
	}

	// We're done. Everything will be cleaned up by the importer destructor
	//scene->~aiScene();
	return true;
}
void MCB::AnimationModel::CopyNodesWithMeshes( aiNode* ainode,const aiScene* scene, Node* targetParent)
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
		for (uint32_t i = 0; i < ainode->mNumMeshes; i++)//D3D12 ERROR: ID3D12Resource2::ID3D12Resource::Unmap: Resource (0x000001F358E61980:'Unnamed ID3D12Resource Object'), Subresource (0) is not mapped. [ RESOURCE_MANIPULATION ERROR #310: RESOURCE_UNMAP_NOTMAPPED]の原因
		{
			std::unique_ptr<AnimationMesh> tempmodel = std::make_unique<AnimationMesh>();
			processMesh(scene->mMeshes[ainode->mMeshes[i]], scene, *tempmodel.get()); //D3D12 ERROR 関数抜けた瞬間
			newObject->meshes.push_back(move(tempmodel));
			for (auto& itr :newObject->meshes)//なぜかreturnされるまでfalseだったtextureのfreeがtrueにされているので修正(上記のやつが同じ原因っぽい）
			{
				for (auto& itr2 : itr->textures_)
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
		nodes_.push_back(std::move(newObject));
		parent = nodes_.back().get();
		//transform.SetUnity();
		
	if (targetParent)
	{
		parent->parent = targetParent;
		targetParent->globalTransform *= parent->globalTransform;
		//targetParent->globalInverseTransform *= parent->globalInverseTransform;
	}
	//// continue for all child nodes
	for (uint32_t i = 0; i < ainode->mNumChildren; i++) {
		CopyNodesWithMeshes(ainode->mChildren[i], scene, parent);
	}
}


void AnimationModel::processMesh(aiMesh* mesh, const aiScene* scene, AnimationMesh& tempmodel) {
	// Data to fill

	// Walk through each of the mesh's vertices
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		AnimationVertex vertex;

		vertex.pos.x_ = mesh->mVertices[i].x;
		vertex.pos.y_ = mesh->mVertices[i].y;
		vertex.pos.z_ = mesh->mVertices[i].z;
		vertex.normal.x_ = mesh->mNormals[i].x;
		vertex.normal.y_ = mesh->mNormals[i].y;
		vertex.normal.z_ = mesh->mNormals[i].z;
		if (mesh->mTextureCoords[0]) {
			vertex.uv.x_ = (float)mesh->mTextureCoords[0][i].x;
			vertex.uv.y_ = (float)mesh->mTextureCoords[0][i].y;
		}

		tempmodel.vertices_.push_back(vertex);
	}


	for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (uint32_t j = 0; j < face.mNumIndices; j++)
			tempmodel.indices_.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) {
		aiColor3D color;
		ObjectMaterial mat;
		aiString name;

		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE,color);
		mat.material_.diffuse.x_ = color.r;
		mat.material_.diffuse.y_ = color.g;
		mat.material_.diffuse.z_ = color.b;

		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_SPECULAR, color);
		mat.material_.specular.x_ = color.r;
		mat.material_.specular.y_ = color.g;
		mat.material_.specular.z_ = color.b;

		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_AMBIENT, color);
		mat.material_.ambient.x_ = color.r;
		mat.material_.ambient.y_ = color.g;
		mat.material_.ambient.z_ = color.b;

		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_NAME, name);
		mat.material_.name = name.C_Str();

		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_OPACITY, mat.material_.alpha);
		mat.Init();
		tempmodel.material_.push_back(mat);
		tempmodel.textures_ = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "texture_diffuse", scene);
		//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}
	std::vector<std::list<SetWeight>> weightList(tempmodel.vertices_.size());
	for (uint32_t i = 0; i < mesh->mNumBones; i++)
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
		temp.finalMatrix = temp.offsetMatrix;
		for (uint32_t j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{

			SetWeight tempVer;
			tempVer.id = i;
			tempVer.weight = mesh->mBones[i]->mWeights[j].mWeight;
			weightList[ mesh->mBones[i]->mWeights[j].mVertexId].push_back(tempVer);
		}


		bones_.push_back(temp);
	}
	for (int32_t i = 0; i < tempmodel.vertices_.size(); i++)
	{
		auto& weightL = weightList[i];
		weightL.sort([](auto const& lhs, auto const rhs) {return lhs.weight > rhs.weight; });
		int32_t weightArrayIndex = 0;
		for (auto& weightSet : weightL)
		{
			
			tempmodel.vertices_[i].ids[weightArrayIndex] = weightSet.id;
			tempmodel.vertices_[i].weights[weightArrayIndex] = weightSet.weight;
			if (++weightArrayIndex >= NUM_BONES_PER_VERTEX)
			{
				float weight = 0.0f;
				for (int32_t j = 1; j < NUM_BONES_PER_VERTEX; j++)
				{
					weight += tempmodel.vertices_[i].weights[j];

				}
				tempmodel.vertices_[i].weights[0] = 1.0f - weight;
				break;
			}
		}
	}

	tempmodel.Init();
	//return tempmodel;
	//return Mesh(dev_, vertices, indices, textures);
}
void MCB::AnimationModel::Draw()
{
	Dx12* dx12 = Dx12::GetInstance();
	ShaderResource* descriptor = ShaderResource::GetInstance();
	for (auto& itr : nodes_)
	{
		for (auto& itr2 : itr->meshes)
		{
			//定数バッファビュー(CBV)の設定コマンド
			dx12->commandList_->SetGraphicsRootConstantBufferView(2, itr2->material_.begin()->constBuffMaterialB1_->GetGPUVirtualAddress());
			//SRVヒープの先頭アドレスを取得
			D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap_->GetGPUDescriptorHandleForHeapStart();

			srvGpuHandle.ptr += itr2->textures_.front()->texture->incrementNum_ * dx12->device_.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc_.Type);

			//SRVヒープの先頭にあるSRVをパラメータ1番に設定
			dx12->commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

			//頂点データ
			dx12->commandList_->IASetVertexBuffers(0, 1, &itr2->vbView_);
			//インデックスデータ
			dx12->commandList_->IASetIndexBuffer(&itr2->ibView_);
			//描画コマンド
			dx12->commandList_->DrawIndexedInstanced((uint32_t)itr2->indices_.size(), 1, 0, 0, 0);
		}
	}


}
//

std::vector<TextureCell*> AnimationModel::loadMaterialTextures(aiMaterial* mat,const aiTextureType& type,
	const std::string& typeName, const aiScene* scene)
{
	std::vector<TextureCell*> textures;
	for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		TextureCell* tempTex;
		std::string path;
		mat->GetTexture(type, i, &str);
		path = str.C_Str();
		while (path.find("\\") != std::string::npos)
		{
			path = path.substr(path.find("\\") + 1);
		}
		path = "Resources\\" + fileName_ + "\\" + path;
		wchar_t wfilepath[128];
		int32_t iBufferSize = MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, wfilepath, _countof(wfilepath));
		tempTex = textureManager_->LoadTexture(wfilepath);
		textures.push_back(tempTex);

	}

	if (textures.empty())
	{
		TextureCell* tempTex;
		tempTex = textureManager_->CreateNoTextureFileIsTexture();
		textures.push_back(tempTex);
	}
	
	return textures;
}

  void AnimationModel::boneAnimTransform( float& timeInSeconds, const std::string& currentAnimation, bool loop)
  {
    
    //if(!nodeAnimMapPtr)
    //{
    //  nodeAnimMapPtr = new NodeAnimMap();
    //}
    
    //NSLog(@"%d", scene->mNumAnimations);



	if (prevAnimName_ != currentAnimation)
	{
		auto itr = animations_.find(currentAnimation);
		if (itr == animations_.end())
		{
			return;
		}
		prevAnimName_ = currentAnimation;
		prevAnim = animations_[currentAnimation].get();
	}
	if (timeInSeconds >= animations_[currentAnimation]->duration)
	{
		timeInSeconds = 0;
	}
	Animation* anim;
	anim = prevAnim;

    float ticksPerSecond = (float)(anim->ticksPerSecond != 0 ? anim->ticksPerSecond : 25.0f);
    float timeInTicks = timeInSeconds * ticksPerSecond;
    float animationTime = timeInTicks;
    
    if(loop)
      animationTime = (float)fmod(animationTime, anim->duration);
    else
      animationTime = (float)min(animationTime, anim->duration -0.0001f);
    
	
	for (auto& itr : nodes_) itr->AnimaetionParentMat = XMMatrixIdentity();
	for (auto& itr : bones_) itr.finalMatrix = itr.offsetMatrix;
	for (auto& itr : nodes_)
	{
		readAnimNodeHeirarchy(animationTime, itr.get(), &itr->AnimaetionParentMat, itr->transform, currentAnimation);
	}
    
  /*  if(transforms->getCount() == 0)
      transforms->addElements((int32_t)boneMapping().size());
    
    for(uint32_t i = 0; i < boneMapping().size(); i++)
    {
      memcpy(transforms->elementAtIndex(i), &((BoneTransformInfo *)editBoneTransforms->elementAtIndex(i))->finalTransform, sizeof(float16));
    }*/
  }

  void AnimationModel::readAnimNodeHeirarchy( float animationTime, Node* pNode, DirectX::XMMATRIX* parentTransform, 
	  const DirectX::XMMATRIX& globalInverseTransform, const string& currentAnimation)
  {
	  const string& nodeName = pNode->name;

	  if (prevAnimName_ != currentAnimation)
	  {
		  auto itr = animations_.find(currentAnimation);
		  if (itr == animations_.end())
		  {
			  return;
		  }
	  }
	  const Animation* pAnimation = animations_[currentAnimation].get();

	  XMMATRIX nodeTrans = pNode->transform;

	  const NodeAnim* pNodeAnim = findNodeAnim(pAnimation, nodeName);

	  if (pNodeAnim)
	  {
		   //Interpolate scaling and generate scaling transformation matrix
		  Vector3D scaling;
		  calcInterpolatedScaling(scaling, animationTime, pNodeAnim);
		  XMMATRIX scalingM = XMMatrixScaling(scaling.vec_.x_, scaling.vec_.y_, scaling.vec_.z_);

		  // Interpolate rotation and generate rotation transformation matrix
		  Quaternion rotationQ;
		  calcInterpolatedRotation(rotationQ, animationTime, pNodeAnim);
		  //quat q = quat(rotationQ.x, rotationQ.y, rotationQ.z, rotationQ.w);
		  XMVECTOR XMrotationQ = { rotationQ.x_,rotationQ.y_,rotationQ.z_,rotationQ.w_ };
		  XMMATRIX rotationM = XMMatrixRotationQuaternion(XMrotationQ);

		  // Interpolate translation and generate translation transformation matrix
		  Vector3D translation;
		  calcInterpolatedPosition(translation, animationTime, pNodeAnim);
		  XMMATRIX translationM = XMMatrixTranslation( translation.vec_.x_, translation.vec_.y_, translation.vec_.z_ );

		   //Combine the above transformations
		  nodeTrans = scalingM * rotationM * translationM;
	  }
	  XMMATRIX mat;
	  if (pNode->parent) pNode->AnimaetionParentMat = nodeTrans * (pNode->parent->AnimaetionParentMat);
	  else pNode->AnimaetionParentMat = nodeTrans;
	 mat = /*pNode->globalTransform **/ pNode->AnimaetionParentMat;
	  Bone* bonePtr = nullptr;
	  for (auto& itr : bones_)
	  {
		  //std::string name = nodeName.substr(0, itr.name.size());
		  if (itr.name == nodeName)
		  {
			  bonePtr = &itr;
			  break;
		  }
	  }

	  if (bonePtr != nullptr)
	  {
		  //size_t boneIndex = iter;
		  //BoneTransformInfo* boneInfo = (BoneTransformInfo*)editBoneTransforms->elementAtIndex(boneIndex);
		  XMMATRIX* boneOff = &bonePtr->offsetMatrix;
		  XMMATRIX trans = (*boneOff) * (/*globalInverseTransform **/ mat) ;
		  bonePtr->finalMatrix = trans;
		  //memcpy(&boneInfo->finalTransform, &trans, sizeof(float16));
	  }

	  //for (uint32_t i = 0; i < pNode->mNumChildren; i++)
	  //{
		 // readAnimNodeHeirarchy(animationTime, pNode->mChildren[i], globalTransformation, scene, globalInverseTransform);
	  //}
  }

  const NodeAnim* AnimationModel::findNodeAnim(const Animation* pAnimation, const std::string& NodeName)
  {

	  for (uint32_t i = 0; i < pAnimation->channels.size(); i++)
	  {
		  const NodeAnim* pNodeAnim = &pAnimation->channels[i];

		  if (pNodeAnim->name == NodeName)
		  {
			  return pNodeAnim;
		  }
	  }

	  return NULL;
  }



   void AnimationModel::calcInterpolatedPosition(Vector3D& Out, float AnimationTime, const NodeAnim* pNodeAnim)
  {
	  if (pNodeAnim->position.size() == 1)
	  {
		  Out = pNodeAnim->position[0];
		  return;
	  }

	  uint32_t PositionIndex = static_cast<uint32_t>(findPosition(AnimationTime, pNodeAnim));
	  uint32_t NextPositionIndex = (PositionIndex + 1);
	  if(NextPositionIndex >= pNodeAnim->position.size()) NextPositionIndex = (uint32_t)pNodeAnim->position.size() - 1;
	  float DeltaTime = (float)(pNodeAnim->positionTime[NextPositionIndex] - pNodeAnim->positionTime[PositionIndex]);
	  float Factor = clamp((AnimationTime - (float)pNodeAnim->positionTime[PositionIndex]) / DeltaTime);
	  Vector3D Start = pNodeAnim->position[PositionIndex];
	  Vector3D End = pNodeAnim->position[NextPositionIndex];
	  Vector3D Delta = End - Start;
	  Out = Start + Factor * Delta;
  }

   void AnimationModel::calcInterpolatedRotation(Quaternion& Out, float AnimationTime, const NodeAnim* pNodeAnim)
  {
	  // we need at least two values to interpolate...
	  if (pNodeAnim->rotation.size() == 1) {
		  Out = pNodeAnim->rotation[0];
		  return;
	  }

	  uint32_t RotationIndex = static_cast<uint32_t>(findRotation(AnimationTime, pNodeAnim));
	  uint32_t NextRotationIndex = (RotationIndex + 1);
	  if (NextRotationIndex >= pNodeAnim->rotation.size()) NextRotationIndex = (uint32_t)pNodeAnim->rotation.size() - 1;
	  float DeltaTime = (float)(pNodeAnim->rotationTime[NextRotationIndex] - pNodeAnim->rotationTime[RotationIndex]);
	  float Factor = clamp((AnimationTime - (float)pNodeAnim->rotationTime[RotationIndex]) / DeltaTime);

	  const Quaternion& StartRotationQ = pNodeAnim->rotation[RotationIndex];
	  const Quaternion& EndRotationQ = pNodeAnim->rotation[NextRotationIndex];
	  Out = Out.Slerp(StartRotationQ, EndRotationQ, Factor);
	  Out.Normalize();
  }


   void AnimationModel::calcInterpolatedScaling(Vector3D& Out, float AnimationTime, const NodeAnim* pNodeAnim)
  {
	  if (pNodeAnim->scale.size() == 1) {
		  Out = pNodeAnim->scale[0];
		  return;
	  }

	  uint32_t ScalingIndex = static_cast<uint32_t>(findScaling(AnimationTime, pNodeAnim));
	  uint32_t NextScalingIndex = (ScalingIndex + 1);
	  if(NextScalingIndex >= pNodeAnim->scale.size()) NextScalingIndex = (uint32_t)pNodeAnim->scale.size() - 1;
	  float DeltaTime = (float)(pNodeAnim->scaleTime[NextScalingIndex] - pNodeAnim->scaleTime[ScalingIndex]);
	  float Factor = clamp((AnimationTime - (float)pNodeAnim->scaleTime[ScalingIndex]) / DeltaTime);
	  //assert(Factor >= 0.0f && Factor <= 1.0f);
	  Vector3D Start = pNodeAnim->scale[ScalingIndex];
	  Vector3D End = pNodeAnim->scale[NextScalingIndex];
	  Vector3D Delta = End - Start;
	  Out = Start + Factor * Delta;
  }

   size_t AnimationModel::findPosition( float AnimationTime, const NodeAnim* pNodeAnim)
   {
	   for (uint32_t i = 0; i < pNodeAnim->position.size(); i++) {
		   if (AnimationTime < (float)pNodeAnim->positionTime[i]) {
			   return i;
		   }
	   }

	   assert(0);

	   return 0;
   }


   size_t AnimationModel::findRotation( float AnimationTime, const NodeAnim* pNodeAnim)
   {
	   assert(pNodeAnim->rotation.size() > 0);

	   for (uint32_t i = 0; i < pNodeAnim->rotation.size() - 1; i++) {
		   if (AnimationTime < (float)pNodeAnim->rotationTime[i + 1]) {
			   return i;
		   }
	   }

	   assert(0);

	   return 0;
   }


   size_t AnimationModel::findScaling( float AnimationTime, const NodeAnim* pNodeAnim)
   {
	   assert(pNodeAnim->scale.size() > 0);

	   for (uint32_t i = 0; i < pNodeAnim->scale.size() - 1; i++) {
		   if (AnimationTime < (float)pNodeAnim->scaleTime[i + 1]) {
			   return i;
		   }
	   }

	   assert(0);

	   return 0;
   }