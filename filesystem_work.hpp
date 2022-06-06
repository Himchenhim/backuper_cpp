#pragma once
#include "CBackup.hpp"
#include <filesystem>
#include <iostream>
#include <utility>
#include <vector>
#include <map>


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
void ReadPreviousBackups(vector<shared_ptr<CBackup>> & backups);
void CreateBackup (vector<shared_ptr<CBackup>> & all_backups, string name);
void ReturnBackup (vector< shared_ptr<CBackup> > & all_backups, const string & name);
void ShowBackups(vector< shared_ptr<CBackup> > &backups);