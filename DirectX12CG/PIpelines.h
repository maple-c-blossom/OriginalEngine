#pragma once
#include "Dx12.h"
#include "PIpelineRootSignature.h"
#include "RootParameter.h"
namespace MCB
{

    namespace Pipeline
    {
        //static PipelineRootSignature obj3dPipeline[4] = { obj3dPipeline[Alpha].Create3DObjectPipeline(depth, rootparams,Alpha),
        //                                                  obj3dPipeline[Add].Create3DObjectPipeline(depth, rootparams,Add),
        //                                                  obj3dPipeline[Sub].Create3DObjectPipeline(depth,rootparams,Sub),};
        //static PipelineRootSignature spritePipeline = spritePipeline.CreateSpritePipeline(depth, rootparams);
        //static PipelineRootSignature particlePipeline = particlePipeline.CreateParticlePipeline(depth, rootparams);
        //inline void SetObj3dPipeline(int blendMode)
        //{
        //    switch (blendMode)
        //    {
        //    case Alpha:
        //        pipeline.SetAlphaBlend();
        //        break;
        //    case Add:
        //        pipeline.SetAddBlend();
        //        break;
        //    case Sub:
        //        pipeline.SetSubBlend();
        //        break;
        //    case Inv:
        //        pipeline.SetInvBlend();
        //        break;
        //    default:
        //        pipeline.SetAlphaBlend();
        //        break;
        //    }
        //}
    }
}