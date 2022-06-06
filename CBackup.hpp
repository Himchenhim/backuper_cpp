#pragma once
#include "CDataUnit.hpp"
#include "CDirectory.hpp"

#include "filesystem_work.hpp"

class CBackup{
private:
    // тут будет храниться дерево, с полной иерархией папок, файлов и ссылок
    string name_of_backup;
    string hash_of_data;
    string hash_of_backup;
    time_t time_of_backup{};

    // проверять на то, что IsDirectory
    shared_ptr<CDirectory> root_directory;
    bool SaveBackup();

public:
    // for creating from directories
    CBackup(string name);
    CBackup(const CBackup & src);
    // for creating from saving file
    CBackup(std::fstream & input_stream);

    // если бэкапа с заданным именем не существует, то возвращаем false, или кидаем ошибку
    // меняет все файлы в директории на те, которые были подняты с бэкапа

    void ReturnBackupToDirectory();

    void ShowBackup();


    string GetName()const{return name_of_backup;}
    string GetHashData()const{return hash_of_data;}
    string GetHashBackup()const{return hash_of_backup;}
    time_t GetTime()const{return time_of_backup;}


    friend void CompareBackups(const CBackup & old_backup, const CBackup & new_backup);
    friend void CompareBackupWithActualState(const CBackup & backup);
    friend void CompareBackups(const CBackup & f_backup, const CBackup & s_backup);
};




const CBackup & FindBackup(vector <shared_ptr<CBackup>> & all_backups, const string & name_of_backup);

// return it's hash






