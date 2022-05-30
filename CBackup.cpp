#include "CBackup.hpp"
#include "CDirectory.hpp"
#include <filesystem>

namespace fs = std::filesystem;
using std::make_shared;
using std::shared_ptr;

CBackup::CBackup(string name) : name_of_backup(move(name)){
    root_directory = make_shared<CDirectory>(fs::current_path().c_str(),true);
}



