// This file is part of https://github.com/Oniup/KryosEngine
// Copyright (c) 2024 Oniup (https://github.com/Oniup)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "RHI/WindowHandle.h"
#include "Core/Console.h"

void WindowHandle::InitializeGLFW()
{
    RHI_CONDITION_FATAL(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW");
    glfwSetErrorCallback([](int error_number, const char* description) {
        CONTEXT_ERROR("GLFW", "{} => {}", error_number, description);
    });
}

void WindowHandle::TerminateGLFW()
{
    glfwTerminate();
}

bool WindowHandle::ValidMode(int flags)
{
    int window_modes = WindowHandle_WindowedModeBit | WindowHandle_BorderlessModeBit |
                       WindowHandle_FullscreenModeBit;

    int enabled_mode = window_modes & flags;

    bool mode = enabled_mode == 0 || enabled_mode == WindowHandle_WindowedModeBit ||
                enabled_mode == WindowHandle_BorderlessModeBit ||
                enabled_mode == WindowHandle_FullscreenModeBit;

    return mode;
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
