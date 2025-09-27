#include <core/Timer.h>



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

    return delta_time;
}

float Timer::Get_Delta() const
{
    return delta_time;
}

void Timer::Reset()
{
    last_time = std::chrono::high_resolution_clock::now();
    delta_time = 0;
}
