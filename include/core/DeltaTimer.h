#pragma once
#include<chrono>

class DeltaTimer
{
public:
	DeltaTimer();
	float Updata();

	float Get_Delta() const;

	void Reset();
private:
	std::chrono::high_resolution_clock::time_point last_time;
	float delta_time;
};