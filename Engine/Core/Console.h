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

#ifdef _MSC_VER
#    define INTERNAL_GENERATE_TRAP() __debugbreak()
#else
#    define INTERNAL_GENERATE_TRAP() __builtin_trap()
#endif

#define FUNCTION_STR __FUNCTION__

// Internal
// ------------------------------------------------------------------------------------------------
#define INTERNAL_MSG(_context, _severity, ...)                                                    \
    Console::PrintToOutputs(__LINE__, fmt::format(__VA_ARGS__), __FILE__, FUNCTION_STR,           \
                            (_context), ConsoleMessage::_severity)

#define INTERNAL_MSG_RETURN(_context, _returning, _severity, ...)                                 \
    Console::PrintToOutputs(__LINE__, fmt::format(__VA_ARGS__), __FILE__, FUNCTION_STR,           \
                            (_context), ConsoleMessage::_severity);                               \
    return (_returning)

#ifndef NDEBUG
#    define INTERNAL_FATAL_MSG(_context, _severity, ...)                                          \
        Console::PrintToOutputs(__LINE__, fmt::format(__VA_ARGS__), __FILE__, FUNCTION_STR,       \
                                (_context), ConsoleMessage::_severity);                           \
        INTERNAL_GENERATE_TRAP()
#else
#    define INTERNAL_FATAL_MSG(_context, _severity, ...)
#endif

