#pragma once
#include<chrono>
const float FPS =144;
const float FRAMEDURATION = 1.0f / FPS;
class Timer
{
public:
	Timer()=default;
	//float Updata();
	void Init();

	void Start_Frame();

	void End_frame();

	float Get_Delta() const;

	float Get_framecount()const;

	unsigned int Get_fps()const;

	void Reset();
private:
	/*std::chrono::high_resolution_clock::time_point last_time;*/
	float delta_time=0.0f;
	unsigned int fps = 1;
	float duration = 0.0f;
	int count = 0;
	std::chrono::high_resolution_clock::time_point frame_start;
};