#include "FBXModel.h"
#include "Util.h"
using namespace MCB;
using namespace std;

void MCB::ParseNodeRecursive(FBXModel* model, FbxNode* node)
{
	//�m�[�h���擾
	string name = node->GetName();
	//���f���Ƀm�[�h��ǉ�
	model->nodes.emplace_back();
	Node& tempNode = model->nodes.back();
	//�m�[�h�����擾
	tempNode.name = node->GetName();
	//FBX�m�[�h���[�J���ړ����
	FbxDouble3 rota = node->LclRotation.Get();
	FbxDouble3 scale = node->LclScaling.Get();
	FbxDouble3 trans = node->LclTranslation.Get();
	//�`���ϊ�����
	tempNode.rotation.vec = { (float)rota[0],(float)rota[1],(float)rota[2] };
	tempNode.scale.vec = { (float)scale[0],(float)scale[1],(float)scale[2] };
	tempNode.translasion.vec = { (float)trans[0],(float)trans[1],(float)trans[2] };
	//��]�p�����W�A���ɕϊ�
	tempNode.rotation.vec.x = ConvertRadius(tempNode.rotation.vec.x);
	tempNode.rotation.vec.y = ConvertRadius(tempNode.rotation.vec.y);
	tempNode.rotation.vec.z = ConvertRadius(tempNode.rotation.vec.z);
	//�e��s��̌v�Z
	DirectX::XMMATRIX scaleMat, transMat, rotaMat;
	//scaleMat = DirectX::XMMatrixScalingFromVector(DirectX::XMVECTOR{ tempNode.scale.vec.x, tempNode.scale.vec.y, tempNode.scale.vec.z });

	//FbxNode�̏�����͂��ċL�^
	
	//FbxNode�̃��b�V���������

	//�q�m�[�h�ɑ΂��čċN�Ăяo��
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		ParseNodeRecursive(model, node->GetChild(i));
	}
}
