// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef BASH_CC145ECA_2335_4A2E_AA21_EBE0E2832CC2
#define BASH_CC145ECA_2335_4A2E_AA21_EBE0E2832CC2
#include <string>

namespace kon {
int execute_bash_file(std::string_view file_name, std::string_view args, std::string& output);
int execute_bash_script(std::string_view script, std::string_view args, std::string& output);
} // namespace kon
#endif // bash.hpp