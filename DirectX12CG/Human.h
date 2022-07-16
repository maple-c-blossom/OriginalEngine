#pragma once
#include "Object3d.h"
#include <array>

namespace MCB
{
    class Human
    {
    public:
        enum PartId
        {
            Root,
            Spine,
            Chest,
            Head,
            ArmL,
            ArmR,
            HandL,
            HandR,
            Hip,
            LegL,
            LegR,
            FootL,
            FootR,
        };
        std::array<Object3d, 13> Box;

        void Init(Model* model);
        void UpDate(View view, Projection proj);
        void Draw();
    };

}