#define INTERNAL_CONDITION(_context, _condition, _severity, ...)                                  \
    if (!(_condition)) {                                                                          \
        Console::PrintToOutputs(                                                                  \
            __LINE__, fmt::format(fmt::format("`{}` == FALSE: ", #_condition) + __VA_ARGS__),     \
            __FILE__, FUNCTION_STR, (_context), ConsoleMessage::_severity);                       \
        return;                                                                                   \
    }                                                                                             \
    else                                                                                          \
        ((void)0)

#define INTERNAL_CONDITION_RETURN(_context, _returning, _condition, _severity, ...)               \
    if (!(_condition)) {                                                                          \
        Console::PrintToOutputs(                                                                  \
            __LINE__, fmt::format(fmt::format("`{}` == FALSE: ", #_condition) + __VA_ARGS__),     \
            __FILE__, FUNCTION_STR, (_context), ConsoleMessage::_severity);                       \
        return (_returning);                                                                      \
    }                                                                                             \
    else                                                                                          \
        ((void)0)

#ifndef NDEBUG
#    define INTERNAL_FATAL_CONDITION(_context, _condition, _severity, ...)                        \
        if (!(_condition)) {                                                                      \
            Console::PrintToOutputs(                                                              \
                __LINE__, fmt::format(fmt::format("`{}` == FALSE: ", #_condition) + __VA_ARGS__), \
                __FILE__, FUNCTION_STR, (_context), ConsoleMessage::_severity);                   \
            INTERNAL_GENERATE_TRAP();                                                             \
        }                                                                                         \
        else                                                                                      \
            ((void)0)
#else
#    define INTERNAL_FATAL_CONDITION(_context, _condition, _severity, ...) (_condition)
#endif

// Basic
// ------------------------------------------------------------------------------------------------
#define VERBOSE(...) INTERNAL_MSG(nullptr, Verbose, __VA_ARGS__)
#define TRACE(...) INTERNAL_MSG(nullptr, Trace, __VA_ARGS__)
#define INFO(...) INTERNAL_MSG(nullptr, Info, __VA_ARGS__)
#define WARN(...) INTERNAL_MSG(nullptr, Warning, __VA_ARGS__)
#define ERROR(...) INTERNAL_MSG(nullptr, Error, __VA_ARGS__)
#define FATAL(...) INTERNAL_FATAL_MSG(nullptr, Fatal, __VA_ARGS__)

#define CONTEXT_VERBOSE(_context, ...) INTERNAL_MSG(_context, Verbose, __VA_ARGS__)
#define CONTEXT_TRACE(_context, ...) INTERNAL_MSG(_context, Trace, __VA_ARGS__)
#define CONTEXT_INFO(_context, ...) INTERNAL_MSG(_context, Info, __VA_ARGS__)
#define CONTEXT_WARN(_context, ...) INTERNAL_MSG(_context, Warning, __VA_ARGS__)
#define CONTEXT_ERROR(_context, ...) INTERNAL_MSG(_context, Error, __VA_ARGS__)
#define CONTEXT_FATAL(_context, ...) INTERNAL_FATAL_MSG(_context, Fatal, __VA_ARGS__)

// Return Value
// ------------------------------------------------------------------------------------------------
#define WARN_RETURN(_returning, ...) INTERNAL_MSG_RETURN(nullptr, _returning, Warning, __VA_ARGS__)
#define ERROR_RETURN(_returning, ...) INTERNAL_MSG_RETURN(nullptr, _returning, Error, __VA_ARGS__)

#define CONTEXT_WARN_RETURN(_context, _returning, ...)                                            \
    INTERNAL_MSG_RETURN(_context, _returning, Warning, __VA_ARGS__)
#define CONTEXT_ERROR_RETURN(_context, _returning, ...)                                           \
    INTERNAL_MSG_RETURN(_context, _returning, Error, __VA_ARGS__)

// Conditions
// ------------------------------------------------------------------------------------------------
#define CONDITION_WARN(_condition, ...)                                                           \
    INTERNAL_CONDITION(nullptr, _condition, Warning, __VA_ARGS__);
#define CONDITION_ERROR(_condition, ...)                                                          \
    INTERNAL_CONDITION(nullptr, _condition, Error, __VA_ARGS__);
#define CONDITION_FATAL(_condition, ...)                                                          \
    INTERNAL_FATAL_CONDITION(nullptr, _condition, Fatal, __VA_ARGS__);

#define CONDITION_WARN_RETURN(_condition, _returning, ...)                                        \
    INTERNAL_CONDITION_RETURN(nullptr, _returning, _condition, Warning, __VA_ARGS__);
#define CONDITION_ERROR_RETURN(_condition, _returning, ...)                                       \
    INTERNAL_CONDITION_RETURN(nullptr, _returning, _condition, Error, __VA_ARGS__);

// Context
// ------------------------------------------------------------------------------------------------
#define CONTEXT_CONDITION_WARN(_context, _condition, ...)                                         \
    INTERNAL_CONDITION(_context, _condition, Warning, __VA_ARGS__);
#define CONTEXT_CONDITION_ERROR(_context, _condition, ...)                                        \
    INTERNAL_CONDITION(_context, _condition, Error, __VA_ARGS__);
#define CONTEXT_CONDITION_FATAL(_context, _condition, ...)                                        \
    INTERNAL_FATAL_CONDITION(_context, _condition, Fatal, __VA_ARGS__);

#define CONTEXT_CONDITION_WARN_RETURN(_context, _condition, _returning, ...)                      \
    INTERNAL_CONDITION_RETURN(_context, _returning, _condition, Warning, __VA_ARGS__);
#define CONTEXT_CONDITION_ERROR_RETURN(_context, _condition, _returning, ...)                     \
    INTERNAL_CONDITION_RETURN(_context, _returning, _condition, Error, __VA_ARGS__);

// Render Hardware Interface
// ------------------------------------------------------------------------------------------------
#ifdef KRYOS_RHI_OPENGL
#    define RHI_VERBOSE(...) CONTEXT_VERBOSE("OPENGL", __VA_ARGS__)
#    define RHI_TRACE(...) CONTEXT_TRACE("OPENGL", __VA_ARGS__)
#    define RHI_INFO(...) CONTEXT_INFO("OPENGL", __VA_ARGS__)
#    define RHI_WARN(...) CONTEXT_WARN("OPENGL", __VA_ARGS__)
#    define RHI_ERROR(...) CONTEXT_ERROR("OPENGL", __VA_ARGS__)
#    define RHI_FATAL(...) CONTEXT_FATAL("OPENGL", __VA_ARGS__)

#    define RHI_WARN_RETURN(_returning, ...) CONTEXT_WARN_RETURN("OPENGL", _returning, __VA_ARGS__)
#    define RHI_ERROR_RETURN(_returning, ...)                                                     \
        CONTEXT_ERROR_RETURN("OPENGL", _returning, __VA_ARGS__)
#    define RHI_FATAL_RETURN(_returning, ...)                                                     \
        CONTEXT_ERROR_RETURN("OPENGL", _returning, __VA_ARGS__)

#    define RHI_CONDITION_WARN(_condition, ...)                                                   \
        CONTEXT_CONDITION_WARN("OPENGL", _condition, __VA_ARGS__)
#    define RHI_CONDITION_ERROR(_condition, ...)                                                  \
        CONTEXT_CONDITION_ERROR("OPENGL", _condition, __VA_ARGS__)
#    define RHI_CONDITION_FATAL(_condition, ...)                                                  \
        CONTEXT_CONDITION_FATAL("OPENGL", _condition, __VA_ARGS__)

#    define RHI_CONDITION_WARN_RETURN(_condition, _returning, ...)                                \
        CONTEXT_CONDITION_WARN_RETURN("OPENGL", _condition, _returning, __VA_ARGS__)
#    define RHI_CONDITION_ERROR_RETURN(_condition, _returning, ...)                               \
        CONTEXT_CONDITION_ERROR_RETURN("OPENGL", _condition, _returning, __VA_ARGS__)
#elif KRYOS_RHI_VULKAN
#    define RHI_VERBOSE(...) CONTEXT_VERBOSE("VULKAN", __VA_ARGS__)
#    define RHI_TRACE(...) CONTEXT_TRACE("VULKAN", __VA_ARGS__)
#    define RHI_INFO(...) CONTEXT_INFO("VULKAN", __VA_ARGS__)
#    define RHI_WARN(...) CONTEXT_WARN("VULKAN", __VA_ARGS__)
#    define RHI_ERROR(...) CONTEXT_ERROR("VULKAN", __VA_ARGS__)
#    define RHI_FATAL(...) CONTEXT_FATAL("VULKAN", __VA_ARGS__)

#    define RHI_WARN_RETURN(_returning, ...) CONTEXT_WARN_RETURN("VULKAN", _returning, __VA_ARGS__)
#    define RHI_ERROR_RETURN(_returning, ...)                                                     \
        CONTEXT_ERROR_RETURN("VULKAN", _returning, __VA_ARGS__)
#    define RHI_FATAL_RETURN(_returning, ...)                                                     \
        CONTEXT_ERROR_RETURN("VULKAN", _returning, __VA_ARGS__)

#    define RHI_CONDITION_WARN(_condition, ...)                                                   \
        CONTEXT_CONDITION_WARN("VULKAN", _condition, __VA_ARGS__)
#    define RHI_CONDITION_ERROR(_condition, ...)                                                  \
        CONTEXT_CONDITION_ERROR("VULKAN", _condition, __VA_ARGS__)
#    define RHI_CONDITION_FATAL(_condition, ...)                                                  \
        CONTEXT_CONDITION_FATAL("VULKAN", _condition, __VA_ARGS__)

#    define RHI_CONDITION_WARN_RETURN(_condition, _returning, ...)                                \
        CONTEXT_CONDITION_WARN_RETURN("VULKAN", _condition, _returning, __VA_ARGS__)
#    define RHI_CONDITION_ERROR_RETURN(_condition, _returning, ...)                               \
        CONTEXT_CONDITION_ERROR_RETURN("VULKAN", _condition, _returning, __VA_ARGS__)
#endif

#include <fmt/format.h>
#include <string>
#include <string_view>
#include <vector>

struct ConsoleMessage {
    enum Severity {
        Invalid = 0,
        Verbose = 1 << 0,
        Trace   = 1 << 1,
        Info    = 1 << 2,
        Warning = 1 << 3,
        Error   = 1 << 4,
        Fatal   = 1 << 5,
    };

    int Line = -1;
    std::string Message;
    const char* File      = nullptr;
    const char* Function  = nullptr;
    const char* Context   = nullptr;
    Severity SeverityFlag = ConsoleMessage::Invalid;

    static const char* SeverityFlagToCString(ConsoleMessage::Severity code);
};

enum ConsoleOutputFlags : uint32_t {
    ConsoleOutput_NoneBit             = 0,
    ConsoleOutput_FlushPerMessageBit  = 1 << 0,
    ConsoleOutput_ColorBit            = 1 << 1,
    ConsoleOutput_BreakAfterHeaderBit = 1 << 2,
    ConsoleOutput_BreakAfterInfoBit   = 1 << 3,
    ConsoleOutput_FilterFileBit       = 1 << 4,
    ConsoleOutput_FilterLineBit       = 1 << 5,
    ConsoleOutput_FilterFunctionBit   = 1 << 6,
};

struct ConsoleOutput {
    static constexpr int DefaultFlags = ConsoleOutput_FlushPerMessageBit | ConsoleOutput_ColorBit |
                                        ConsoleOutput_BreakAfterInfoBit |
                                        ConsoleOutput_BreakAfterHeaderBit;
    uint32_t Flags;

    virtual ~ConsoleOutput() {}

    virtual void Initialize(uint32_t flags = DefaultFlags) = 0;
    virtual void Destroy()                                 = 0;

    virtual std::string_view Name() const               = 0;
    virtual void PrintOutput(const ConsoleMessage& msg) = 0;

    virtual std::string _FormatHead(const ConsoleMessage& msg);
    virtual std::string _FormatBody(const ConsoleMessage& msg);
};

struct ConsoleTerminalOutput : public ConsoleOutput {
    void Initialize(uint32_t flags = ConsoleOutput::DefaultFlags) override;
    inline void Destroy() override {}

    inline std::string_view Name() const override { return "Error Terminal Output"; }
    void PrintOutput(const ConsoleMessage& msg) override;
};

struct Console {
    static constexpr int DefaultFlags = ConsoleMessage::Info | ConsoleMessage::Warning |
                                        ConsoleMessage::Error | ConsoleMessage::Fatal;

    std::vector<ConsoleOutput*> Outputs;
    int SeverityFlags;

    static void PrintToOutputs(int line, const std::string& msg, const char* file,
                               const char* function, const char* context,
                               ConsoleMessage::Severity severity);

    static Console Create(int severity_flags = DefaultFlags);

    template <typename... TOutputs>
    static Console CreateAndInitOutputs(int severity_flags = DefaultFlags,
                                        int output_flags   = ConsoleOutput::DefaultFlags)
    {
        Console console = Create(severity_flags);
        (console.AddOutput<TOutputs>(output_flags), ...);
        return console;
    }

    void Destroy();

    void AddOutput(ConsoleOutput* output, uint32_t flags = ConsoleOutput_NoneBit);

    template <typename TConsoleOutput>
    void AddOutput(uint32_t flags = ConsoleOutput_NoneBit)
    {
        AddOutput(new TConsoleOutput, flags);
    }
};
