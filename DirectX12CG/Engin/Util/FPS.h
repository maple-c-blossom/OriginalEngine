#pragma once
#include <chrono>
#include <thread>

namespace MCB
{
	class FPS
	{
    private:
        FPS() {};
        FPS(const FPS& dx12) {};
        FPS& operator=(const FPS& dx12) {};
        ~FPS() {};
        std::chrono::steady_clock::time_point reference;
    public:
        static FPS* GetInstance();
        static FPS* GetInitInstance();
        static void DeleteInstance();
		void Initialize();
		void UpDate();
	};
}

