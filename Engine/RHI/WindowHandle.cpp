#include "RHI/WindowHandle.h"
#include "Core/Console.h"

void WindowHandle::InitializeGLFW()
{
    RHI_CONDITION_FATAL(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW");
}

void WindowHandle::TerminateGLFW()
{
    glfwTerminate();
}

bool WindowHandle::ValidMode(int flags)
{
    int target = WindowHandle_WindowedModeBit | WindowHandle_BorderlessModeBit |
                 WindowHandle_FullscreenModeBit;
    int enabled = target & flags;
    return enabled == 0 || enabled == WindowHandle_WindowedModeBit ||
           enabled == WindowHandle_BorderlessModeBit || enabled == WindowHandle_FullscreenModeBit;
}

bool WindowHandle::Closing() const
{
    return glfwWindowShouldClose(WindowPtr);
}

const std::string_view WindowHandle::Title() const
{
    return std::string_view(glfwGetWindowTitle(WindowPtr));
}

glm::ivec2 WindowHandle::FramebufferSize() const
{
    glm::ivec2 buffer_size;
    glfwGetFramebufferSize(WindowPtr, &buffer_size.x, &buffer_size.y);
    return buffer_size;
}

glm::ivec2 WindowHandle::Size() const
{
    glm::ivec2 win_size;
    glfwGetWindowSize(WindowPtr, &win_size.x, &win_size.y);
    return win_size;
}

glm::ivec2 WindowHandle::Position() const
{
    glm::ivec2 win_pos;
    glfwGetWindowPos(WindowPtr, &win_pos.x, &win_pos.y);
    return win_pos;
}

void WindowHandle::Close(bool close)
{
    glfwSetWindowShouldClose(WindowPtr, close);
}

void WindowHandle::MakeCurrentContext() const
{
    glfwMakeContextCurrent(WindowPtr);
}
