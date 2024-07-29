#ifdef KRYOS_RHI_OPENGL

#    include "RHI/WindowHandle.h"
#    include "Core/Console.h"
#    include <glad/glad.h>

static bool s_GladInitialized = false;

void WindowHandle::Initialize(const std::string_view& title, int width, int height, int flags)
{
    RHI_CONDITION_FATAL(title[title.size()] == '\0', "Title string must be null terminated");
    RHI_CONDITION_FATAL(ValidMode(flags),
                        "Can only create window with one or none of the window modes");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, (flags & WindowHandle_ResizeableBit) != 0);
    glfwWindowHint(GLFW_DECORATED, (flags & ~WindowHandle_BorderlessModeBit) != 0);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, (flags & WindowHandle_TransparentBufferBit) != 0);

    GLFWmonitor* monitor       = glfwGetPrimaryMonitor();
    const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
    if (width < 0 | height < 0) {
        width  = vidmode->width / 2;
        height = vidmode->height / 2;
    }

    if (flags & ~WindowHandle_FullscreenModeBit) {
        monitor = nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, title.data(), monitor, nullptr);
    RHI_CONDITION_FATAL(window != nullptr, "Failed to create GLFW window");
    glfwMakeContextCurrent(window);

    if (!s_GladInitialized) {
        s_GladInitialized = true;
        RHI_CONDITION_FATAL(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
                            "Failed to initialize GLAD");
    }

    if (flags & WindowHandle_VsyncBit) {
        glfwSwapInterval(0);
    }
    else {
        glfwSwapInterval(1);
    }

    WindowPtr = window;
    Flags     = flags;
}

void WindowHandle::Destroy()
{
    RHI_CONDITION_ERROR(WindowPtr != nullptr, "WindowHandle is nullptr, Cannot destroy window");
    glfwDestroyWindow(WindowPtr);
    WindowPtr = nullptr;
    Flags     = 0;
}

void WindowHandle::SwapBuffers()
{
    glfwSwapBuffers(WindowPtr);
}

#endif
