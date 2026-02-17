#include "sistema_archivos.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

std::filesystem::path file_dialog_script =
    std::filesystem::path(PROJECT_SOURCE_DIR) /
    "scripts" /
    "file_dialog.py";
 
vector<string> get_file_names(std::string dir_name)
{
    vector<string> files;
    std::cout << "iterando sobre " << dir_name << std::endl;
    for(auto& p: fs::directory_iterator(dir_name)) {
        #ifdef _WIN32
        string s = p.path().string();
        #else
        string s = p.path();
        #endif
        std::cout << s << '\n';
        files.push_back(s);
    }
    return files;
}

bool rstp_config::load(const fs::path& file, rstp_config& config)
{
    if (!fs::exists(file))
        return false;

    std::ifstream in(file);
    if (!in)
        return false;

    std::getline(in, config.username);
    std::getline(in, config.password);
    std::getline(in, config.ipaddr);

    return !config.username.empty() && !config.password.empty() && !config.ipaddr.empty();
}

rstp_config rstp_config::get()
{
    fs::path root = PROJECT_SOURCE_DIR;
    fs::path config_file = root / "data" / "rstp_config.txt";
    fs::path script = root / "scripts" / "rstp_config_dialog.py";

    rstp_config config;
    if (rstp_config::load(config_file, config))
        return config;

    std::cout << "RSTP config file not found. Opening dialog...\n";
    if (!rstp_config::request(script, config_file))
    {
        throw std::runtime_error("Failed to obtain RSTP config from dialog");
    }

    if (!rstp_config::load(config_file, config))
    {
        throw std::runtime_error("RSTP Config file still invalid");
    }

    return config;
}

bool rstp_config::request(const fs::path& script, const fs::path& config_file)
{
#ifdef _WIN32
    std::string python = "python";
#else
    std::string python = "python3";
#endif

    std::string command =
        python + " \"" +
        script.string() + "\" \"" +
        config_file.string() + "\"";

    std::cout << "Running command: " << command << std::endl;

    return std::system(command.c_str()) == 0;
}
