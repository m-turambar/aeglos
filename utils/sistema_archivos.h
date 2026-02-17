#pragma once

#include <vector>
#include <iostream>
#include <filesystem>

extern std::filesystem::path file_dialog_script;

std::vector<std::string> get_file_names(std::string dir_name);

struct rstp_config {
    std::string username;
    std::string password;
    std::string ipaddr;

    static rstp_config get();
private:
    static bool load(const std::filesystem::path& file, rstp_config& config);
    static bool request(const std::filesystem::path& script, const std::filesystem::path& config_file);
};