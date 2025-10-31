#include <core/Timer.h>
#include<iostream>
#include<graphics.h>
#include<glfw3.h>
#include<thread>

void Timer::Init()
{
	std::cout << "Timer Init..." << std::endl;
    frame_start = std::chrono::high_resolution_clock::now();
}

void Timer::Start_Frame()
{
    frame_start = std::chrono::high_resolution_clock::now();
}

void Timer::End_frame()
{
    auto frame_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float>diff = frame_end - frame_start;
    delta_time= diff.count();
    float sleeptime = FRAMEDURATION - delta_time;
   
    if (sleeptime > 0.0f) {
        std::this_thread::sleep_for(std::chrono::duration<float>(sleeptime));
		delta_time = FRAMEDURATION; // fix delta_time to target frame duration
    }
    else 
    {
		// if frame took longer than target duration, just log it
        std::cout << "Frame took too long: " << delta_time * 1000 << " ms" << std::endl;
    }
    std::cout << delta_time*1000 << std::endl;
    duration += delta_time;
    fps++;

    if (duration >= 1.0f) {
        std::cout << "FPS: " << fps << " Delta Time: " << delta_time * 1000 << " ms" <<",duration:" << duration << std::endl;
        duration = duration-1.0f;
        fps = 0;
    }
}

float Timer::Get_Delta() const 
{
    return delta_time;
}

float Timer::Get_framecount() const
{
    return duration;
}

unsigned int Timer::Get_fps() const
{
    return fps;
}

void Timer::Reset()
{
    frame_start = std::chrono::high_resolution_clock::now();
    delta_time = 0;
}
