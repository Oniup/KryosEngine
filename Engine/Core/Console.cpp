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

#include "Core/Console.h"
#include <fmt/color.h>

static Console* InstancePtr = nullptr;

const char* ConsoleMessage::SeverityFlagToCString(ConsoleMessage::Severity severity)
{
    switch (severity) {
        case ConsoleMessage::Trace:
            return "Trace";
        case ConsoleMessage::Verbose:
            return "Verbose";
        case ConsoleMessage::Info:
            return "Info";
        case ConsoleMessage::Warning:
            return "Warning";
        case ConsoleMessage::Error:
            return "Error";
        case ConsoleMessage::Fatal:
            return "Fatal";
        default:
            return "Invalid";
    }
}

std::string ConsoleOutput::_FormatHead(const ConsoleMessage& msg)
{
    fmt::text_style style;
    if (Flags & ConsoleOutput_ColorBit) {
        switch (msg.SeverityFlag) {
            case ConsoleMessage::Trace:
                style = fmt::fg(fmt::color::dim_gray);
                break;
            case ConsoleMessage::Verbose:
                style = fmt::fg(fmt::color::dim_gray);
                break;
            case ConsoleMessage::Info:
                style = fmt::fg(fmt::color::sky_blue);
                break;
            case ConsoleMessage::Warning:
                style = fmt::emphasis::italic | fmt::fg(fmt::color::yellow);
                break;
            case ConsoleMessage::Error:
                style =
                    fmt::emphasis::italic | fmt::emphasis::bold | fmt::fg(fmt::color::orange_red);
                break;
            case ConsoleMessage::Fatal:
                style = fmt::emphasis::italic | fmt::fg(fmt::color::white) |
                        fmt::bg(fmt::color::dark_red);
                break;
            default:
                break;
        }
    }

    if (msg.Context != nullptr) {
        return fmt::format(style, "{} [{}]",
                           ConsoleMessage::SeverityFlagToCString(msg.SeverityFlag), msg.Context);
    }
    else {
        return fmt::format(style, "{}", ConsoleMessage::SeverityFlagToCString(msg.SeverityFlag));
    }
}

std::string ConsoleOutput::_FormatBody(const ConsoleMessage& msg)
{
    std::string body;
    body.append(fmt::format("{}", (Flags & ConsoleOutput_BreakAfterHeaderBit) ? "\n" : " "));
    bool include_meta_info = false;
    if (msg.SeverityFlag > ConsoleMessage::Info) {
        include_meta_info = true;
        if (Flags & ~ConsoleOutput_FilterFileBit) {
            body.append(fmt::format("file={} ", msg.File));
        }
        if (Flags & ~ConsoleOutput_FilterLineBit) {
            body.append(fmt::format("line={} ", msg.Line));
        }
        if (Flags & ~ConsoleOutput_FilterFunctionBit) {
            body.append(fmt::format("func={} ", msg.Function));
        }
    }
    if (include_meta_info) {
        body.append(fmt::format("{}{}", (Flags & ConsoleOutput_BreakAfterInfoBit) ? "\n" : "",
                                msg.Message));
    }
    else {
        body.append(msg.Message);
    }
    return body;
}

void ConsoleTerminalOutput::Initialize(uint32_t flags)
{
    if (flags != ConsoleOutput_NoneBit) {
        Flags = flags;
    }
}

void ConsoleTerminalOutput::PrintOutput(const ConsoleMessage& msg)
{
    FILE* out = stdout;
    if (msg.SeverityFlag > ConsoleMessage::Warning) {
        out = stderr;
    }
    fmt::println(out, "{}{}", _FormatHead(msg), _FormatBody(msg));
    if (Flags & ConsoleOutput_FlushPerMessageBit) {
        std::fflush(out);
    }
}

void Console::PrintToOutputs(int line, const std::string& msg, const char* file,
                             const char* function, const char* context,
                             ConsoleMessage::Severity severity)
{
    ConsoleMessage message {
        .Line = line,
        .Message = msg,
        .File = file,
        .Function = function,
        .Context = context,
        .SeverityFlag = severity,
    };
    for (ConsoleOutput* output : InstancePtr->Outputs) {
        if (InstancePtr->SeverityFlags & severity) {
            output->PrintOutput(message);
        }
    }
}

void Console::Initialize(int severity_flags)
{
    SeverityFlags = severity_flags;
    InstancePtr = this;
}

void Console::Destroy()
{
    for (ConsoleOutput* output : Outputs) {
        output->Destroy();
        delete output;
    }
}

void Console::AddOutput(ConsoleOutput* output, uint32_t flags)
{
    output->Initialize(flags);
    Outputs.push_back(output);
}
