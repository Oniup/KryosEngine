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

#pragma once

#include "Core/InputKeyCodes.h"
#include "RHI/WindowHandle.h"
#include <array>
#include <string_view>

enum InputType {
    InputType_Unknown = -1,
    InputType_Keyboard,
    InputType_Mouse,
    InputType_GamePad,
};

enum MouseMode {
    MouseMode_Invalid = -1,
    MouseMode_Visable,
    MouseMode_Hidden,
    MouseMode_Captured,

};

std::string_view InputTypeToString(InputType type);
std::string_view MouseModeToString(MouseMode mode);

struct RegisteredInput {
    InputType Type      = InputType_Unknown;
    int Code            = -1;
    bool RemoveNextTick = false;
    bool Pressed        = false;
};

struct InputContext {
    static constexpr size_t RegisterOnceBufferSize = 16;

    WindowHandle* WindowPtr = nullptr;
    size_t RegActiveCount   = 0;
    std::array<RegisteredInput, RegisterOnceBufferSize> RegBuffer;

    static InputContext Create(WindowHandle* window);

    void PollEvents();
    bool RegisterOnce(InputType type, int code, bool pressed);
};

namespace Input {

bool KeyPressed(KeyCode code);
bool KeyReleased(KeyCode code);

bool KeyPress(KeyCode code);
bool KeyRelease(KeyCode code);

bool MousePressed(MouseButton button);
bool MouseReleased(MouseButton button);

bool MousePress(MouseButton button);
bool MouseRelease(MouseButton button);

} // namespace Input
