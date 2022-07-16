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
            Hip,
            LegL,
            LegR
        };
        std::array<Object3d, 9> Box;

        void Init(Model* model);
        void UpDate(View view, Projection proj);
        void Draw();
    };

}

