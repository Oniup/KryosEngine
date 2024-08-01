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

static InputContext* s_InputInstance = nullptr;

std::string_view InputTypeToString(InputType type)
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

std::string_view MouseModeToString(MouseMode mode)
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

InputContext InputContext::Create(WindowHandle* window)
{
    InputContext input = {
        .WindowPtr      = window,
        .RegActiveCount = 0,
        .RegBuffer      = {},
    };
    for (RegisteredInput& reg : input.RegBuffer) {
        reg.Type = InputType_Unknown;
    }
    s_InputInstance = &input;
    return input;
}

void InputContext::PollEvents()
{
    glfwPollEvents();
    size_t reg_counted = 0;
    for (RegisteredInput& reg : s_InputInstance->RegBuffer) {
        if (reg_counted == s_InputInstance->RegActiveCount) {
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
            s_InputInstance->RegActiveCount--;
        }
    }
}

bool InputContext::RegisterOnce(InputType type, int code, bool pressed)
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
                                   InputTypeToString(type), code);

    RegBuffer[free_index] = RegisteredInput {
        .Type           = type,
        .Code           = code,
        .RemoveNextTick = false,
        .Pressed        = pressed,
    };
    RegActiveCount++;
    return true;
}

#define REG_ONCE_IMPL(func, input_type, code, pressed)                                            \
    bool result = func(code);                                                                     \
    if (result) {                                                                                 \
        result = s_InputInstance->RegisterOnce(input_type, code, pressed);                        \
    }                                                                                             \
    return result

bool Input::KeyPressed(KeyCode code)
{
    REG_ONCE_IMPL(Input::KeyPress, InputType_Keyboard, code, true);
}

bool Input::KeyReleased(KeyCode code)
{
    REG_ONCE_IMPL(Input::KeyRelease, InputType_Keyboard, code, false);
}

bool Input::KeyPress(KeyCode code)
{
    return glfwGetKey(s_InputInstance->WindowPtr->WindowPtr, code) == GLFW_PRESS;
}

bool Input::KeyRelease(KeyCode code)
{
    return glfwGetKey(s_InputInstance->WindowPtr->WindowPtr, code) == GLFW_RELEASE;
}

bool Input::MousePressed(MouseButton button)
{
    REG_ONCE_IMPL(Input::MousePress, InputType_Mouse, button, true);
}

bool Input::MouseReleased(MouseButton button)
{
    REG_ONCE_IMPL(Input::MouseRelease, InputType_Mouse, button, false);
}

bool Input::MousePress(MouseButton button)
{
    return glfwGetMouseButton(s_InputInstance->WindowPtr->WindowPtr, button) == GLFW_PRESS;
}

bool Input::MouseRelease(MouseButton button)
{
    return glfwGetMouseButton(s_InputInstance->WindowPtr->WindowPtr, button) == GLFW_RELEASE;
}
