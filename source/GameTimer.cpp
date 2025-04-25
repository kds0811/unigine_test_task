#include "GameTimer.h"
#include "Windows.h"

GameTimer::GameTimer() : SecondsPerCount(0.0), DeltaTime(-1.0), BaseTime(0), PausedTime(0), PrevTime(0), CurrTime(0), bStopped(false)
{
    __int64 countsPerSec;
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    SecondsPerCount = 1.0 / (double)countsPerSec;
}

float GameTimer::GetTotalTime() const noexcept
{
    if (bStopped)
    {
        return static_cast<float>(((StopTime - PausedTime) - BaseTime) * SecondsPerCount);
    }
    else
    {
        return static_cast<float>(((CurrTime - PausedTime) - BaseTime) * SecondsPerCount);
    }
}

void GameTimer::Reset() noexcept
{
    __int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
    CurrTime = currTime;
    BaseTime = currTime;
    PrevTime = currTime;
    StopTime = 0;
    bStopped = false;
    DeltaTime = 0.0;
}

void GameTimer::Start() noexcept
{
    __int64 startTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
    if (bStopped)
    {
        PausedTime += (startTime - StopTime);
        PrevTime = startTime;
        StopTime = 0;
        bStopped = false;
    }
}

void GameTimer::Stop() noexcept
{
    if (!bStopped)
    {
        __int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
        StopTime = currTime;
        bStopped = true;
        DeltaTime = 0.0;
    }
}

void GameTimer::Tick() noexcept
{
    if (bStopped)
    {
        DeltaTime = 0.0;
        return;
    }

    __int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
    CurrTime = currTime;

    DeltaTime = (CurrTime - PrevTime) * SecondsPerCount;

    PrevTime = CurrTime;

    if (DeltaTime < 0.0)
    {
        DeltaTime = 0.0;
    }
}
