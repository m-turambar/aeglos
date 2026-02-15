#include "sistema_archivos.h"
#include <fstream>
#include <iostream>
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
