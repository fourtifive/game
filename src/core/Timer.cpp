#include <core/Timer.h>
#include<iostream>
#include<graphics.h>
#include<glfw3.h>

void Timer::Init()
{
	std::cout << "Timer Init..." << std::endl;
    frame_start = std::chrono::high_resolution_clock::now();
}

//float Timer::Updata()
//{
//    auto current_time=std::chrono::high_resolution_clock::now();
//    std::chrono::duration<float>diff =  current_time-last_time;
//    delta_time = diff.count();
//    last_time = current_time;
//    return delta_time;
//}

void Timer::Start_Frame() 
{

    frame_start = std::chrono::high_resolution_clock::now();
    //glfwPollEvents();
}

void Timer::End_frame()
{
    auto frame_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float>diff = frame_end - frame_start;
    float frameduration = diff.count();
    delta_time = frameduration;
    float sleeptime = FRAMEDURATION*1000 - frameduration*1000;
   
    if (sleeptime >= 0) {
        Sleep(sleeptime);
        duration += FRAMEDURATION;
        count++;
        //std::cout << count<<" :"<<duration<<","<<frameduration*1000<<","<<sleeptime<< std::endl;
        if (count == 144)count = 0;
    }
    else duration += delta_time;
    
    fps++;

    if (duration >= 1.0f||fps==144)
    {
        std::cout << "FPS:" << fps << " " << delta_time*1000 << " " << duration<<" " <<sleeptime<< std::endl;
        duration = 0.0f, fps = 0;
    }
    
}

float Timer::Get_Delta() const 
{
  /*  auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float>diff = current_time - last_time;
    delta_time = diff.count();
    last_time = current_time;*/

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
