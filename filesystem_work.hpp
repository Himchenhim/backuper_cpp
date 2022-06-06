#pragma once
#include "CBackup.hpp"
#include <filesystem>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <ctime>


class CBackup;

namespace fs = std::filesystem;
using std::vector;

using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;
using std::make_unique;
using std::map;

void init_directory();
int init_time_mode();
void ReadPreviousBackups(vector<shared_ptr<CBackup>> & backups);
void CreateBackup (vector<shared_ptr<CBackup>> & all_backups, string name);
void CreateBackupOnTime();
void ReturnBackup (vector< shared_ptr<CBackup> > & all_backups, const string & name);
void ShowBackups(vector< shared_ptr<CBackup> > &backups);
void SaveInfoToScheduleMode(int mode, const std::chrono::time_point<std::chrono::system_clock> &time_after_backup);