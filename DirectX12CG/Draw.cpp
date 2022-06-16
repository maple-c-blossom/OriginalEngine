#include "Draw.h"

void MCB::Draw::AfterDraw(Dx12 &dx12, Depth depth, ShaderResource srv, PipelineRootSignature pipeline)
{
    dx12.commandList->SetPipelineState(pipeline.pipeline.pipelinestate.Get());
    dx12.commandList->SetGraphicsRootSignature(pipeline.rootsignature.rootsignature.Get());


    //プリミティブ形状の設定コマンド（三角形リスト）--------------------------
    dx12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //SRVヒープの設定コマンド
    dx12.commandList->SetDescriptorHeaps(1, srv.srvHeap.GetAddressOf());
}
