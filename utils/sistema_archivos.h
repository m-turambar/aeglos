#pragma once

#include <vector>
#include <iostream>
#include <filesystem>

extern std::filesystem::path file_dialog_script;

std::vector<std::string> get_file_names(std::string dir_name);