// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#include <kon/tools/bash.hpp>
#include <kon/file_helper.hpp>
#include <filesystem>
#include <fmt/base.h>

namespace kon {

int execute_bash_file(std::string_view file_name, std::string_view args, std::string& output) {
    std::string cmd{"bash \""};
    cmd.append(file_name);
    cmd.append("\" ");
    cmd.append(args);
    cmd.append(" 2>&1");

    // cmd: bash file_name arg1 arg2 ... 2>&1
    FILE* pipe = ::popen(cmd.c_str(), "r");
    if (!pipe) {
        return -1;
    }
    constexpr std::size_t buffer_size{512};
    auto buffer = std::make_unique_for_overwrite<char[]>(buffer_size);
    std::size_t n;
    while ((n = fread(buffer.get(), 1, buffer_size, pipe)) > 0) {
        output.append(buffer.get(), n);
    }

    int status = pclose(pipe);
    int exit_code = -1;
    if (WIFEXITED(status)) {
        exit_code = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        exit_code = -2;
    }
    return exit_code;
}

int execute_bash_script(std::string_view script, std::string_view args, std::string& output) {
    std::string&& temp_file_name = kon::file_helper::create_tempfile(".kon-bash-script", script);
    if (temp_file_name.empty()) {
        return -1;
    }
    int error = execute_bash_file(temp_file_name, args, output);
    std::error_code ec;
    std::filesystem::remove(temp_file_name, ec);
    return error;
}
} // namespace kon
