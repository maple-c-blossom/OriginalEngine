#include "FBXModel.h"
#include "Util.h"
using namespace MCB;
using namespace std;

void MCB::ParseNodeRecursive(FBXModel* model, FbxNode* node)
{
	//ノード名取得
	string name = node->GetName();
	//モデルにノードを追加
	model->nodes.emplace_back();
	Node& tempNode = model->nodes.back();
	//ノード名を取得
	tempNode.name = node->GetName();
	//FBXノードローカル移動情報
	FbxDouble3 rota = node->LclRotation.Get();
	FbxDouble3 scale = node->LclScaling.Get();
	FbxDouble3 trans = node->LclTranslation.Get();
	//形式変換後代入
	tempNode.rotation.vec = { (float)rota[0],(float)rota[1],(float)rota[2] };
	tempNode.scale.vec = { (float)scale[0],(float)scale[1],(float)scale[2] };
	tempNode.translasion.vec = { (float)trans[0],(float)trans[1],(float)trans[2] };
	//回転角をラジアンに変換
	tempNode.rotation.vec.x = ConvertRadius(tempNode.rotation.vec.x);
	tempNode.rotation.vec.y = ConvertRadius(tempNode.rotation.vec.y);
	tempNode.rotation.vec.z = ConvertRadius(tempNode.rotation.vec.z);
	//各種行列の計算
	DirectX::XMMATRIX scaleMat, transMat, rotaMat;
	//scaleMat = DirectX::XMMatrixScalingFromVector(DirectX::XMVECTOR{ tempNode.scale.vec.x, tempNode.scale.vec.y, tempNode.scale.vec.z });

	//FbxNodeの情報を解析して記録
	
	//FbxNodeのメッシュ情報を解析

	//子ノードに対して再起呼び出し
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		ParseNodeRecursive(model, node->GetChild(i));
	}
}
