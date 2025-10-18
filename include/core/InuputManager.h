#pragma once
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <glfw3.h>

struct InputState {
    bool pressed = false;
    double downTime = 0.0;    // glfwGetTime() when pressed
    double duration = 0.0;    // updated in Update()
};

class InputManager {
public:
    static InputManager& Instance();

    // 初始化，负责注册回调。传入 GLFWwindow*（非空）
    bool Init_Inputmgr(GLFWwindow* window);

    // 注销回调并清理（在 glfwDestroyWindow 或退出前调用）
    void Shutdown(GLFWwindow* window);

    // per-frame 更新（计算 duration）
    void Update(double currentTime);

    // 查询接口
    bool IsKeyDown(int key) const;
    double GetKeyDownDuration(int key, double currentTime) const;

    bool IsMouseButtonDown(int button) const;
    std::pair<double, double> GetCursorPos() const;

    InputManager()=default;
    ~InputManager()=default;

private:
   

    // 非静态实例方法：回调转发到这些方法来修改状态
    void OnKey(int key, int scancode, int action, int mods);
    void OnMouseButton(int button, int action, int mods);
    void OnCursorPos(double xpos, double ypos);
    void OnScroll(double xoffset, double yoffset);

    // GLFW回调必须是静态或自由函数，这里用静态转发
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    std::unordered_map<int, InputState> keyStates;
    std::unordered_set<int> mouseButtonsDown;
    double cursorX = 0.0, cursorY = 0.0;
};