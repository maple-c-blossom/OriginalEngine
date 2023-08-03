#include "Util.h"
#include <stdlib.h>
#include <time.h>

using namespace MCB;
using namespace PrimitiveFigure;
double MCB::Lerp(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return change * time + startPos;
}

double MCB::Lerp(double startPos, double endPos, double time)
{
	double change = endPos - startPos;
	return change * time + startPos;
}
double MCB::InQuad(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return change * time * time + startPos;
}
double MCB::OutQuad(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return -change * time * (time - 2) + startPos;
}
double MCB::InOutQuad(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime / 2;
	double change = endPos - startPos;
	if (time < 1) return change / 2 * time * time + startPos;
	return -change / 2 * ((--time) * (time - 2) - 1) + startPos;
}
double MCB::OutInRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time)
{
	if (time < relayTime)
	{
		return OutQuad(relayPos, startPos, relayTime, time);
	}
	else
	{
		return InQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}
double MCB::OutOutRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time)
{
	if (time < relayTime)
	{
		return OutQuad(relayPos, startPos, relayTime, time);
	}
	else
	{
		return OutQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}
double MCB::EaseInBack(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	double c1 = 1.70158;
	double c3 = c1 + 1;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
double MCB::EaseInBack2(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	double c1 = 1.2;
	double c3 = c1 + 0.8;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
double MCB::EaseOutBounce(double startPos, double endPos, double time, double maxTime)
{
	time /= maxTime;
	double change = endPos - startPos;
	double n1 = 7.5625;
	double d1 = 2.75;

	if (time < 1 / d1)
	{
		return n1 * time * time + startPos + change;
	}
	else if (time < 2 / d1)
	{
		return n1 * (time -= 1.5 / d1) * time + 0.75 + startPos + change;
	}
	else if (time < 2.5 / d1)
	{
		return n1 * (time -= 2.25 / d1) * time + 0.9375 + startPos + change;
	}
	else
	{
		return n1 * (time -= 2.625 / d1) * time + 0.984375 + startPos + change;
	}
}

int32_t MCB::Abs(int32_t num)
{
	if (num < 0)
	{
		return num * -1;
	}
	return num;
}

float MCB::Abs(float num)
{
	if (num < 0)
	{
		return num * -1;
	}
	return num;
}

double MCB::Abs(double num)
{
	if (num < 0)
	{
		return num * -1;
	}
	return num;
}

float MCB::ConvertRadius(float angle)
{
	return angle * PI / 180.0f;
}

void MCB::InitRand()
{
	srand((static_cast<uint32_t>(time(nullptr))));
}

int32_t MCB::GetRand(int32_t min, int32_t max)
{
	return min + rand() % (max - min);
}

float MCB::clamp(float f)
{
	return (f < 0.0f) ? 0.0f : ((f > 1.0f) ? 1.0f : f);
}

float MCB::clamp(float f,float min,float max)
{
	return (f < min) ? min : ((f > max) ? max : f);
}
Triangle::Triangle()
{

	triangle_.Init();
	triangle_.model_ = &triangleMaterial_;
	triangleMaterial_.vertices_ = {
		{PointA_,{1,1,1},{0,0}},
		{PointB_,{1,1,1},{0,0}},
		{PointC_,{1,1,1},{0,0}}
	};
	triangleMaterial_.SetSizeVB();
	triangleMaterial_.material_.SetVertexBuffer(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, static_cast<uint32_t>(triangleMaterial_.sizeVB_), 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
	triangleMaterial_.CreateVertexBuffer(triangleMaterial_.material_.HeapProp_, D3D12_HEAP_FLAG_NONE, triangleMaterial_.material_.Resdesc_, D3D12_RESOURCE_STATE_GENERIC_READ);
	triangleMaterial_.VertexMaping();
	triangleMaterial_.SetVbView();
	triangle_.model_->texture_ = triangle_.model_->loader_->CreateNoTextureFileIsTexture();
}

void Triangle::DrawTriangle(ICamera* camera)
{

	Dx12* dx12 = Dx12::GetInstance();
	ShaderResource* descriptor = ShaderResource::GetInstance();

	triangleMaterial_.vertices_ = {
		{PointA_,{1,1,1},{0,0}},
		{PointB_,{1,1,1},{0,0}},
		{PointC_,{1,1,1},{0,0}}
	};

	triangleMaterial_.VertexMaping();



	triangle_.Update(camera);

	//定数バッファビュー(CBV)の設定コマンド
	dx12->commandList_->SetGraphicsRootConstantBufferView(2, triangleMaterial_.material_.constBuffMaterialB1_->GetGPUVirtualAddress());

	//SRVヒープの先頭アドレスを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap_->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle.ptr += triangle_.model_->texture_->texture->incrementNum_ * dx12->device_.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc_.Type);
	//SRVヒープの先頭にあるSRVをパラメータ1番に設定
	dx12->commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//頂点データ
	dx12->commandList_->IASetVertexBuffers(0, 1, &triangleMaterial_.vbView_);
	//定数バッファビュー(CBV)の設定コマンド
	dx12->commandList_->SetGraphicsRootConstantBufferView(0, triangle_.GetConstBuffTrans()->GetGPUVirtualAddress());
	//描画コマンド
	dx12->commandList_->DrawInstanced((uint32_t)triangleMaterial_.vertices_.size(), 1, 0, 0);
}

Line::Line()
{

	line.Init();
	line.model_ = &lineMaterial;
	lineMaterial.vertices_ = {
		{PointA_,{1,1,1},{0,0}},
		{PointB_,{1,1,1},{0,0}},
	};
	lineMaterial.SetSizeVB();
	lineMaterial.material_.SetVertexBuffer(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, static_cast<uint32_t>(lineMaterial.sizeVB_), 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
	lineMaterial.CreateVertexBuffer(lineMaterial.material_.HeapProp_, D3D12_HEAP_FLAG_NONE, lineMaterial.material_.Resdesc_, D3D12_RESOURCE_STATE_GENERIC_READ);
	lineMaterial.VertexMaping();
	lineMaterial.SetVbView();
	line.model_->texture_ = line.model_->loader_->CreateNoTextureFileIsTexture();
}

void Line::DrawLine(ICamera* camera)
{

	Dx12* dx12 = Dx12::GetInstance();
	ShaderResource* descriptor = ShaderResource::GetInstance();

	lineMaterial.vertices_ = {
		{PointA_,{1,1,1},{0,0}},
		{PointB_,{1,1,1},{0,0}},
		
	};

	lineMaterial.VertexMaping();



	line.Update(camera);

	//定数バッファビュー(CBV)の設定コマンド
	dx12->commandList_->SetGraphicsRootConstantBufferView(2, lineMaterial.material_.constBuffMaterialB1_->GetGPUVirtualAddress());

	//SRVヒープの先頭アドレスを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap_->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle.ptr += line.model_->texture_->texture->incrementNum_ * dx12->device_.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc_.Type);
	//SRVヒープの先頭にあるSRVをパラメータ1番に設定
	dx12->commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//頂点データ
	dx12->commandList_->IASetVertexBuffers(0, 1, &lineMaterial.vbView_);
	//定数バッファビュー(CBV)の設定コマンド
	dx12->commandList_->SetGraphicsRootConstantBufferView(0, line.GetConstBuffTrans()->GetGPUVirtualAddress());
	//描画コマンド
	dx12->commandList_->DrawInstanced((uint32_t)lineMaterial.vertices_.size(), 1, 0, 0);
}
