#include <MyImgui/imgui.h>
#include "AnimationModel.h"
#include "Dx12.h"
#include <list>
#include <algorithm>
#include "Util.h"
#include <algorithm>
#include "Object3d.h"
using namespace MCB;
using namespace Assimp;
using namespace DirectX;
using namespace std;

void MCB::AnimationModel::AnimationUpdate(float& timeInSeconds, const std::string& currentAnimation, bool loop)
{
	Animation* anim = animationManager.GetAnimation(currentAnimation);
	skeleton.boneAnimTransform(timeInSeconds,anim,loop);
}

MCB::AnimationModel::~AnimationModel()
{
	for (auto& node :  *skeleton.GetNodes_())
	{
		for (auto& mesh : node->meshes)
		{
			for (auto& texture: mesh->textures_)
			{
				texture->free = true;
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
			animationManager.SetAnimation(move(tempAnim));
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
	std::unique_ptr<Object3d> object = std::make_unique<Object3d>();
	newObject->object = move(object);
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
		newObject->localTransform.r[0].m128_f32[0] = ainode->mTransformation.a1;
		newObject->localTransform.r[0].m128_f32[1] = ainode->mTransformation.a2;
		newObject->localTransform.r[0].m128_f32[2] = ainode->mTransformation.a3;
		newObject->localTransform.r[0].m128_f32[3] = ainode->mTransformation.a4;

		newObject->localTransform.r[1].m128_f32[0] = ainode->mTransformation.b1;
		newObject->localTransform.r[1].m128_f32[1] = ainode->mTransformation.b2;
		newObject->localTransform.r[1].m128_f32[2] = ainode->mTransformation.b3;
		newObject->localTransform.r[1].m128_f32[3] = ainode->mTransformation.b4;

		newObject->localTransform.r[2].m128_f32[0] = ainode->mTransformation.c1;
		newObject->localTransform.r[2].m128_f32[1] = ainode->mTransformation.c2;
		newObject->localTransform.r[2].m128_f32[2] = ainode->mTransformation.c3;
		newObject->localTransform.r[2].m128_f32[3] = ainode->mTransformation.c4;

		newObject->localTransform.r[3].m128_f32[0] = ainode->mTransformation.d1;
		newObject->localTransform.r[3].m128_f32[1] = ainode->mTransformation.d2;
		newObject->localTransform.r[3].m128_f32[2] = ainode->mTransformation.d3;
		newObject->localTransform.r[3].m128_f32[3] = ainode->mTransformation.d4;
		aiVector3D s;
		aiQuaternion r;
		aiVector3D t;
		ainode->mTransformation.Decompose(s, r, t);
		newObject->scale.m128_f32[0] = s.x;
		newObject->scale.m128_f32[1] = s.y;
		newObject->scale.m128_f32[2] = s.z;

		newObject->object->rotationQ_.x_ = newObject->rotation.m128_f32[0] = r.x;
		newObject->object->rotationQ_.y_ = newObject->rotation.m128_f32[1] = r.y;
		newObject->object->rotationQ_.z_ = newObject->rotation.m128_f32[2] = r.z;
		newObject->object->rotationQ_.w_ = newObject->rotation.m128_f32[3] = r.w;

		newObject->object->position_.x = newObject->translation.m128_f32[0] = t.x;
		newObject->object->position_.y = newObject->translation.m128_f32[1] = t.y;
		newObject->object->position_.z = newObject->translation.m128_f32[2] = t.z;


		newObject->localTransform = DirectX::XMMatrixTranspose(newObject->localTransform);
		XMMatrixDecompose(&newObject->defaultScale, &newObject->defaultRotation, &newObject->defaultLocalTranslation, newObject->localTransform);

		/*newObject->defaultTranslation = newObject->translation;
		newObject->defaultRotation = newObject->rotation;*/
		newObject->endPosition.vec_.x_ = newObject->translation.m128_f32[0];
		newObject->endPosition.vec_.y_ = newObject->translation.m128_f32[1];
		newObject->endPosition.vec_.z_ = newObject->translation.m128_f32[2];
		if (targetParent) newObject->startPosition = targetParent->endPosition;
		else newObject->startPosition = { 0,0,0 };
		newObject->defaultBoneVec = Vector3D().Vector3Substruct(newObject->startPosition.vec_, newObject->endPosition.vec_);
		newObject->globalTransform = newObject->localTransform;
		newObject->defaultModelTransform = newObject->globalTransform;
		newObject->globalInverseTransform = XMMatrixInverse(nullptr, newObject->globalTransform);
		skeleton.SetNode(std::move(newObject));
		parent = skeleton.GetNodes_()->back().get();
		//transform.SetUnity();
		
	if (targetParent)
	{
		parent->parent = targetParent;
		parent->object->nameId_ = parent->name;
		parent->object->parent_ = targetParent->object.get();
		parent->globalTransform *= targetParent->globalTransform;
		parent->defaultModelTransform = parent->globalTransform;
		parent->globalInverseTransform = XMMatrixInverse(nullptr, parent->globalTransform);
		targetParent->children.push_back(parent);
		//targetParent->globalInverseTransform *= parent->globalInverseTransform;
	}
	else
	{
		if (skeleton.rootNode == nullptr)
		{
			skeleton.rootNode = parent;
		}
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


		tempmodel.bones_.push_back(temp);
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
	for (auto& itr : *skeleton.GetNodes_())
	{
		for (auto& itr2 : itr->meshes)
		{
			for (auto& itr3: itr2->constMapSkin_->boneMats)
			{
				itr3 = XMMatrixIdentity();
			}
			for (size_t i = 0; i < itr2->bones_.size(); i++)
			{
				itr2->constMapSkin_->boneMats[i] = itr2->bones_[i].finalMatrix;
			}
		}
	}
	for (auto& itr : *skeleton.GetNodes_())
	{
		for (auto& itr2 : itr->meshes)
		{
			dx12->commandList_->SetGraphicsRootConstantBufferView(4, itr2->constBuffSkin_->GetGPUVirtualAddress());
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

void MCB::AnimationModel::DrawHeirarchy()
{
	if (ImGui::CollapsingHeader(string(fileName_ + "::Skeleton").c_str()))
	{
		float* color[3] = {&skeleton.lineDefaultColor.x_,&skeleton.lineDefaultColor.y_
		,&skeleton.lineDefaultColor.z_};
		ImGui::ColorEdit3("LineColor", *color);
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(450, 350), ImGuiWindowFlags_NoTitleBar, ImGuiWindowFlags_HorizontalScrollbar);
		{
			skeleton.DrawHeirarchy(skeleton.rootNode);
			//ImGui::TreePop();
		}
		ImGui::EndChild();
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

void MCB::AnimationModel::TwoBoneIkOrder(Object3d& objPos, Vector3D targetPos)
{
	skeleton.SetTwoBoneIK(objPos, targetPos);
}

  void Skeleton::boneAnimTransform( float& timeInSeconds, Animation* animation, bool loop)
  {
    
    //if(!nodeAnimMapPtr)
    //{
    //  nodeAnimMapPtr = new NodeAnimMap();
    //}
    
    //NSLog(@"%d", scene->mNumAnimations);
	  float animationTime = 0;
	  if (animation != nullptr)
	  {

		  if (prevAnimation != animation)
		  {
			  prevAnimation = currentAnimation;
			  currentAnimation = animation;
		  }
		  if (timeInSeconds >= currentAnimation->duration)
		  {
			  timeInSeconds = 0;
		  }


		  float ticksPerSecond = (float)(currentAnimation->ticksPerSecond != 0 ? currentAnimation->ticksPerSecond : 25.0f);
		  float timeInTicks = timeInSeconds * ticksPerSecond;
		  animationTime = timeInTicks;

		  if (loop)
			  animationTime = (float)fmod(animationTime, currentAnimation->duration);
		  else
			  animationTime = (float)min(animationTime, currentAnimation->duration - 0.0001f);

	  }
	for (auto& itr : nodes_)
	{
		itr->AnimaetionParentMat = XMMatrixIdentity();
		for (auto& itr2 : itr->meshes)
		{
			for (auto& itr3 : itr2->bones_) itr3.finalMatrix = itr3.offsetMatrix;
		}
	}

	for (auto& itr : nodes_)
	{
		readAnimNodeHeirarchy(animationTime, itr.get(),  currentAnimation);
	}
	AllNodeMatrixForModelToBone();
  /*  if(transforms->getCount() == 0)
      transforms->addElements((int32_t)boneMapping().size());
    
    for(uint32_t i = 0; i < boneMapping().size(); i++)
    {
      memcpy(transforms->elementAtIndex(i), &((BoneTransformInfo *)editBoneTransforms->elementAtIndex(i))->finalTransform, sizeof(float16));
    }*/
  }

  void Skeleton::readAnimNodeHeirarchy( float animationTime, Node* pNode,  Animation* currentAnimation)
  {
	  const string& nodeName = pNode->name;

	  const Animation* pAnimation = currentAnimation;

	  XMMATRIX nodeTrans = pNode->localTransform;

	  const NodeAnim* pNodeAnim = findNodeAnim(pAnimation, nodeName);

	  //XMMATRIX mat;
	  if (pNodeAnim)
	  {
		  // //Interpolate scaling and generate scaling transformation matrix
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
		  pNode->translation = translation.ConvertXMVEC();
		  pNode->scale = scaling.ConvertXMVEC();
		  pNode->rotation = rotationQ.ConvertXMVector();

		  nodeTrans = scalingM * rotationM * translationM;

	  }
	  if (pNode->parent) pNode->AnimaetionParentMat = nodeTrans * (pNode->parent->AnimaetionParentMat);
	  else pNode->AnimaetionParentMat = nodeTrans;
	  XMMatrixDecompose(&pNode->scale, &pNode->rotation, &pNode->translation, nodeTrans);
	  pNode->endPosition.vec_.x_ = pNode->translation.m128_f32[0];
	  pNode->endPosition.vec_.y_ = pNode->translation.m128_f32[1];
	  pNode->endPosition.vec_.z_ = pNode->translation.m128_f32[2];
	  if (pNode->parent) pNode->startPosition = pNode->parent->endPosition;
	  else pNode->startPosition = { 0,0,0 };
	  pNode->boneVec = Vector3D().Vector3Substruct(pNode->startPosition.vec_, pNode->endPosition.vec_);
	  pNode->boneLength = pNode->boneVec.V3Len();


  }

  const NodeAnim* Skeleton::findNodeAnim(const Animation* pAnimation, const std::string& NodeName)
  {
	  if (!pAnimation) return nullptr;
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



   void Skeleton::calcInterpolatedPosition(Vector3D& Out, float AnimationTime, const NodeAnim* pNodeAnim)
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

   void Skeleton::calcInterpolatedRotation(Quaternion& Out, float AnimationTime, const NodeAnim* pNodeAnim)
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


   void Skeleton::calcInterpolatedScaling(Vector3D& Out, float AnimationTime, const NodeAnim* pNodeAnim)
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

   size_t Skeleton::findPosition( float AnimationTime, const NodeAnim* pNodeAnim)
   {
	   for (uint32_t i = 0; i < pNodeAnim->position.size(); i++) {
		   if (AnimationTime < (float)pNodeAnim->positionTime[i]) {
			   return i;
		   }
	   }

	   assert(0);

	   return 0;
   }


   size_t Skeleton::findRotation( float AnimationTime, const NodeAnim* pNodeAnim)
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


   size_t Skeleton::findScaling( float AnimationTime, const NodeAnim* pNodeAnim)
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


   float cosineFrom3LineLength(float a = 3, float b = 3, float c = 3)
   {
	   float temp = ((powf(b, 2) + powf(c, 2) - powf(a, 2)) /
		   (2.f * b * c));

	   return clamp(temp,-1,1);
   }

   void MCB::Skeleton::TwoBoneIK(Node& endJoint, Node& middleJoint)
   {
	   
	   if (&endJoint == nullptr)return;
	   if (&middleJoint == nullptr)return;
	   //座標変換(rootJointの座標系に変換)-----------------------------------
	   Node* rootJoint = middleJoint.parent;
	   if(!rootJoint->chengeObjectColor)rootJoint->object->color_ = { 0.5f,0,0.5f,1 };
	   if(!middleJoint.chengeObjectColor)middleJoint.object->color_ = { 0,0.5f,0,1 };
	   if(!endJoint.chengeObjectColor)endJoint.object->color_ = { 0,0,0.5f,1 };
	   
	   endJoint.ikData.middleJointNode = &middleJoint;
	   endJoint.ikData.rootJointNode = rootJoint;

	   MCBMatrix rootJointModelMatrixinv = rootJointModelMatrixinv.MatrixInverse(rootJoint->defaultModelTransform);
	   //向きたい場所のObj座標系の場所
	   Vector3D effectorWorldVec = endJoint.ikData.iKEffectorPosition;//Objからの相対位置(targetPos - ObjPos)
	   //向きたい場所(RootJointの座標系)
	   Vector3D EffectorLocalFromRootPos = MCBMatrix::GetTranslate(MCBMatrix::MCBMatrixTransrate(effectorWorldVec) * rootJointModelMatrixinv);



	   Vector3D xmLocalConstraintVectorFromRoot = MCBMatrix::GetTranslate(MCBMatrix::MCBMatrixTransrate(endJoint.ikData.constraintModelVector) * 
		   rootJointModelMatrixinv);

	   endJoint.ikData.constraintLocalPositionFromRoot = xmLocalConstraintVectorFromRoot;

	   Vector3D middleJointLocalPositionFromRoot = MCBMatrix::GetTranslate(middleJoint.defaultModelTransform * rootJointModelMatrixinv);
	   Vector3D endJointLocalPositionFromRoot = MCBMatrix::GetTranslate(endJoint.defaultModelTransform * rootJointModelMatrixinv);
	   Vector3D rootJointLocalPositionFromRoot = MCBMatrix::GetTranslate(rootJoint->defaultModelTransform * rootJointModelMatrixinv);
	   endJoint.ikData.effectorPosFromRoot = EffectorLocalFromRootPos;
	   //xmEffectorLocalVecFromRoot = endJointLocalPositionFromRoot;
	   //------------------------------
	   Vector3D nd = nd.GetV3Normal(rootJointLocalPositionFromRoot,
		   middleJointLocalPositionFromRoot, endJointLocalPositionFromRoot);//rootJointからみた位置で法線取ってるならrootJointは原点じゃね？
	   endJoint.ikDebugData.defaultTriangleNormal = nd;

	   Vector3D nt = nt.GetV3Normal(rootJointLocalPositionFromRoot,
		   EffectorLocalFromRootPos, xmLocalConstraintVectorFromRoot);
	   endJoint.ikData.rootAndEffectorAndConstraintTriangle.PointA_ = rootJointLocalPositionFromRoot;
	   endJoint.ikData.rootAndEffectorAndConstraintTriangle.PointB_ = EffectorLocalFromRootPos;
	   endJoint.ikData.rootAndEffectorAndConstraintTriangle.PointC_ = xmLocalConstraintVectorFromRoot;

	   endJoint.ikData.jointTriangle.PointA_ = rootJointLocalPositionFromRoot;
	   endJoint.ikData.jointTriangle.PointB_ = middleJointLocalPositionFromRoot;
	   endJoint.ikData.jointTriangle.PointC_ = endJointLocalPositionFromRoot;

	   endJoint.ikDebugData.taregetTriangleNormal = nt;
	   Quaternion q1;//同一平面上にいるようにする回転
	   q1 = q1.GetDirectProduct(rootJoint->defaultRotation,q1.DirToDir(nd, nt));
	   Vector3D middleBoneVector = middleJointLocalPositionFromRoot;
	   float middleJointBoneLength = middleJointLocalPositionFromRoot.V3Len();
	   float endJointBoneLength = Vector3D(endJoint.defaultLocalTranslation).V3Len();
	   float rootToEndLength = middleJointLocalPositionFromRoot.V3Len();
	   float localTargetVectorFromRootJoint = EffectorLocalFromRootPos.V3Len();

	   float angleFromdefaultTriangle = acos(cosineFrom3LineLength(middleJointBoneLength, rootToEndLength, endJointBoneLength));//余弦定理で角度算出
	   float angleFromTargetTriangle = acos(cosineFrom3LineLength(middleJointBoneLength, localTargetVectorFromRootJoint, endJointBoneLength));//余弦定理で角度算出

	   float theta = angleFromTargetTriangle - angleFromdefaultTriangle;
	   Quaternion d2RotaionQ(nt, theta);//平面の回転で考えるならnt(平面の法線)を回転軸として利用してもいいと予想
	   Vector3D targetMiddleVector = d2RotaionQ.SetRotationVector(d2RotaionQ, Vector3D(EffectorLocalFromRootPos));//rootからmiddleにいてほしい場所までのベクトル算出
	   Quaternion q2 = q2.DirToDir(middleBoneVector,targetMiddleVector);
	   Quaternion rootJointRotation = rootJointRotation.GetDirectProduct(q2,q1);
	   rootJoint->rotation = q1.ConvertXMVector();
	   UpdateNodeMatrix(rootJoint);
	   UpdateNodeMatrix(&middleJoint);//middleJointを回転させる
	   UpdateNodeMatrix(&endJoint);

	   effectorWorldVec = endJoint.ikData.iKEffectorPosition;//Objからの相対位置(objPos - targetPos)
	   EffectorLocalFromRootPos = effectorWorldVec.ConvertXMVEC();
	   MCBMatrix middleJointWorldMatrixinv = middleJointWorldMatrixinv.MatrixInverse(middleJoint.AnimaetionParentMat);
	   Vector3D xmTargetLocalVecFromMiddle = MCBMatrix::GetTranslate(
		   MCBMatrix::MCBMatrixTransrate(EffectorLocalFromRootPos) * middleJointWorldMatrixinv);
	   endJoint.ikData.effectorPosFromMiddle = xmTargetLocalVecFromMiddle;
	   Vector3D endJointLocalVecFromMiddle = MCBMatrix::GetTranslate(endJoint.AnimaetionParentMat * middleJointWorldMatrixinv);
	   Vector3D localTargetVectorFromMiddle =  xmTargetLocalVecFromMiddle;
	   Quaternion q3 = q3.DirToDir(endJointLocalVecFromMiddle, localTargetVectorFromMiddle);
	   q3 = q3.GetDirectProduct(middleJoint.defaultRotation,q3);
	   middleJoint.rotation = q3.ConvertXMVector();
	   UpdateNodeMatrix(rootJoint);
	   UpdateNodeMatrix(&middleJoint);
	   UpdateNodeMatrix(&endJoint);
   }

   void MCB::Skeleton::AllNodeMatrixForModelToBone()
   {
	   for (auto& node : nodes_)
	   {
		   if (node->ikData.isIK)
		   {
			   TwoBoneIK(*node.get(), *node->parent);
		   }
		   UpdateNodeMatrix(node.get());
	   }
	   for (auto& node : nodes_)
	   {
		   XMMATRIX mat = node->AnimaetionParentMat;
		   const string& nodeName = node->name;
		   std::list<Bone*> bonePtr{};
		   for (auto& itr : nodes_)
		   {
			   for (auto& itr2 : itr->meshes)
			   {
				   for (auto& itr3 : itr2->bones_)
				   {

					   if (itr3.name == nodeName)
					   {
						   bonePtr.push_back(&itr3);
						   break;
					   }
				   }
			   }
		   }

		   if (!bonePtr.empty())
		   {
			   for (auto& itr : bonePtr)
			   {
				   XMMATRIX* boneOff = &itr->offsetMatrix;
				   XMMATRIX trans = (*boneOff) * (mat);
				   itr->finalMatrix = trans;
			   }

		   }

	   }

   }

   void MCB::Skeleton::CCDIK(Node& effectter, Vector3D targetPos, int32_t numMaxIteration, float errToleranceSq)
   {
	   XMVECTOR localTargetPos = XMVectorZero();
	   XMVECTOR localEffecrPos = XMVectorZero();

	   for (int32_t i = 0; i < numMaxIteration; i++)
	   {
		   for (Node* joint = effectter.parent; joint != nullptr; joint = joint->parent)
		   {
				XMVECTOR effectorPos = effectter.translation;
				XMVECTOR jointPos = joint->translation;
				XMMATRIX invCoord = XMMatrixInverse(nullptr, joint->AnimaetionParentMat);
				localEffecrPos = XMVector3Transform(effectorPos, invCoord);
				localTargetPos = XMVector3Transform(targetPos.ConvertXMVEC(), invCoord);
				
		   }
	   }


   }

   void MCB::Skeleton::SetTwoBoneIK(const Object3d& obj, const Vector3D& targetPos, const Vector3D& constraintPosition,
	   const string& boneName)
   {
	   Vector3D pos(obj.position_.x,obj.position_.y,obj.position_.z);
	   WorldMatrix mat = obj.matWorld_;
	   MCBMatrix worldMatInv = XMMatrixInverse(nullptr,mat.matWorld_);
	   Node* node;
	   if (boneName == "NULL")
	   {
			 node = GetNearPositionNode(targetPos,pos);
	   }
	   else
	   {
		   node = GetNode(boneName);
	   }
	   if (node)
	   {
		   node->ikData.isIK = true;
		  
		   node->ikData.iKEffectorPosition = MCBMatrix::GetTranslate(MCBMatrix::MCBMatrixTransrate(targetPos) * worldMatInv);
		   node->ikData.constraintWorldVector = constraintPosition;
		   node->ikData.constraintModelVector = MCBMatrix::GetTranslate(MCBMatrix::MCBMatrixTransrate(constraintPosition) * worldMatInv);
	   }
   }
   void MCB::Skeleton::TwoBoneIKOff(const string& boneName)
   {
	   Node* node;
	   if (boneName == "NULL")
	   {
		   return;
	   }


		node = GetNode(boneName);

	   if (node)
	   {
		   node->ikData.isIK = false;

	   }
   }



   void MCB::Skeleton::Vectorconstraiont(Node& joint)
   {
	   Vector3D vec = joint.ikData.constraintModelVector;
	   XMVECTOR xmVec = vec.ConvertXMVEC();
	   xmVec = XMVector3Transform(xmVec, joint.globalInverseTransform);
	   float root = sqrtf(powf(vec.vec_.y_, 2) + powf(vec.vec_.z_, 2));
	   float cosX = vec.vec_.y_ / root;
	   float sinX = vec.vec_.z_ / root;

	   XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(0.f,0.f, atan2f(sinX,cosX));
	   XMVECTOR currentRotation = joint.rotation;
	   XMVECTOR finalRotation = XMQuaternionMultiply(currentRotation, rotation);
	   joint.rotation = finalRotation;

   }

   void MCB::Skeleton::ApplyRotation(Node& joint, const XMFLOAT3& axis, float angle)
   {
	   XMVECTOR rotationAxis = XMLoadFloat3(&axis);
	   XMVECTOR rotation = XMQuaternionRotationAxis(rotationAxis, angle);
	   XMVECTOR currentRotation = joint.rotation;
	   XMVECTOR finalRotation = XMQuaternionMultiply(currentRotation, rotation);
	   joint.rotation = finalRotation;
   }

   void MCB::Skeleton::DrawHeirarchy(Node* node)
   {
	   string transTag = node->name + "_Transform";
	   string child = node->name + "_children";
	   if (ImGui::CollapsingHeader(node->name.c_str()))
	   {
		   if (ImGui::TreeNode(transTag.c_str()))
		   {
			   ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(450, 250), ImGuiWindowFlags_NoTitleBar);
			  {
			   
				   ImGui::InputFloat3("position", node->translation.m128_f32);
				   ImGui::InputFloat4("rotation", node->rotation.m128_f32);
				   ImGui::InputFloat3("scale", node->scale.m128_f32);
				   if (ImGui::TreeNode("Matrix"))
				   {
					   ImGui::Text("%f, %f, %f, %f", node->localTransform.r[0].m128_f32[0], node->localTransform.r[0].m128_f32[1],
						   node->localTransform.r[0].m128_f32[2], node->localTransform.r[0].m128_f32[3]);

					   ImGui::Text("%f, %f, %f, %f", node->localTransform.r[1].m128_f32[0], node->localTransform.r[1].m128_f32[1],
						   node->localTransform.r[1].m128_f32[2], node->localTransform.r[1].m128_f32[3]);

					   ImGui::Text("%f, %f, %f, %f", node->localTransform.r[2].m128_f32[0], node->localTransform.r[2].m128_f32[1],
						   node->localTransform.r[2].m128_f32[2], node->localTransform.r[2].m128_f32[3]);

					   ImGui::Text("%f, %f, %f, %f", node->localTransform.r[3].m128_f32[0], node->localTransform.r[3].m128_f32[1],
						   node->localTransform.r[3].m128_f32[2], node->localTransform.r[3].m128_f32[3]);
					   ImGui::TreePop();
				   }

					ImGui::Text("BoneVec:%f,%f,%f", node->boneVec.vec_.x_, node->boneVec.vec_.y_, node->boneVec.vec_.z_);
					ImGui::Text("BoneLength:%f", node->boneLength);
					ImGui::Checkbox("jointView", &node->jointView);
					Float4* objCol = &node->object->color_;
					float* color[3] = { &objCol->x_,&objCol->y_,&objCol->z_ };
					ImGui::Checkbox("chengeObjectColor", &node->chengeObjectColor);
					if (node->chengeObjectColor)ImGui::ColorEdit3("JointColor", *color);
					else for (auto& ptr : color) *ptr = 1.0f;
					ImGui::Checkbox("objectColorLine", &node->lineColorEqualObject);
					if (node->ikData.isIK)
					{
						if (ImGui::TreeNode("IkData"))
						{
							Node::IKData& nodeIkData = node->ikData;
							if (node->ikData.rootJointNode)
							{
								ImGui::Text("RootJointName:%s", nodeIkData.rootJointNode->name.c_str());
							}
							if (node->ikData.middleJointNode)
							{
								ImGui::Text("MiddleJointName:%s", nodeIkData.middleJointNode->name.c_str());
							}
							ImGui::Text("EffectorPositionFromObj:%f,%f,%f", nodeIkData.iKEffectorPosition.vec_.x_,
								nodeIkData.iKEffectorPosition.vec_.y_, nodeIkData.iKEffectorPosition.vec_.z_);
							ImGui::Text("ConstRaintPosFromWorld:%f,%f,%f", nodeIkData.constraintWorldVector.vec_.x_,
								nodeIkData.constraintWorldVector.vec_.y_, nodeIkData.constraintWorldVector.vec_.z_);
							ImGui::TreePop();
						}
					}
					else
					{
						ImGui::Text("NoEndJoint");
					}
			   }
			   ImGui::EndChild();
			   ImGui::TreePop();

		   }

		   if (node->children.size())
		   {
				if (ImGui::TreeNode(child.c_str()))
				{

					for (auto& child : node->children)
					{
					DrawHeirarchy(child);
					}
					ImGui::TreePop();
				}

		   }
		   
	   }
   }

   void MCB::Skeleton::UpdateNodeMatrix(Node* pNode)
   {
	   XMMATRIX nodeTrans;
	   XMMATRIX scalingM = XMMatrixScaling(pNode->scale.m128_f32[0], pNode->scale.m128_f32[1], pNode->scale.m128_f32[2]);
	   XMMATRIX rotationM = XMMatrixRotationQuaternion(pNode->rotation);
	   XMMATRIX translationM = XMMatrixTranslation(pNode->translation.m128_f32[0], pNode->translation.m128_f32[1], pNode->translation.m128_f32[2]);
	   nodeTrans = scalingM * rotationM * translationM;
	   if (pNode->parent) pNode->AnimaetionParentMat = nodeTrans * (pNode->parent->AnimaetionParentMat);
	   else pNode->AnimaetionParentMat = nodeTrans;
	   XMMatrixDecompose(&pNode->scale, &pNode->rotation, &pNode->translation, nodeTrans);
	   pNode->endPosition.vec_.x_ = pNode->translation.m128_f32[0];
	   pNode->endPosition.vec_.y_ = pNode->translation.m128_f32[1];
	   pNode->endPosition.vec_.z_ = pNode->translation.m128_f32[2];
	   if (pNode->parent) pNode->startPosition = pNode->parent->endPosition;
	   else pNode->startPosition = { 0,0,0 };
	   pNode->boneVec = Vector3D().Vector3Substruct(pNode->startPosition.vec_, pNode->endPosition.vec_);
	   pNode->boneLength = pNode->boneVec.V3Len();

	

   }

   void MCB::Skeleton::JointObjectMatrixUpdate(ICamera* camera, Object3d* Obj, Model* model, const Float3& scale)
   {
	   for (auto& node : nodes_)
	   {
		   node->lineDefaultColor = lineDefaultColor;
		   node->JointObjectMatrixUpdate(camera, Obj, model,scale);
	   }
   }

   void MCB::Skeleton::JointObjectDraw()
   {
	   for (auto& node : nodes_)
	   {
		   if (!node->jointView)continue;
		  node->JointObjectDraw();
	   }
   }

   void MCB::Skeleton::JointLineDraw()
   {
	   for (auto& node : nodes_)
	   {
		   if (!node->jointView)continue;
		   node->JointLineDraw();
	   }
   }

   Node* MCB::Skeleton::GetNearPositionNode(const Vector3D& targetPos, const Vector3D& objectPositoin, uint32_t closestNum)
   {
	   struct LengeData
	   {
		   Node* node;
		   float lenge;
	   };

		Node* result = nullptr;
		Vector3D localTargetPos = targetPos - objectPositoin;//Mesh空間に変換
		list<LengeData> lenges;
		for (auto& node : nodes_)
		{
			Vector3D pointVec = pointVec.Vector3Substruct(node->startPosition.vec_, localTargetPos.vec_);
			float lenge =pointVec.V3Len() - node->boneVec.GetV3Dot(pointVec);//Boneのベクトルから最も近い位置を算出
			LengeData temp;
			temp.node = node.get();
			temp.lenge = lenge;
			lenges.push_back(temp);
		}

		lenges.sort([](auto const& lhs, auto const rhs) {return lhs.lenge < rhs.lenge; });//近い順でソート(closestNumで検索掛けるため)
		int32_t i = 1;//添え字ではなく"何番目か"を指定するので1番目を最初に(0番を最初にするかは思案中)
		for (auto itr = lenges.begin(); itr != lenges.end(); ++itr)
		{
			if (i == closestNum)//完全一致しない場合はnullptr(ずれた値出るとまずいため）
			{
				result = itr->node;
			}
			i++;
		}
		return result;
   }

   void MCB::Node::JointObjectMatrixUpdate(ICamera* camera, Object3d* Obj,Model* model,const Float3& scale)
   {
	   if (!object->parent_)
	   {
		   object->parent_ = Obj;
		   object->scale_.x = scale.x_;
		   object->scale_.y = scale.y_;
		   object->scale_.z = scale.z_;
	   }
	   object->model_ = model;
	   object->camera_ = camera;
	   object->rotationQ_.x_ = rotation.m128_f32[0]; 
	   object->rotationQ_.y_ = rotation.m128_f32[1]; 
	   object->rotationQ_.z_ = rotation.m128_f32[2]; 
	   object->rotationQ_.w_ = rotation.m128_f32[3];
	   object->scale_ = { 1.f,1.f,1.f };
	   object->position_.x = translation.m128_f32[0];
	   object->position_.y = translation.m128_f32[1];
	   object->position_.z = translation.m128_f32[2];

	   boneLine.line.parent_ = object.get()->parent_;
	   boneLine.line.camera_ = camera;
	   if (lineColorEqualObject)boneLine.line.color_ = object->color_;
	   else boneLine.line.color_ = lineDefaultColor;
	   boneLine.PointA_ = { 0,0,0 };
	   boneLine.PointB_ = { object->position_.x,object->position_.y,object->position_.z };

	   object->Update(object->rotationQ_);

	   if (ikData.isIK)
	   {
		   ikData.constraintObj.position_.x = ikData.constraintWorldVector.vec_.x_;
		   ikData.constraintObj.position_.y = ikData.constraintWorldVector.vec_.y_;
		   ikData.constraintObj.position_.z = ikData.constraintWorldVector.vec_.z_;
		   ikData.constraintLine.line.parent_ = ikData.rootJointNode->object.get();
		   ikData.constraintLine.PointA_ = { 0,0,0 };
		   ikData.constraintLine.PointB_.x_ = ikData.constraintLocalPositionFromRoot.vec_.x_;
		   ikData.constraintLine.PointB_.y_ = ikData.constraintLocalPositionFromRoot.vec_.y_;
		   ikData.constraintLine.PointB_.z_ = ikData.constraintLocalPositionFromRoot.vec_.z_;
		   if (ikData.rootJointNode->lineColorEqualObject)
		   {
			   ikData.constraintLine.line.color_ = ikData.rootJointNode->object->color_;
		   }
		   else ikData.constraintLine.line.color_ = lineDefaultColor;


		   ikData.effectorVecFromRoot.line.parent_ = ikData.rootJointNode->object.get();
		   ikData.effectorVecFromRoot.PointA_ = { 0,0,0 };
		   ikData.effectorVecFromRoot.PointB_.x_ = ikData.effectorPosFromRoot.vec_.x_;
		   ikData.effectorVecFromRoot.PointB_.y_ = ikData.effectorPosFromRoot.vec_.y_;
		   ikData.effectorVecFromRoot.PointB_.z_ = ikData.effectorPosFromRoot.vec_.z_;
		   if (ikData.rootJointNode->lineColorEqualObject)
		   {
			   ikData.effectorVecFromRoot.line.color_ = ikData.rootJointNode->object->color_;
		   }
		   else ikData.effectorVecFromRoot.line.color_ = lineDefaultColor;


		   ikData.effectorVecFromMiddle.line.parent_ = ikData.middleJointNode->object.get();
		   ikData.effectorVecFromMiddle.PointA_ = { 0,0,0 };
		   ikData.effectorVecFromMiddle.PointB_.x_ = ikData.effectorPosFromMiddle.vec_.x_;
		   ikData.effectorVecFromMiddle.PointB_.y_ = ikData.effectorPosFromMiddle.vec_.y_;
		   ikData.effectorVecFromMiddle.PointB_.z_ = ikData.effectorPosFromMiddle.vec_.z_;
		   if (ikData.rootJointNode->lineColorEqualObject)
		   {
			   ikData.effectorVecFromMiddle.line.color_ = ikData.rootJointNode->object->color_;
		   }
		   else ikData.effectorVecFromMiddle.line.color_ = lineDefaultColor;


		   ikData.constraintObj.scale_ = { 0.5f,0.5f,0.5f };
		   ikData.constraintObj.model_ = model;
		   ikData.constraintObj.color_ = { 0.5,0,0,1 };
		   ikData.constraintObj.camera_ = camera;
		   ikData.constraintObj.Update();


		   ikData.rootAndEffectorAndConstraintTriangle.triangle_.parent_ = ikData.rootJointNode->object.get();
		   ikData.jointTriangle.triangle_.parent_ = ikData.rootJointNode->object.get();
		   ikData.rootAndEffectorAndConstraintTriangle.triangle_.color_ = ikData.rootJointNode->object->color_;
		   ikData.rootAndEffectorAndConstraintTriangle.triangle_.color_.w_ = 0.25f;
		   ikData.jointTriangle.triangle_.color_ = ikData.rootJointNode->object->color_;
		   ikData.jointTriangle.triangle_.color_.w_ = 0.25f;
	   }
	   //object->matWorld_.matWorld_ *= worldObjMatrix;
   }

   void MCB::Node::JointObjectDraw()
   {
	   //object->Draw();

	   if (ikData.isIK)
	   {
		   ikData.constraintObj.Draw();
		   //ikData.rootAndEffectorAndConstraintTriangle.DrawTriangle(object->camera_);
		   //ikData.jointTriangle.DrawTriangle(object->camera_);
	   }
   }
   void MCB::Node::JointLineDraw()
   {
	   boneLine.DrawLine(object->camera_);
	   if (ikData.isIK)
	   {
		   ikData.constraintLine.DrawLine(object->camera_);
		   ikData.effectorVecFromRoot.DrawLine(object->camera_);
		   ikData.effectorVecFromMiddle.DrawLine(object->camera_);
	   }
   }