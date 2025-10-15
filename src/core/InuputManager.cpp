#include "core/InuputManager.h"

InputManager& InputManager::Instance() {
    static InputManager inst;
    return inst;
}

bool InputManager::Init_Inputmgr(GLFWwindow* window) {
    if (!window) return 1;
    // 把 this 绑定到 window 的 user pointer，回调中取回
    glfwSetWindowUserPointer(window, this);

    // 注册回调（可以在这里注册所有你需要的回调）
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);
}

void InputManager::Shutdown(GLFWwindow* window) {
    if (!window) return;
    // 解绑回调（可选，但推荐在销毁 window 前解除）
    glfwSetKeyCallback(window, nullptr);
    glfwSetMouseButtonCallback(window, nullptr);
    glfwSetCursorPosCallback(window, nullptr);
    glfwSetScrollCallback(window, nullptr);
    glfwSetWindowUserPointer(window, nullptr);

    // 清理状态
    keyStates.clear();
    mouseButtonsDown.clear();
    cursorX = cursorY = 0.0;
}

void InputManager::Update(double currentTime) {
    // 更新所有正在按下键的持续时间
        for(auto& i:keyStates) {
            if (i.second.pressed) i.second.duration = currentTime - i.second.downTime;
            else i.second.duration = 0.0;
        }
}

// 非静态处理函数（由静态回调转发）
void InputManager::OnKey(int key, int scancode, int action, int mods) {
    double now = glfwGetTime();
    if (action == GLFW_PRESS) {
        auto& s = keyStates[key];
        s.pressed = true;
        s.downTime = now;
        s.duration = 0.0;
    }
    else if (action == GLFW_RELEASE) {
        auto it = keyStates.find(key);
        if (it != keyStates.end()) {
            it->second.pressed = false;
            it->second.duration = now - it->second.downTime;
            // （选）可以选择erase以节省空间： keyStates.erase(it);
        }
    }
}

void InputManager::OnMouseButton(int button, int action, int mods) {
    double now = glfwGetTime();
    if (action == GLFW_PRESS) {
        mouseButtonsDown.insert(button);
        // 若需要记录按下时长，可用一个 mouse button map 与上面类似
    }
    else if (action == GLFW_RELEASE) {
        mouseButtonsDown.erase(button);
    }
}

void InputManager::OnCursorPos(double xpos, double ypos) {
    cursorX = xpos;
    cursorY = ypos;
}

void InputManager::OnScroll(double xoffset, double yoffset) {
    // 根据需要记录滚轮或触发事件
}

// 静态回调：取回 window 的 user pointer 转发给实例
void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    void* ptr = glfwGetWindowUserPointer(window);
    if (ptr) reinterpret_cast<InputManager*>(ptr)->OnKey(key, scancode, action, mods);
}
void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    void* ptr = glfwGetWindowUserPointer(window);
    if (ptr) reinterpret_cast<InputManager*>(ptr)->OnMouseButton(button, action, mods);
}
void InputManager::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    void* ptr = glfwGetWindowUserPointer(window);
    if (ptr) reinterpret_cast<InputManager*>(ptr)->OnCursorPos(xpos, ypos);
}
void InputManager::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    void* ptr = glfwGetWindowUserPointer(window);
    if (ptr) reinterpret_cast<InputManager*>(ptr)->OnScroll(xoffset, yoffset);
}

// 读取状态的简单实现
bool InputManager::IsKeyDown(int key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second.pressed;
}
double InputManager::GetKeyDownDuration(int key, double currentTime) const {
    auto it = keyStates.find(key);
    if (it != keyStates.end() && it->second.pressed)
        return currentTime - it->second.downTime;
    return 0.0;
}
bool InputManager::IsMouseButtonDown(int button) const {
    return mouseButtonsDown.find(button) != mouseButtonsDown.end();
}
std::pair<double, double> InputManager::GetCursorPos() const {
    return { cursorX, cursorY };
}

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}
