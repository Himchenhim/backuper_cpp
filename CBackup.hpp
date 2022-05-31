#pragma once
#include "CDataUnit.hpp"
#include "CDirectory.hpp"


class CBackup{
private:
    // тут будет храниться дерево, с полной иерархией папок, файлов и ссылок
    string name_of_backup;
    string hash_of_backup;

    // проверять на то, что IsDirectory
    shared_ptr<CDirectory> root_directory;
    bool SaveBackup() const;

public:
    CBackup(string name);
    // если бэкапа с заданным именем не существует, то возвращаем false, или кидаем ошибку
    // меняет все файлы в директории на те, которые были подняты с бэкапа
    bool ReadBackup(const string & name_of_backup_to_read);
    ostream & ShowFileHierarchy(ostream & os);
    string GetName()const{return name_of_backup;}
    string GetHash()const{return hash_of_backup;}
};



