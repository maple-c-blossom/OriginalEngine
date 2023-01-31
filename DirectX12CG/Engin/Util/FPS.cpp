#include "FPS.h"

using namespace MCB;
using namespace std::chrono;
using namespace std;

FPS* MCB::FPS::GetInstance()
{
    static FPS* instance = new FPS;
    return instance;
}

FPS* MCB::FPS::GetInitInstance()
{
    static FPS* instance = FPS::GetInstance();
    instance->Initialize();
    return instance;
}

void MCB::FPS::DeleteInstance()
{
    delete FPS::GetInstance();
}

void MCB::FPS::Initialize()
{
    reference = steady_clock::now();
}

void MCB::FPS::UpDate()
{
    const microseconds minTime(uint16_t(1000000.0f / 60.f));
    const microseconds minCheckTime(uint16_t(1000000.0f / 65.f));
    steady_clock::time_point now = steady_clock::now();
    microseconds elapsed = duration_cast<microseconds>(now - reference);
    if (elapsed < minCheckTime)
    {
        while (steady_clock::now() - reference < minTime) this_thread::sleep_for(microseconds(1));

    }
    reference = steady_clock::now();
}
