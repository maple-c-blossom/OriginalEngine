#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <chrono>
#include <thread>
WarningIgnoreEnd
namespace MCB
{
	class FPS
	{
    private:
        FPS();
        FPS(const FPS& );
        FPS& operator=(const FPS& );
        ~FPS();
        std::chrono::steady_clock::time_point reference_;
    public:
        static FPS* GetInstance();
        static FPS* GetInitInstance();
        static void DeleteInstance();
		void Initialize();
		void UpDate();
	};
}

