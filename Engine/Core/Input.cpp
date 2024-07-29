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

#include "Core/Input.h"
#include "Core/Console.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

static Input* s_InstancePtr = nullptr;

#define KY_REG_ONCE_IMPL(func, input_type, code, pressed)                                         \
    bool result = func(code);                                                                     \
    if (result) {                                                                                 \
        result = s_InstancePtr->_RegisterOnce(input_type, code, pressed);                         \
    }                                                                                             \
    return result

#define KY_REG_ONCE_WITH_HANDLE_IMPL(func, handle, input_type, code, pressed)                     \
    bool result = func(handle, code);                                                             \
    if (result) {                                                                                 \
        result = s_InstancePtr->_RegisterOnce(input_type, code, pressed);                         \
    }                                                                                             \
    return result

void Input::Initialize(WindowHandle& window)
{
    s_InstancePtr            = this;
    s_InstancePtr->WindowPtr = &window;
    for (RegisteredInput& reg : s_InstancePtr->RegBuffer) {
        reg.Type = InputType_Unknown;
    }
}

bool Input::KeyPressed(KeyCode code)
{
    KY_REG_ONCE_IMPL(KeyPress, InputType_Keyboard, code, true);
}

bool Input::KeyReleased(KeyCode code)
{
    KY_REG_ONCE_IMPL(KeyRelease, InputType_Keyboard, code, false);
}

bool Input::KeyPress(KeyCode code)
{
    return glfwGetKey(s_InstancePtr->WindowPtr->WindowPtr, code) == GLFW_PRESS;
}

bool Input::KeyRelease(KeyCode code)
{
    return glfwGetKey(s_InstancePtr->WindowPtr->WindowPtr, code) == GLFW_RELEASE;
}

bool Input::MousePressed(MouseButton button)
{
    KY_REG_ONCE_IMPL(MousePress, InputType_Mouse, button, true);
}

bool Input::MouseReleased(MouseButton button)
{
    KY_REG_ONCE_IMPL(MouseRelease, InputType_Mouse, button, false);
}

bool Input::MousePress(MouseButton button)
{
    return glfwGetMouseButton(s_InstancePtr->WindowPtr->WindowPtr, button) == GLFW_PRESS;
}

bool Input::MouseRelease(MouseButton button)
{
    return glfwGetMouseButton(s_InstancePtr->WindowPtr->WindowPtr, button) == GLFW_RELEASE;
}

std::string_view Input::TypeToString(InputType type)
{
    switch (type) {
    case InputType_Keyboard:
        return std::string_view("Keyboard");
    case InputType_Mouse:
        return std::string_view("Mouse");
    case InputType_GamePad:
        return std::string_view("Game Pad");
    default:
        return std::string_view("Unknown");
    }
}

std::string_view Input::MouseModeToString(MouseMode mode)
{
    switch (mode) {
    case MouseMode::MouseMode_Visable:
        return std::string_view("Visable");
    case MouseMode::MouseMode_Hidden:
        return std::string_view("Hidden");
    case MouseMode::MouseMode_Captured:
        return std::string_view("Captured");
    default:
        return std::string_view("Invalid");
    }
}

void Input::PollEvents()
{
    glfwPollEvents();
    size_t reg_counted = 0;
    for (RegisteredInput& reg : s_InstancePtr->RegBuffer) {
        if (reg_counted == s_InstancePtr->RegActiveCount) {
            break;
        }
        else if (reg.Type == InputType_Unknown) {
            continue;
        }
        reg_counted++;

        if (!reg.RemoveNextTick) {
            reg.RemoveNextTick = true;
        }
        else {
            reg.Type = InputType_Unknown;
            s_InstancePtr->RegActiveCount--;
        }
    }
}

bool Input::_RegisterOnce(InputType type, int code, bool pressed)
{
    int free_index = -1;
    for (size_t i = 0; i < RegBuffer.size(); i++) {
        if (RegBuffer[i].Type == InputType_Unknown) {
            free_index = i;
            continue;
        }
        bool found = RegBuffer[i].Type == type && RegBuffer[i].Code == code &&
                     RegBuffer[i].Pressed == pressed;
        if (found) {
            RegBuffer[i].RemoveNextTick = false;
            return false;
        }
    }

    CONTEXT_CONDITION_ERROR_RETURN("INPUT", free_index != -1, false,
                                   "Toggle register buffer full, cannot add {} {}",
                                   TypeToString(type), code);

    RegBuffer[free_index] = RegisteredInput {
        .Type           = type,
        .Code           = code,
        .RemoveNextTick = false,
        .Pressed        = pressed,
    };
    RegActiveCount++;
    return true;
}
