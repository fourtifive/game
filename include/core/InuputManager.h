#pragma once
#include<glfw3.h>
#include<unordered_set>
#include<unordered_map>

struct Input_State
{
	bool is_press=0;
	float duration = 0.0f;
};

class InputManager
{
public:
	InputManager()=default; 
	~InputManager()=default;

	bool Init();


	static void key_callback(GLFWwindow* window);
	static void cursor_callback(GLFWwindow* window);



private:
	std::unordered_set<int> cursor;
	std::unordered_map<int, Input_State> key;
	//corsor's position;

};