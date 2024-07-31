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

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#ifdef KRYOS_RHI_OPENGL

enum ShaderType {
    ShaderType_Invalid  = 0,
    ShaderType_Vertex   = 1 << 0,
    ShaderType_Fragment = 1 << 1,
    ShaderType_Geometry = 1 << 2,
    ShaderType_Compute  = 1 << 3,
};

struct RHIShaderBinarySource {
    ShaderType Type = ShaderType_Invalid;
    std::vector<std::byte> BinaryCode;
};

struct RHIShader {
    int Types   = ShaderType_Invalid;
    uint32_t Id = 0;
};

struct RHIShaderProgram {
    ShaderType Type = ShaderType_Invalid;
    uint32_t Id     = 0;
};

#elif KRYOS_RHI_VULKAN
// TODO: ...
#endif

std::string LoadRHIShaderSource(const std::string_view& file_path);
RHIShaderProgram CompileRHIShaderSource(ShaderType type, const std::string& source);

bool InitializeRHIShader(RHIShader* shader, const std::vector<RHIShaderProgram>& programs);
void DestroyRHIShader(RHIShader* shader);
