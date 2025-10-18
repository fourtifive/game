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

    // ��ʼ��������ע��ص������� GLFWwindow*���ǿգ�
    bool Init_Inputmgr(GLFWwindow* window);

    // ע���ص��������� glfwDestroyWindow ���˳�ǰ���ã�
    void Shutdown(GLFWwindow* window);

    // per-frame ���£����� duration��
    void Update(double currentTime);

    // ��ѯ�ӿ�
    bool IsKeyDown(int key) const;
    double GetKeyDownDuration(int key, double currentTime) const;

    bool IsMouseButtonDown(int button) const;
    std::pair<double, double> GetCursorPos() const;

    InputManager()=default;
    ~InputManager()=default;

private:
   

    // �Ǿ�̬ʵ���������ص�ת������Щ�������޸�״̬
    void OnKey(int key, int scancode, int action, int mods);
    void OnMouseButton(int button, int action, int mods);
    void OnCursorPos(double xpos, double ypos);
    void OnScroll(double xoffset, double yoffset);

    // GLFW�ص������Ǿ�̬�����ɺ����������þ�̬ת��
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    std::unordered_map<int, InputState> keyStates;
    std::unordered_set<int> mouseButtonsDown;
    double cursorX = 0.0, cursorY = 0.0;
};