#pragma once
#include "CDataUnit.hpp"
#include "CDirectory.hpp"
#include "filesystem_work.hpp"

class CBackup{
private:
    // тут будет храниться дерево, с полной иерархией папок, файлов и ссылок
    string name_of_backup;
    string hash_of_backup;
    string hash_of_data;
    mutable time_t time_of_backup{};

    // проверять на то, что IsDirectory
    shared_ptr<CDirectory> root_directory;
    bool SaveBackup();

public:
    CBackup(string name);
    CBackup(std::fstream & input_stream);

    // если бэкапа с заданным именем не существует, то возвращаем false, или кидаем ошибку
    // меняет все файлы в директории на те, которые были подняты с бэкапа

    bool ReadBackup(const string & name_of_backup_to_read);
    void ReturnBackupToDirectory(const string & path);
    ostream & ShowFileHierarchy(ostream & os);

    string GetName()const{return name_of_backup;}
    string GetHashData()const{return hash_of_data;}
    string GetHashBackup()const{return hash_of_backup;}
    time_t GetTime()const{return time_of_backup;}

};



