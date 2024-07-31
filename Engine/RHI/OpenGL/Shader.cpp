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

#ifdef KRYOS_RHI_OPENGL

#    include "RHI/Shader.h"
#    include "Core/Console.h"
#    include <glad/glad.h>

enum GLShaderType {
    GLShaderType_Invalid  = -1,
    GLShaderType_Vertex   = 0x8B31,
    GLShaderType_Fragment = 0x8B30,
    GLShaderType_Geometry = 0x8DD9,
    GLShaderType_Compute  = 0x91B9,
};

static GLShaderType ShaderTypeToGLShaderType(ShaderType type)
{
    switch (type) {
    case ShaderType_Invalid:
        return GLShaderType_Invalid;
        break;
    case ShaderType_Vertex:
        return GLShaderType_Vertex;
        break;
    case ShaderType_Fragment:
        return GLShaderType_Fragment;
        break;
    case ShaderType_Geometry:
        return GLShaderType_Geometry;
        break;
    case ShaderType_Compute:
        return GLShaderType_Compute;
        break;
    }
}

std::string LoadRHIShaderSource(const std::string_view& file_path)
{
    std::FILE* file = std::fopen(file_path.data(), "rb");
    RHI_CONDITION_ERROR_RETURN(file != nullptr, std::string(nullptr), " Cannot open shader file");

    std::fseek(file, 0, SEEK_END);
    size_t file_size = std::ftell(file);
    std::fseek(file, 0, SEEK_SET);

    std::string source(file_size + 1, '\0');
    int result = std::fread(source.data(), file_size, sizeof(char), file);
    if (result != file_size) {
        RHI_ERROR("Failed to read shader source file at {}", file_path);
        std::fclose(file);
        return std::string(nullptr);
    }

    std::fclose(file);
    return source;
}

RHIShaderProgram CompileRHIShaderSource(ShaderType type, const std::string& source)
{
    RHI_CONDITION_ERROR_RETURN(source.empty(), RHIShaderProgram {}, "Shader source is empty");
    RHI_CONDITION_ERROR_RETURN(source[source.size()] == '\0', RHIShaderProgram {},
                               "Shader source is not null-terminated");

    RHIShaderProgram program = {
        .Type = type,
        .Id   = glCreateShader(ShaderTypeToGLShaderType(type)),
    };

    const char* ptr               = source.c_str();
    const char* const* source_ptr = &ptr;
    glShaderSource(program.Id, 1, source_ptr, nullptr);
    glCompileShader(program.Id);

    int success_compile_shader;
    glGetShaderiv(program.Id, GL_COMPILE_STATUS, &success_compile_shader);
    if (!success_compile_shader) {
        constexpr size_t log_length = 512;
        char info_log[log_length];
        glGetShaderInfoLog(program.Id, log_length, nullptr, info_log);
        RHI_CONDITION_ERROR_RETURN(success_compile_shader, RHIShaderProgram {},
                                   "Failed to compile shader\nGL Error: {}", info_log);
    }
    return program;
}

bool InitializeRHiShader(RHIShader* shader, const std::vector<RHIShaderProgram>& programs)
{
    shader->Id = glCreateProgram();
    for (const RHIShaderProgram& program : programs) {
        shader->Types |= program.Type;
        glAttachShader(shader->Id, program.Id);
    }

    glLinkProgram(shader->Id);
    for (const RHIShaderProgram& program : programs) {
        glDeleteShader(program.Id);
    }

    int success_link_programs;
    glGetProgramiv(shader->Id, GL_LINK_STATUS, &success_link_programs);
    if (!success_link_programs) {
        constexpr size_t log_length = 512;
        char info_log[log_length];
        glGetProgramInfoLog(shader->Id, log_length, nullptr, info_log);
        RHI_CONDITION_ERROR_RETURN(success_link_programs, false,
                                   "Failed to compile shader:\nGL Error: {}", info_log);
    }
    return true;
}

void DestroyRHIShader(RHIShader* shader)
{
    RHI_CONDITION_ERROR(shader != nullptr, "Cannot destroy shader. Pointer is null");
    if (shader->Id != -1) {
        glDeleteProgram(shader->Id);
    }
    delete shader;
}

#endif
