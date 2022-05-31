#pragma once
#include "CBackup.hpp"
#include <filesystem>
#include <iostream>
#include <utility>

namespace fs = std::filesystem;
using std::cout;
using std::endl;
using std::make_shared;

void init_directory();
void ReadPreviousBackups(set <shared_ptr<CBackup>> & backups);
uintmax_t ComputeFileSize (const fs::path& pathToCheck);
void DisplayFileInfo(const fs::directory_entry & entry, string & lead, fs::path &filename);
void CreateBackup (set <shared_ptr<CBackup>> & all_backups, string name);
void ShowBackups(set< shared_ptr<CBackup>> &backups);