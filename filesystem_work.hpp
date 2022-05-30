#pragma once
#include "CBackup.hpp"
#include <filesystem>
#include <iostream>
#include <utility>

namespace fs = std::filesystem;
using std::cout;
using std::endl;
using std::make_shared;
uintmax_t ComputeFileSize (const fs::path& pathToCheck)
{
    if (fs::exists(pathToCheck) &&
        fs::is_regular_file(pathToCheck))
    {
        auto err = std::error_code{};
        auto filesize = fs::file_size(pathToCheck, err);
        if ( filesize != static_cast<uintmax_t>(-1))
            return filesize;
    }
    return static_cast<uintmax_t>(-1);
}
void DisplayFileInfo(const fs::directory_entry & entry, string & lead, fs::path &filename)
{
    cout << lead << " " << filename.c_str()<< ", " << ComputeFileSize(entry) << endl;
}
void CreateBackup (set <shared_ptr<CBackup>> & all_backups, string name){
    // iterator on our new backup
    all_backups.insert(make_shared<CBackup>( move(name) ));
}