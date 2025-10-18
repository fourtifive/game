#include <core/Timer.h>
#include<iostream>


Timer::Timer()
{
    last_time = std::chrono::high_resolution_clock::now();
}

float Timer::Updata()
{
    auto current_time=std::chrono::high_resolution_clock::now();
    std::chrono::duration<float>diff =  current_time-last_time;
    delta_time = diff.count();
    last_time = current_time;
    if (framecount > 1)
    {
        std::cout << "FPS:"<<fps<<" "<<delta_time<<std::endl;
        framecount = 0, fps = 0;
    }
    fps += 1;
    framecount += delta_time;
    return delta_time;
}

float Timer::Get_Delta() const
{
    return delta_time;
}

float Timer::Get_framecount() const
{
    return framecount;
}

float Timer::Get_fps() const
{
    return fps;
}

void Timer::Reset()
{
    last_time = std::chrono::high_resolution_clock::now();
    delta_time = 0;
}
