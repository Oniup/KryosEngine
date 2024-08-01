
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

#define GLFW_INCLUDE_NONE

#include "Renderer/Context.h"
#include "Core/Console.h"
#include <GLFW/glfw3.h>

RendererContext RendererContext::Create(const std::string_view& title, int width, int height,
                                        int flags)
{
    RHI_CONDITION_FATAL(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW");
    glfwSetErrorCallback([](int error_number, const char* description) {
        CONTEXT_ERROR("GLFW", "{} => {}", error_number, description);
    });

    RendererContext renderer = {
        .Window = WindowHandle::Create(title, width, height, flags),
    };
    return renderer;
}

void RendererContext::Destroy()
{
    Window.Destroy();
    glfwTerminate();
}
