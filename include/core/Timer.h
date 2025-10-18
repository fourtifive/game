#pragma once
#include<chrono>
const unsigned int FPS = 144;
class Timer
{
public:
	Timer();
	float Updata();

	float Get_Delta() const;

	float Get_framecount()const;

	float Get_fps()const;

	void Reset();
private:
	std::chrono::high_resolution_clock::time_point last_time;
	float delta_time;
	unsigned int fps = 0;
	float framecount = 0;
};