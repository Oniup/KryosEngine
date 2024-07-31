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
#include <Core/Console.h>
#include <RHI/Context.h>
#include <RHI/Shader.h>

int main()
{
    Console console;
    RHIContext context;
    Input input;

    console.Initialize();
    console.AddOutput<ConsoleTerminalOutput>();

    context.Initialize("KryosEngine");
    input.Initialize(context.Window);

    while (!context.Window.Closing()) {
        context.Window.SwapBuffers();
        input.PollEvents();
    }

    context.Destroy();
    console.Destroy();
}
