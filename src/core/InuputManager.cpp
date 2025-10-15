#include "core/InuputManager.h"

InputManager& InputManager::Instance() {
    static InputManager inst;
    return inst;
}

bool InputManager::Init_Inputmgr(GLFWwindow* window) {
    if (!window) return 1;
    // �� this �󶨵� window �� user pointer���ص���ȡ��
    glfwSetWindowUserPointer(window, this);

    // ע��ص�������������ע����������Ҫ�Ļص���
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);
}

void InputManager::Shutdown(GLFWwindow* window) {
    if (!window) return;
    // ���ص�����ѡ�����Ƽ������� window ǰ�����
    glfwSetKeyCallback(window, nullptr);
    glfwSetMouseButtonCallback(window, nullptr);
    glfwSetCursorPosCallback(window, nullptr);
    glfwSetScrollCallback(window, nullptr);
    glfwSetWindowUserPointer(window, nullptr);

    // ����״̬
    keyStates.clear();
    mouseButtonsDown.clear();
    cursorX = cursorY = 0.0;
}

void InputManager::Update(double currentTime) {
    // �����������ڰ��¼��ĳ���ʱ��
        for(auto& i:keyStates) {
            if (i.second.pressed) i.second.duration = currentTime - i.second.downTime;
            else i.second.duration = 0.0;
        }
}

// �Ǿ�̬���������ɾ�̬�ص�ת����
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
            // ��ѡ������ѡ��erase�Խ�ʡ�ռ䣺 keyStates.erase(it);
        }
    }
}

void InputManager::OnMouseButton(int button, int action, int mods) {
    double now = glfwGetTime();
    if (action == GLFW_PRESS) {
        mouseButtonsDown.insert(button);
        // ����Ҫ��¼����ʱ��������һ�� mouse button map ����������
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
    // ������Ҫ��¼���ֻ򴥷��¼�
}

// ��̬�ص���ȡ�� window �� user pointer ת����ʵ��
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

// ��ȡ״̬�ļ�ʵ��
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
